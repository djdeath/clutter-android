/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2012  Intel Corporation.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author:
 *   Lionel Landwerlin <lionel.g.landwerlin@linux.intel.com>
 *
 * Based on clutter-drag-action.c
 */

/**
 * SECTION:clutter-zoom-action
 * @Title: ClutterZoomAction
 * @Short_Description: Action enabling dragging & zooming on actors
 *
 * #ClutterZoomAction is a sub-class of #ClutterAction that implements
 * all the necessary logic for dragging & zooming actors.
 *
 * The simplest usage of #ClutterZoomAction consists in adding it to
 * a #ClutterActor and setting it as reactive; for instance, the following
 * code:
 *
 * |[
 *   clutter_actor_add_action (actor, clutter_zoom_action_new ());
 *   clutter_actor_set_reactive (actor, TRUE);
 * ]|
 *
 * will automatically result in the actor moving to follow the pointer
 * whenever the pointer's button is pressed over the actor and moved
 * across the stage.
 *
 * The #ClutterZoomAction will signal the begin and the end of a dragging
 * through the #ClutterZoomAction::drag-begin and #ClutterZoomAction::drag-end
 * signals, respectively. Each pointer motion during a drag will also result
 * in the #ClutterZoomAction::drag-motion signal to be emitted.
 *
 * It is also possible to set another #ClutterActor as the dragged actor
 * by calling clutter_zoom_action_set_zoom_handle() from within a handle
 * of the #ClutterZoomAction::drag-begin signal. The drag handle must be
 * parented and exist between the emission of #ClutterZoomAction::drag-begin
 * and #ClutterZoomAction::drag-end.
 *
 * <example id="zoom-action-example">
 *   <title>A simple draggable actor</title>
 *   <programlisting>
 * <xi:include xmlns:xi="http://www.w3.org/2001/XInclude" parse="text" href="../../../../tests/interactive/test-drag.c">
 *   <xi:fallback>FIXME: MISSING XINCLUDE CONTENT</xi:fallback>
 * </xi:include>
 *   </programlisting>
 *   <para>The example program above allows dragging the rectangle around
 *   the stage using a #ClutterZoomAction. When pressing the
 *   <keycap>Shift</keycap> key the actor that is going to be dragged is a
 *   separate rectangle, and when the drag ends, the original rectangle will
 *   be animated to the final coordinates.</para>
 * </example>
 *
 * #ClutterZoomAction is available since Clutter 1.4
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>

#include "clutter-zoom-action.h"

#include "clutter-debug.h"
#include "clutter-enum-types.h"
#include "clutter-marshal.h"
#include "clutter-private.h"
#include "clutter-stage-private.h"

typedef struct
{
  gfloat start_x;
  gfloat start_y;
  gfloat transformed_start_x;
  gfloat transformed_start_y;

  gfloat update_x;
  gfloat update_y;
  gfloat transformed_update_x;
  gfloat transformed_update_y;
} ZoomPoint;

struct _ClutterZoomActionPrivate
{
  ClutterStage *stage;

  ClutterDragAxis zoom_axis;

  ZoomPoint points[2];

  gfloat initial_x;
  gfloat initial_y;

  gdouble initial_scale_x;
  gdouble initial_scale_y;

  gdouble zoom_initial_distance;
};

enum
{
  PROP_0,

  PROP_ZOOM_AXIS,

  PROP_LAST
};

static GParamSpec *drag_props[PROP_LAST] = { NULL, };

enum
{
  ZOOM,

  LAST_SIGNAL
};

static guint zoom_signals[LAST_SIGNAL] = { 0, };

G_DEFINE_TYPE (ClutterZoomAction, clutter_zoom_action, CLUTTER_TYPE_GESTURE_ACTION);

static void
capture_point_initial_position (ClutterGestureAction *action,
                                ClutterActor         *actor,
                                gint                  device,
                                ZoomPoint            *point)
{
  clutter_gesture_action_get_motion_coords (action, device,
                                            &point->start_x,
                                            &point->start_y);

  point->transformed_start_x = point->update_x = point->start_x;
  point->transformed_start_y = point->update_x = point->start_y;
  clutter_actor_transform_stage_point (actor,
                                       point->start_x, point->start_y,
                                       &point->transformed_start_x,
                                       &point->transformed_start_y);
  point->transformed_update_x = point->transformed_start_x;
  point->transformed_update_y = point->transformed_start_y;
}

static void
capture_point_update_position (ClutterGestureAction *action,
                               ClutterActor         *actor,
                               gint                  device,
                               ZoomPoint            *point)
{
  clutter_gesture_action_get_motion_coords (action, device,
                                            &point->update_x,
                                            &point->update_y);

  point->transformed_update_x = point->update_x;
  point->transformed_update_y = point->update_y;
  clutter_actor_transform_stage_point (actor,
                                       point->update_x, point->update_y,
                                       &point->transformed_update_x,
                                       &point->transformed_update_y);
}

static gboolean
clutter_zoom_action_gesture_begin (ClutterGestureAction *action,
                                   ClutterActor         *actor)
{
  ClutterZoomActionPrivate *priv = ((ClutterZoomAction *) action)->priv;
  gfloat dx, dy;

  capture_point_initial_position (action, actor, 0, &priv->points[0]);
  capture_point_initial_position (action, actor, 1, &priv->points[1]);

  dx = priv->points[1].transformed_start_x - priv->points[0].transformed_start_x;
  dy = priv->points[1].transformed_start_y - priv->points[0].transformed_start_y;
  priv->zoom_initial_distance = sqrt (dx * dx + dy * dy);

  clutter_actor_get_position (actor,
                              &priv->initial_x,
                              &priv->initial_y);
  clutter_actor_get_scale (actor,
                           &priv->initial_scale_x,
                           &priv->initial_scale_y);

  return TRUE;
}

static gboolean
clutter_zoom_action_gesture_progress (ClutterGestureAction *action,
                                      ClutterActor         *actor)
{
  ClutterZoomActionPrivate *priv = ((ClutterZoomAction *) action)->priv;
  ClutterActor *parent = clutter_actor_get_parent (actor);
  gdouble distance, new_scale, scale_x, scale_y;
  gfloat dx, dy, center_x, center_y, trans_x, trans_y;
  ClutterVertex out, in;

  capture_point_update_position (action, actor, 0, &priv->points[0]);
  capture_point_update_position (action, actor, 1, &priv->points[1]);

  dx = priv->points[1].update_x - priv->points[0].update_x;
  dy = priv->points[1].update_y - priv->points[0].update_y;
  distance = sqrt (dx * dx + dy * dy);

  if (distance == 0)
    return TRUE;

  new_scale = distance / priv->zoom_initial_distance;
  clutter_actor_get_scale (actor, &scale_x, &scale_y);

  switch (priv->zoom_axis)
    {
    case CLUTTER_DRAG_AXIS_NONE:
      clutter_actor_set_scale (actor, new_scale, new_scale);
      break;

    case CLUTTER_DRAG_X_AXIS:
      clutter_actor_set_scale (actor, new_scale, scale_y);
      break;

    case CLUTTER_DRAG_Y_AXIS:
      clutter_actor_set_scale (actor, scale_x, new_scale);
      break;

    default:
      break;
    }

  center_x = (priv->points[0].update_x + priv->points[1].update_x) / 2;
  center_y = (priv->points[0].update_y + priv->points[1].update_y) / 2;
  trans_x = (priv->points[0].transformed_update_x +
             priv->points[1].transformed_update_x) / 2;
  trans_y = (priv->points[0].transformed_update_y +
             priv->points[1].transformed_update_y) / 2;

  in.x = trans_x;
  in.y = trans_y;
  in.z = 0;

  clutter_actor_apply_relative_transform_to_point (actor,
                                                   parent,
                                                   &in, &out);

  clutter_actor_move_by (actor, center_x - out.x, center_y - out.y);

  return TRUE;
}

static void
clutter_zoom_action_gesture_cancel (ClutterGestureAction *action,
                                    ClutterActor         *actor)
{
  ClutterZoomActionPrivate *priv = ((ClutterZoomAction *) action)->priv;

  clutter_actor_set_position (actor, priv->initial_x, priv->initial_y);
  clutter_actor_set_scale (actor, priv->initial_scale_x, priv->initial_scale_y);
}

static void
clutter_zoom_action_set_property (GObject      *gobject,
                                  guint         prop_id,
                                  const GValue *value,
                                  GParamSpec   *pspec)
{
  ClutterZoomAction *action = CLUTTER_ZOOM_ACTION (gobject);

  switch (prop_id)
    {
    case PROP_ZOOM_AXIS:
      clutter_zoom_action_set_zoom_axis (action, g_value_get_enum (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
clutter_zoom_action_get_property (GObject    *gobject,
                                  guint       prop_id,
                                  GValue     *value,
                                  GParamSpec *pspec)
{
  ClutterZoomActionPrivate *priv = CLUTTER_ZOOM_ACTION (gobject)->priv;

  switch (prop_id)
    {
    case PROP_ZOOM_AXIS:
      g_value_set_enum (value, priv->zoom_axis);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
    }
}

static void
clutter_zoom_action_dispose (GObject *gobject)
{
  G_OBJECT_CLASS (clutter_zoom_action_parent_class)->dispose (gobject);
}

static void
clutter_zoom_action_class_init (ClutterZoomActionClass *klass)
{
  ClutterGestureActionClass *gesture_class =
    CLUTTER_GESTURE_ACTION_CLASS (klass);
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (ClutterZoomActionPrivate));


  gobject_class->set_property = clutter_zoom_action_set_property;
  gobject_class->get_property = clutter_zoom_action_get_property;
  gobject_class->dispose = clutter_zoom_action_dispose;

  gesture_class->gesture_begin = clutter_zoom_action_gesture_begin;
  gesture_class->gesture_progress = clutter_zoom_action_gesture_progress;
  gesture_class->gesture_cancel = clutter_zoom_action_gesture_cancel;

  /* klass->zoom = clutter_zoom_action_real_zoom; */

  /**
   * ClutterZoomAction:zoom-axis:
   *
   * Constraints the zooming action to the specified axis
   *
   * Since: 1.12
   */
  drag_props[PROP_ZOOM_AXIS] =
    g_param_spec_enum ("zoom-axis",
                       P_("Zoom Axis"),
                       P_("Constraints the zoom to an axis"),
                       CLUTTER_TYPE_DRAG_AXIS,
                       CLUTTER_DRAG_AXIS_NONE,
                       CLUTTER_PARAM_READWRITE);

  g_object_class_install_properties  (gobject_class,
                                      PROP_LAST,
                                      drag_props);

  /**
   * ClutterZoomAction::zoom-update:
   * @action: the #ClutterZoomAction that emitted the signal
   * @actor: the #ClutterActor attached to the action
   * @distance: the initial distance between the 2 touch points
   *
   * The ::zoom-update signal is emitted for each touch event after
   * the #ClutterZoomAction::zoom-begin signal has been emitted.
   *
   * The components of the distance between the touch begin event and
   * the latest touch update event are computed in the actor's
   * coordinate space, to take into account eventual transformations.
   * If you want the stage coordinates of the latest motion event you
   * can use clutter_zoom_action_get_motion_coords().
   *
   * The default handler of the signal will call
   * clutter_actor_set_scale() either on @actor or, if set, of
   * #ClutterZoomAction:drag-handle using the ratio of the first
   * distance between the 2 touch points and the current distance. If
   * you want to override the default behaviour, you can connect to
   * this signal and call g_signal_stop_emission_by_name() from within
   * your callback.
   *
   * Since: 1.12
   */
  zoom_signals[ZOOM] =
    g_signal_new (I_("zoom"),
                  CLUTTER_TYPE_ZOOM_ACTION,
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (ClutterZoomActionClass, zoom),
                  NULL, NULL,
                  _clutter_marshal_VOID__OBJECT_DOUBLE,
                  G_TYPE_NONE, 2,
                  CLUTTER_TYPE_ACTOR,
                  G_TYPE_DOUBLE);
}

static void
clutter_zoom_action_init (ClutterZoomAction *self)
{
  self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, CLUTTER_TYPE_ZOOM_ACTION,
                                            ClutterZoomActionPrivate);

  clutter_gesture_action_set_n_touch_points (CLUTTER_GESTURE_ACTION (self), 2);
}

/**
 * clutter_zoom_action_new:
 *
 * Creates a new #ClutterZoomAction instance
 *
 * Return value: the newly created #ClutterZoomAction
 *
 * Since: 1.12
 */
ClutterAction *
clutter_zoom_action_new (void)
{
  return g_object_new (CLUTTER_TYPE_ZOOM_ACTION, NULL);
}

/**
 * clutter_zoom_action_set_zoom_axis:
 * @action: a #ClutterZoomAction
 * @axis: the axis to constraint the zooming to
 *
 * Restricts the zooming action to a specific axis
 *
 * Since: 1.12
 */
void
clutter_zoom_action_set_zoom_axis (ClutterZoomAction *action,
                                   ClutterDragAxis    axis)
{
  g_return_if_fail (CLUTTER_IS_ZOOM_ACTION (action));
  g_return_if_fail (axis >= CLUTTER_DRAG_AXIS_NONE &&
                    axis <= CLUTTER_DRAG_Y_AXIS);

  action->priv->zoom_axis = axis;
}

/**
 * clutter_zoom_action_get_zoom_axis:
 * @action: a #ClutterZoomAction
 *
 * Retrieves the axis constraint set by clutter_zoom_action_set_zoom_axis()
 *
 * Return value: the axis constraint
 *
 * Since: 1.12
 */
ClutterDragAxis
clutter_zoom_action_get_zoom_axis (ClutterZoomAction *action)
{
  g_return_val_if_fail (CLUTTER_IS_ZOOM_ACTION (action), CLUTTER_DRAG_AXIS_NONE);

  return action->priv->zoom_axis;
}
