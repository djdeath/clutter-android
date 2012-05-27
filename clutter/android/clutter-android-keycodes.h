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

#ifndef __CLUTTER_ANDROID_KEYCODES_H__
#define __CLUTTER_ANDROID_KEYCODES_H__

#include <clutter/clutter-event.h>

#include <android/input.h>

void _clutter_android_translate_key_event (ClutterKeyEvent *event,
                                           int32_t modifier_state,
                                           AInputEvent *a_event);

#endif /* __CLUTTER_ANDROID_KEYCODES_H__ */
