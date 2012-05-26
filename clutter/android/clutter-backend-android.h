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

#ifndef __CLUTTER_BACKEND_ANDROID_H__
#define __CLUTTER_BACKEND_ANDROID_H__

#include <glib-object.h>
#include <clutter/clutter-event.h>
#include <clutter/clutter-backend.h>
#include <clutter/clutter-device-manager.h>

#include "clutter-backend-private.h"

G_BEGIN_DECLS

#define CLUTTER_TYPE_BACKEND_ANDROID                (_clutter_backend_android_get_type ())
#define CLUTTER_BACKEND_ANDROID(obj)                (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_BACKEND_ANDROID, ClutterBackendAndroid))
#define CLUTTER_IS_BACKEND_ANDROID(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_BACKEND_ANDROID))
#define CLUTTER_BACKEND_ANDROID_CLASS(klass)        (G_TYPE_CHECK_CLASS_CAST ((klass), CLUTTER_TYPE_BACKEND_ANDROID, ClutterBackendAndroidClass))
#define CLUTTER_IS_BACKEND_ANDROID_CLASS(klass)     (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_BACKEND_ANDROID))
#define CLUTTER_BACKEND_ANDROID_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), CLUTTER_TYPE_BACKEND_ANDROID, ClutterBackendAndroidClass))

typedef struct _ClutterBackendAndroid       ClutterBackendAndroid;
typedef struct _ClutterBackendAndroidClass  ClutterBackendAndroidClass;

struct _ClutterBackendAndroid
{
  ClutterBackend parent_instance;

  GSource *android_source;
};

struct _ClutterBackendAndroidClass
{
  ClutterBackendClass parent_class;
};

GType _clutter_backend_android_get_type (void) G_GNUC_CONST;

void _clutter_backend_android_events_init (ClutterBackend *backend);

G_END_DECLS

#endif /* __CLUTTER_BACKEND_ANDROID_H__ */
