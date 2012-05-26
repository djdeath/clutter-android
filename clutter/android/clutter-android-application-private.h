/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2012 Intel Corporation.
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
 *   Lionel Landwerlin <lionel.g.landwerlin@linux.intel.com>
 */

#ifndef __CLUTTER_ANDROID_APPLICATION_PRIVATE_H__
#define __CLUTTER_ANDROID_APPLICATION_PRIVATE_H__

#include "android_native_app_glue.h"

#include "clutter-android-application.h"

struct _ClutterAndroidApplication
{
  GObject parent;

  struct android_app* android_application;

  gint have_window : 1;
  GMainLoop *wait_for_window;
};

ClutterAndroidApplication *_clutter_android_application_get_default (void);

#endif /* __CLUTTER_ANDROID_APPLICATION_PRIVATE_H__ */
