/* Clutter -  An OpenGL based 'interactive canvas' library.
 * Android backend - initial entry point
 *
 * Copyright (C) 2012 Intel Corporation
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
 * Authors:
 *  Lionel Landwerlin <lionel.g.landwerlin@linux.intel.com>
 */

#include "config.h"

#include <math.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <cogl/cogl.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <android/window.h>

#include "clutter-backend-android.h"
#include "clutter-stage-android.h"
#include "clutter-android.h"
#include "clutter-android-application-private.h"

#include "clutter-debug.h"
#include "clutter-private.h"
#include "clutter-stage-private.h"

static void clutter_stage_window_iface_init (ClutterStageWindowIface *iface);

#define clutter_stage_android_get_type _clutter_stage_android_get_type

G_DEFINE_TYPE_WITH_CODE (ClutterStageAndroid,
                         clutter_stage_android,
                         CLUTTER_TYPE_STAGE_COGL,
                         G_IMPLEMENT_INTERFACE (CLUTTER_TYPE_STAGE_WINDOW,
                                                clutter_stage_window_iface_init));

static void
clutter_stage_android_set_fullscreen (ClutterStageWindow *stage_window,
                                      gboolean            is_fullscreen)
{
  ClutterStageAndroid *stage_android = CLUTTER_STAGE_ANDROID (stage_window);
  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (stage_android);
  ClutterStage *stage = stage_cogl->wrapper;
  ClutterAndroidApplication *application;

  if (stage == NULL || CLUTTER_ACTOR_IN_DESTRUCTION (stage))
    return;

  is_fullscreen = !!is_fullscreen;

  if (_clutter_stage_is_fullscreen (stage) == is_fullscreen)
    return;

  application = clutter_android_application_get_default ();

  if (is_fullscreen)
    {
      ANativeActivity_setWindowFlags (application->android_application->activity,
                                      AWINDOW_FLAG_FULLSCREEN,
                                      0);
      _clutter_stage_update_state (stage, 0, CLUTTER_STAGE_STATE_FULLSCREEN);
    }
  else
    {
      ANativeActivity_setWindowFlags (application->android_application->activity,
                                      0,
                                      AWINDOW_FLAG_FULLSCREEN);
      _clutter_stage_update_state (stage, CLUTTER_STAGE_STATE_FULLSCREEN, 0);
    }
}

static void
clutter_stage_android_finalize (GObject *gobject)
{
  G_OBJECT_CLASS (clutter_stage_android_parent_class)->finalize (gobject);
}

static void
clutter_stage_android_dispose (GObject *gobject)
{
  G_OBJECT_CLASS (clutter_stage_android_parent_class)->dispose (gobject);
}

static void
clutter_stage_android_class_init (ClutterStageAndroidClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->finalize = clutter_stage_android_finalize;
  gobject_class->dispose = clutter_stage_android_dispose;
}

static void
clutter_stage_android_init (ClutterStageAndroid *stage)
{
}

static void
clutter_stage_window_iface_init (ClutterStageWindowIface *iface)
{
  iface->set_fullscreen = clutter_stage_android_set_fullscreen;
}
