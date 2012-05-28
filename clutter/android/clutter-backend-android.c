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

#include "clutter-backend-android.h"
#include "clutter-device-manager-android.h"
#include "clutter-event-android.h"

#include "clutter-debug.h"
#include "clutter-private.h"
#include "clutter-stage-private.h"
#include "clutter-stage-android.h"

#include "cogl/cogl.h"

#define DEFAULT_FONT_NAME       "Lucida Grande 13"

#define clutter_backend_android_get_type    _clutter_backend_android_get_type

G_DEFINE_TYPE (ClutterBackendAndroid, clutter_backend_android, CLUTTER_TYPE_BACKEND)

/*************************************************************************/

void
_clutter_backend_android_events_init (ClutterBackend *backend)
{
  if (backend->device_manager != NULL)
    return;

  CLUTTER_NOTE (BACKEND, "init_events");

  backend->device_manager =
    g_object_new (CLUTTER_TYPE_DEVICE_MANAGER_ANDROID,
                  "backend", backend,
                  NULL);
}

/*************************************************************************/

static void
clutter_backend_android_init (ClutterBackendAndroid *backend_android)
{
}

static void
clutter_backend_android_dispose (GObject *object)
{
  ClutterBackendAndroid *backend_android = CLUTTER_BACKEND_ANDROID (object);

  g_source_unref (backend_android->android_source);

  G_OBJECT_CLASS (clutter_backend_android_parent_class)->dispose (object);
}

static gboolean
clutter_backend_android_post_parse (ClutterBackend  *backend,
                                    GError         **error)
{
  ClutterBackendAndroid *backend_android = CLUTTER_BACKEND_ANDROID (backend);

  backend_android->android_source = _clutter_event_source_android_new ();
  g_source_attach (backend_android->android_source, NULL);

  /* TODO: Maybe move the device manager initialization here...
     IDUNNOLOL */

  return TRUE;
}

static void
clutter_backend_android_class_init (ClutterBackendAndroidClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  ClutterBackendClass *backend_class = CLUTTER_BACKEND_CLASS (klass);

  object_class->dispose = clutter_backend_android_dispose;

  backend_class->stage_window_type = CLUTTER_TYPE_STAGE_ANDROID;

  backend_class->post_parse = clutter_backend_android_post_parse;
}
