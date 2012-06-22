/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Authored By Matthew Allum  <mallum@openedhand.com>
 *
 * Copyright (C) 2006 OpenedHand
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
 */

#if !defined(__CLUTTER_H_INSIDE__) && !defined(CLUTTER_COMPILATION)
#error "Only <clutter/clutter.h> can be included directly."
#endif

#ifndef __CLUTTER_TYPES_H__
#define __CLUTTER_TYPES_H__

#include <cairo.h>

#include <clutter/clutter-macros.h>
#include <clutter/clutter-enums.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_ACTOR_BOX          (clutter_actor_box_get_type ())
#define CLUTTER_TYPE_FOG                (clutter_fog_get_type ())
#define CLUTTER_TYPE_GEOMETRY           (clutter_geometry_get_type ())
#define CLUTTER_TYPE_KNOT               (clutter_knot_get_type ())
#define CLUTTER_TYPE_MARGIN             (clutter_margin_get_type ())
#define CLUTTER_TYPE_PAINT_VOLUME       (clutter_paint_volume_get_type ())
#define CLUTTER_TYPE_PERSPECTIVE        (clutter_perspective_get_type ())
#define CLUTTER_TYPE_VERTEX             (clutter_vertex_get_type ())

typedef struct _ClutterActor            ClutterActor;

typedef struct _ClutterStage            ClutterStage;
typedef struct _ClutterContainer        ClutterContainer; /* dummy */
typedef struct _ClutterChildMeta        ClutterChildMeta;
typedef struct _ClutterLayoutMeta       ClutterLayoutMeta;
typedef struct _ClutterActorMeta        ClutterActorMeta;
typedef struct _ClutterLayoutManager    ClutterLayoutManager;
typedef struct _ClutterActorIter        ClutterActorIter;
typedef struct _ClutterPaintNode        ClutterPaintNode;
typedef struct _ClutterContent          ClutterContent; /* dummy */

typedef struct _ClutterAlpha            ClutterAlpha;
typedef struct _ClutterAnimatable       ClutterAnimatable; /* dummy */
typedef struct _ClutterAnimator         ClutterAnimator;
typedef struct _ClutterInterval         ClutterInterval;
typedef struct _ClutterState            ClutterState;
typedef struct _ClutterTimeline         ClutterTimeline;
typedef struct _ClutterTransition       ClutterTransition;
typedef struct _ClutterPropertyTransition       ClutterPropertyTransition;

typedef struct _ClutterAction           ClutterAction;
typedef struct _ClutterConstraint       ClutterConstraint;
typedef struct _ClutterEffect           ClutterEffect;

typedef struct _ClutterPath             ClutterPath;

typedef struct _ClutterActorBox         ClutterActorBox;
typedef struct _ClutterColor            ClutterColor;
typedef struct _ClutterFog              ClutterFog;
typedef struct _ClutterGeometry         ClutterGeometry;
typedef struct _ClutterKnot             ClutterKnot;
typedef struct _ClutterMargin           ClutterMargin;
typedef struct _ClutterPerspective      ClutterPerspective;
typedef struct _ClutterVertex           ClutterVertex;

typedef struct _ClutterBehaviour        ClutterBehaviour;
typedef struct _ClutterShader           ClutterShader;

typedef struct _ClutterInputDevice      ClutterInputDevice;

typedef union _ClutterEvent             ClutterEvent;
typedef struct _ClutterEventSequence    ClutterEventSequence;


/**
 * ClutterPaintVolume:
 *
 * <structname>ClutterPaintVolume</structname> is an opaque structure
 * whose members cannot be directly accessed.
 *
 * A <structname>ClutterPaintVolume</structname> represents an
 * a bounding volume whose internal representation isn't defined but
 * can be set and queried in terms of an axis aligned bounding box.
 *
 * A <structname>ClutterPaintVolume</structname> for a #ClutterActor
 * is defined to be relative from the current actor modelview matrix.
 *
 * Other internal representation and methods for describing the
 * bounding volume may be added in the future.
 *
 * Since: 1.4
 */
typedef struct _ClutterPaintVolume      ClutterPaintVolume;

/**
 * ClutterVertex:
 * @x: X coordinate of the vertex
 * @y: Y coordinate of the vertex
 * @z: Z coordinate of the vertex
 *
 * Vertex of an actor in 3D space, expressed in pixels
 *
 * Since: 0.4
 */
struct _ClutterVertex
{
  gfloat x;
  gfloat y;
  gfloat z;
};

/**
 * CLUTTER_VERTEX_INIT:
 * @x: the X coordinate of the vertex
 * @y: the Y coordinate of the vertex
 * @z: the Z coordinate of the vertex
 *
 * A simple macro for initializing a #ClutterVertex when declaring it, e.g.:
 *
 * |[
 *   ClutterVertext v = CLUTTER_VERTEX_INIT (x, y, z);
 * ]|
 *
 * Since: 1.10
 */
#define CLUTTER_VERTEX_INIT(x,y,z)      { (x), (y), (z) }

GType          clutter_vertex_get_type (void) G_GNUC_CONST;
ClutterVertex *clutter_vertex_new      (gfloat               x,
                                        gfloat               y,
                                        gfloat               z);
void           clutter_vertex_init     (ClutterVertex       *vertex,
                                        gfloat               x,
                                        gfloat               y,
                                        gfloat               z);
ClutterVertex *clutter_vertex_copy     (const ClutterVertex *vertex);
void           clutter_vertex_free     (ClutterVertex       *vertex);
gboolean       clutter_vertex_equal    (const ClutterVertex *vertex_a,
                                        const ClutterVertex *vertex_b);

/**
 * ClutterActorBox:
 * @x1: X coordinate of the top left corner
 * @y1: Y coordinate of the top left corner
 * @x2: X coordinate of the bottom right corner
 * @y2: Y coordinate of the bottom right corner
 *
 * Bounding box of an actor. The coordinates of the top left and right bottom
 * corners of an actor. The coordinates of the two points are expressed in
 * pixels with sub-pixel precision
 */
struct _ClutterActorBox
{
  gfloat x1;
  gfloat y1;

  gfloat x2;
  gfloat y2;
};

/**
 * CLUTTER_ACTOR_BOX_INIT:
 * @x_1: the X coordinate of the top left corner
 * @y_1: the Y coordinate of the top left corner
 * @x_2: the X coordinate of the bottom right corner
 * @y_2: the Y coordinate of the bottom right corner
 *
 * A simple macro for initializing a #ClutterActorBox when declaring
 * it, e.g.:
 *
 * |[
 *   ClutterActorBox box = CLUTTER_ACTOR_BOX_INIT (0, 0, 400, 600);
 * ]|
 *
 * Since: 1.10
 */
#define CLUTTER_ACTOR_BOX_INIT(x_1,y_1,x_2,y_2)         { (x_1), (y_1), (x_2), (y_2) }

GType            clutter_actor_box_get_type      (void) G_GNUC_CONST;
ClutterActorBox *clutter_actor_box_new           (gfloat                 x_1,
                                                  gfloat                 y_1,
                                                  gfloat                 x_2,
                                                  gfloat                 y_2);
void             clutter_actor_box_init          (ClutterActorBox       *box,
                                                  gfloat                 x_1,
                                                  gfloat                 y_1,
                                                  gfloat                 x_2,
                                                  gfloat                 y_2);
void             clutter_actor_box_init_rect     (ClutterActorBox       *box,
                                                  gfloat                 x,
                                                  gfloat                 y,
                                                  gfloat                 width,
                                                  gfloat                 height);
ClutterActorBox *clutter_actor_box_copy          (const ClutterActorBox *box);
void             clutter_actor_box_free          (ClutterActorBox       *box);
gboolean         clutter_actor_box_equal         (const ClutterActorBox *box_a,
                                                  const ClutterActorBox *box_b);
gfloat           clutter_actor_box_get_x         (const ClutterActorBox *box);
gfloat           clutter_actor_box_get_y         (const ClutterActorBox *box);
gfloat           clutter_actor_box_get_width     (const ClutterActorBox *box);
gfloat           clutter_actor_box_get_height    (const ClutterActorBox *box);
void             clutter_actor_box_get_origin    (const ClutterActorBox *box,
                                                  gfloat                *x,
                                                  gfloat                *y);
void             clutter_actor_box_get_size      (const ClutterActorBox *box,
                                                  gfloat                *width,
                                                  gfloat                *height);
gfloat           clutter_actor_box_get_area      (const ClutterActorBox *box);
gboolean         clutter_actor_box_contains      (const ClutterActorBox *box,
                                                  gfloat                 x,
                                                  gfloat                 y);
void             clutter_actor_box_from_vertices (ClutterActorBox       *box,
                                                  const ClutterVertex    verts[]);
void             clutter_actor_box_interpolate   (const ClutterActorBox *initial,
                                                  const ClutterActorBox *final,
                                                  gdouble                progress,
                                                  ClutterActorBox       *result);
void             clutter_actor_box_clamp_to_pixel (ClutterActorBox       *box);
void             clutter_actor_box_union          (const ClutterActorBox *a,
                                                   const ClutterActorBox *b,
                                                   ClutterActorBox       *result);

void             clutter_actor_box_set_origin     (ClutterActorBox       *box,
                                                   gfloat                 x,
                                                   gfloat                 y);
void             clutter_actor_box_set_size       (ClutterActorBox       *box,
                                                   gfloat                 width,
                                                   gfloat                 height);

/**
 * ClutterGeometry:
 * @x: X coordinate of the top left corner of an actor
 * @y: Y coordinate of the top left corner of an actor
 * @width: width of an actor
 * @height: height of an actor
 *
 * The rectangle containing an actor's bounding box, measured in pixels.
 */
struct _ClutterGeometry
{
  /*< public >*/
  gint   x;
  gint   y;
  guint  width;
  guint  height;
};

GType clutter_geometry_get_type (void) G_GNUC_CONST;

void      clutter_geometry_union      (const ClutterGeometry *geometry_a,
                                       const ClutterGeometry *geometry_b,
                                       ClutterGeometry       *result);
gboolean  clutter_geometry_intersects (const ClutterGeometry *geometry0,
                                       const ClutterGeometry *geometry1);

/**
 * ClutterKnot:
 * @x: X coordinate of the knot
 * @y: Y coordinate of the knot
 *
 * Point in a path behaviour.
 *
 * Since: 0.2
 */
struct _ClutterKnot
{
  gint x;
  gint y;
};

GType        clutter_knot_get_type (void) G_GNUC_CONST;
ClutterKnot *clutter_knot_copy     (const ClutterKnot *knot);
void         clutter_knot_free     (ClutterKnot       *knot);
gboolean     clutter_knot_equal    (const ClutterKnot *knot_a,
                                    const ClutterKnot *knot_b);

GType clutter_paint_volume_get_type (void) G_GNUC_CONST;

ClutterPaintVolume *clutter_paint_volume_copy                (const ClutterPaintVolume *pv);
void                clutter_paint_volume_free                (ClutterPaintVolume       *pv);

void                clutter_paint_volume_set_origin          (ClutterPaintVolume       *pv,
                                                              const ClutterVertex      *origin);
void                clutter_paint_volume_get_origin          (const ClutterPaintVolume *pv,
                                                              ClutterVertex            *vertex);
void                clutter_paint_volume_set_width           (ClutterPaintVolume       *pv,
                                                              gfloat                    width);
gfloat              clutter_paint_volume_get_width           (const ClutterPaintVolume *pv);
void                clutter_paint_volume_set_height          (ClutterPaintVolume       *pv,
                                                              gfloat                    height);
gfloat              clutter_paint_volume_get_height          (const ClutterPaintVolume *pv);
void                clutter_paint_volume_set_depth           (ClutterPaintVolume       *pv,
                                                              gfloat                    depth);
gfloat              clutter_paint_volume_get_depth           (const ClutterPaintVolume *pv);
void                clutter_paint_volume_union               (ClutterPaintVolume       *pv,
                                                              const ClutterPaintVolume *another_pv);
CLUTTER_AVAILABLE_IN_1_10
void                clutter_paint_volume_union_box           (ClutterPaintVolume       *pv,
                                                              const ClutterActorBox    *box);

gboolean            clutter_paint_volume_set_from_allocation (ClutterPaintVolume       *pv,
                                                              ClutterActor             *actor);

/**
 * ClutterMargin:
 * @left: the margin from the left
 * @right: the margin from the right
 * @top: the margin from the top
 * @bottom: the margin from the bottom
 *
 * A representation of the components of a margin.
 *
 * Since: 1.10
 */
struct _ClutterMargin
{
  float left;
  float right;
  float top;
  float bottom;
};

GType clutter_margin_get_type (void) G_GNUC_CONST;

ClutterMargin * clutter_margin_new      (void) G_GNUC_MALLOC;
ClutterMargin * clutter_margin_copy     (const ClutterMargin *margin_);
void            clutter_margin_free     (ClutterMargin       *margin_);

/**
 * ClutterProgressFunc:
 * @a: the initial value of an interval
 * @b: the final value of an interval
 * @progress: the progress factor, between 0 and 1
 * @retval: the value used to store the progress
 *
 * Prototype of the progress function used to compute the value
 * between the two ends @a and @b of an interval depending on
 * the value of @progress.
 *
 * The #GValue in @retval is already initialized with the same
 * type as @a and @b.
 *
 * This function will be called by #ClutterInterval if the
 * type of the values of the interval was registered using
 * clutter_interval_register_progress_func().
 *
 * Return value: %TRUE if the function successfully computed
 *   the value and stored it inside @retval
 *
 * Since: 1.0
 */
typedef gboolean (* ClutterProgressFunc) (const GValue *a,
                                          const GValue *b,
                                          gdouble       progress,
                                          GValue       *retval);

G_END_DECLS

#endif /* __CLUTTER_TYPES_H__ */
