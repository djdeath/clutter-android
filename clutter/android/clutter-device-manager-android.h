/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2011 Intel Corp.
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
 * Author: Damien Lespiau <damien.lespiau@intel.com>
 */

#ifndef __CLUTTER_DEVICE_MANAGER_ANDROID_H__
#define __CLUTTER_DEVICE_MANAGER_ANDROID_H__

#include <glib-object.h>

#include <clutter/clutter-device-manager.h>

G_BEGIN_DECLS

#define CLUTTER_TYPE_DEVICE_MANAGER_ANDROID clutter_device_manager_android_get_type()

#define CLUTTER_DEVICE_MANAGER_ANDROID(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
  CLUTTER_TYPE_DEVICE_MANAGER_ANDROID, ClutterDeviceManagerAndroid))

#define CLUTTER_DEVICE_MANAGER_ANDROID_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
  CLUTTER_TYPE_DEVICE_MANAGER_ANDROID, ClutterDeviceManagerAndroidClass))

#define CLUTTER_IS_DEVICE_MANAGER_ANDROID(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
  CLUTTER_TYPE_DEVICE_MANAGER_ANDROID))

#define CLUTTER_IS_DEVICE_MANAGER_ANDROID_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
  CLUTTER_TYPE_DEVICE_MANAGER_ANDROID))

#define CLUTTER_DEVICE_MANAGER_ANDROID_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
  CLUTTER_TYPE_DEVICE_MANAGER_ANDROID, ClutterDeviceManagerAndroidClass))

typedef struct _ClutterDeviceManagerAndroid ClutterDeviceManagerAndroid;
typedef struct _ClutterDeviceManagerAndroidClass ClutterDeviceManagerAndroidClass;
typedef struct _ClutterDeviceManagerAndroidPrivate ClutterDeviceManagerAndroidPrivate;

struct _ClutterDeviceManagerAndroid
{
  ClutterDeviceManager parent;

  ClutterDeviceManagerAndroidPrivate *priv;
};

struct _ClutterDeviceManagerAndroidClass
{
  ClutterDeviceManagerClass parent_class;
};

GType clutter_device_manager_android_get_type (void) G_GNUC_CONST;

ClutterDeviceManagerAndroid * clutter_device_manager_android_new  (void);

void			      _clutter_events_android_init	  (ClutterBackend *backend);
void			      _clutter_events_android_uninit	  (ClutterBackend *backend);
G_END_DECLS

#endif /* __CLUTTER_DEVICE_MANAGER_ANDROID_H__ */
