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

#include "clutter-debug.h"
#include "clutter-device-manager-private.h"
#include "clutter-device-manager-android.h"

#include "clutter-stage-manager.h"


G_DEFINE_TYPE (ClutterDeviceManagerAndroid,
	       clutter_device_manager_android,
	       CLUTTER_TYPE_DEVICE_MANAGER)

#define DEVICE_MANAGER_ANDROID_PRIVATE(o)			      \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o),				      \
				CLUTTER_TYPE_DEVICE_MANAGER_ANDROID,  \
				ClutterDeviceManagerAndroidPrivate))

struct _ClutterDeviceManagerAndroidPrivate
{
  GSList *devices;

  ClutterInputDevice *core_pointer;
  ClutterInputDevice *core_keyboard;
};

/*
 * ClutterDeviceManager implementation
 */

static void
clutter_device_manager_android_add_device (ClutterDeviceManager *manager,
					   ClutterInputDevice   *device)
{
  ClutterDeviceManagerAndroid *manager_android;
  ClutterDeviceManagerAndroidPrivate *priv;

  manager_android = CLUTTER_DEVICE_MANAGER_ANDROID (manager);
  priv = manager_android->priv;

  priv->devices = g_slist_prepend (priv->devices, device);
}

static void
clutter_device_manager_android_remove_device (ClutterDeviceManager *manager,
                                            ClutterInputDevice   *device)
{
  ClutterDeviceManagerAndroid *manager_android;
  ClutterDeviceManagerAndroidPrivate *priv;

  manager_android = CLUTTER_DEVICE_MANAGER_ANDROID (manager);
  priv = manager_android->priv;

  priv->devices = g_slist_remove (priv->devices, device);
}

static const GSList *
clutter_device_manager_android_get_devices (ClutterDeviceManager *manager)
{
  return CLUTTER_DEVICE_MANAGER_ANDROID (manager)->priv->devices;
}

static ClutterInputDevice *
clutter_device_manager_android_get_core_device (ClutterDeviceManager   *manager,
                                              ClutterInputDeviceType  type)
{
  ClutterDeviceManagerAndroid *manager_android;
  ClutterDeviceManagerAndroidPrivate *priv;

  manager_android = CLUTTER_DEVICE_MANAGER_ANDROID (manager);
  priv = manager_android->priv;

  switch (type)
    {
    case CLUTTER_POINTER_DEVICE:
      return priv->core_pointer;

    case CLUTTER_KEYBOARD_DEVICE:
      return priv->core_keyboard;

    case CLUTTER_EXTENSION_DEVICE:
    default:
      return NULL;
    }

  return NULL;
}

static ClutterInputDevice *
clutter_device_manager_android_get_device (ClutterDeviceManager *manager,
					   gint                  id)
{
  ClutterDeviceManagerAndroid *manager_android;
  ClutterDeviceManagerAndroidPrivate *priv;
  GSList *l;

  manager_android = CLUTTER_DEVICE_MANAGER_ANDROID (manager);
  priv = manager_android->priv;

  for (l = priv->devices; l; l = l->next)
    {
      ClutterInputDevice *device = l->data;

      if (clutter_input_device_get_device_id (device) == id)
        return device;
    }

  return NULL;
}

/*
 * GObject implementation
 */

static void
clutter_device_manager_android_constructed (GObject *object)
{
  ClutterDeviceManager *manager = CLUTTER_DEVICE_MANAGER (object);
  ClutterDeviceManagerAndroid *android_manager;
  ClutterDeviceManagerAndroidPrivate *priv;
  ClutterStage *stage;

  android_manager = CLUTTER_DEVICE_MANAGER_ANDROID (object);
  priv = android_manager->priv;

  priv->core_pointer = g_object_new (CLUTTER_TYPE_INPUT_DEVICE,
				     "id", 0,
				     "name", "Core Pointer Device",
				     "device-type", CLUTTER_POINTER_DEVICE,
				      "enabled", TRUE,
				     NULL);

  /* Always associate the device to the default stage */
  stage = clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());
  _clutter_input_device_set_stage (priv->core_pointer, stage);

  _clutter_device_manager_add_device (manager, priv->core_pointer);

  CLUTTER_NOTE (EVENT, "Added Core Pointer device");

  priv->core_keyboard = g_object_new (CLUTTER_TYPE_INPUT_DEVICE,
				      "id", 1,
				      "name", "Core Keyboard Device",
				      "device-type", CLUTTER_KEYBOARD_DEVICE,
				      "enabled", TRUE,
				      NULL);

  _clutter_input_device_set_stage (priv->core_keyboard, stage);

  _clutter_device_manager_add_device (manager, priv->core_keyboard);

  CLUTTER_NOTE (EVENT, "Added Core Keyboard device");
}

static void
clutter_device_manager_android_finalize (GObject *object)
{
  G_OBJECT_CLASS (clutter_device_manager_android_parent_class)->finalize (object);
}

static void
clutter_device_manager_android_class_init (ClutterDeviceManagerAndroidClass *klass)
{
  ClutterDeviceManagerClass *manager_class;
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  g_type_class_add_private (klass, sizeof (ClutterDeviceManagerAndroidPrivate));

  object_class->finalize = clutter_device_manager_android_finalize;
  object_class->constructed = clutter_device_manager_android_constructed;

  manager_class = CLUTTER_DEVICE_MANAGER_CLASS (klass);
  manager_class->add_device = clutter_device_manager_android_add_device;
  manager_class->remove_device = clutter_device_manager_android_remove_device;
  manager_class->get_devices = clutter_device_manager_android_get_devices;
  manager_class->get_core_device = clutter_device_manager_android_get_core_device;
  manager_class->get_device = clutter_device_manager_android_get_device;
}

static void
clutter_device_manager_android_init (ClutterDeviceManagerAndroid *self)
{
  self->priv = DEVICE_MANAGER_ANDROID_PRIVATE (self);
}

ClutterDeviceManagerAndroid *
clutter_device_manager_android_new (void)
{
  return g_object_new (CLUTTER_TYPE_DEVICE_MANAGER_ANDROID, NULL);
}
