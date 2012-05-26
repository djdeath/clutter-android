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

#ifndef __CLUTTER_STAGE_ANDROID_H__
#define __CLUTTER_STAGE_ANDROID_H__

#include <clutter/clutter-group.h>
#include <clutter/clutter-stage.h>

#include "clutter-backend-android.h"
#include "cogl/clutter-stage-cogl.h"

G_BEGIN_DECLS

#define CLUTTER_TYPE_STAGE_ANDROID                  (_clutter_stage_android_get_type ())
#define CLUTTER_STAGE_ANDROID(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), CLUTTER_TYPE_STAGE_ANDROID, ClutterStageAndroid))
#define CLUTTER_IS_STAGE_ANDROID(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CLUTTER_TYPE_STAGE_ANDROID))
#define CLUTTER_STAGE_ANDROID_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), CLUTTER_TYPE_STAGE_ANDROID, ClutterStageAndroidClass))
#define CLUTTER_IS_STAGE_ANDROID_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), CLUTTER_TYPE_STAGE_ANDROID))
#define CLUTTER_STAGE_ANDROID_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), CLUTTER_TYPE_STAGE_ANDROID, ClutterStageAndroidClass))

typedef struct _ClutterStageAndroid         ClutterStageAndroid;
typedef struct _ClutterStageAndroidClass    ClutterStageAndroidClass;

struct _ClutterStageAndroid
{
  ClutterStageCogl parent_instance;
};

struct _ClutterStageAndroidClass
{
  ClutterStageCoglClass parent_class;
};

GType _clutter_stage_android_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* __CLUTTER_STAGE_ANDROID_H__ */
