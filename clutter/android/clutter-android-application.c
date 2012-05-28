/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2011 Intel Corporation.
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
 *   Damien Lespiau <damien.lespiau@intel.com>
 */

#include <stdlib.h>
#include <config.h>

#include <android/input.h>
#include <android/window.h>

#include <cogl/cogl.h>
#include <glib-android/glib-android.h>

#include "clutter-main.h"
#include "clutter-marshal.h"
#include "clutter-private.h"
#include "clutter-device-manager-private.h"
#include "clutter-stage-private.h"

#include "clutter-android-application-private.h"
#include "clutter-android-keycodes.h"
#include "clutter-event-android.h"
#include "clutter-stage-android.h"

#include "android_native_app_glue.h"
#include "android_jni_utils.h"

G_DEFINE_TYPE (ClutterAndroidApplication,
               clutter_android_application,
               G_TYPE_OBJECT)

enum
{
  READY,

  LAST_SIGNAL,
};

static guint signals[LAST_SIGNAL] = { 0, };

static gboolean
clutter_android_application_ready (ClutterAndroidApplication *application)
{
  g_message ("ready! %p", application->android_application->window);
  cogl_android_set_native_window (application->android_application->window);

  return TRUE;
}

static void
clutter_android_application_finalize (GObject *object)
{
  G_OBJECT_CLASS (clutter_android_application_parent_class)->finalize (object);
}

static void
clutter_android_application_class_init (ClutterAndroidApplicationClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = clutter_android_application_finalize;

  klass->ready = clutter_android_application_ready;

  signals[READY] =
    g_signal_new (I_("ready"),
                  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (ClutterAndroidApplicationClass, ready),
                  NULL, NULL,
                  _clutter_marshal_BOOLEAN__VOID,
                  G_TYPE_BOOLEAN, 0);
}

static void
clutter_android_application_init (ClutterAndroidApplication *self)
{
}

ClutterAndroidApplication *
_clutter_android_application_get_default (void)
{
  static ClutterAndroidApplication *app = NULL;

  if (G_LIKELY (app != NULL))
    return app;

  return (app = g_object_new (CLUTTER_TYPE_ANDROID_APPLICATION, NULL));
}


/*
 * Process the next main command.
 */
static void
clutter_android_handle_cmd (struct android_app *app,
                            int32_t             cmd)
{
  ClutterAndroidApplication *application;

  application = CLUTTER_ANDROID_APPLICATION (app->userData);

  switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
      /* The window is being shown, get it ready */
      g_message ("command: INIT_WINDOW");
      if (app->window != NULL)
        {
          gboolean initialized;

          /* Remove the fullscreen we ask at activity creation to be
             able to use it later if needed. */
          ANativeActivity_setWindowFlags (application->android_application->activity,
                                          0, AWINDOW_FLAG_FULLSCREEN);

          g_signal_emit (application, signals[READY], 0, &initialized);

          if (initialized)
            application->have_window = TRUE;

          if (application->wait_for_window)
            {
              g_message ("Waking up the waiting main loop");
              g_main_loop_quit (application->wait_for_window);
            }
        }
      break;

    case APP_CMD_TERM_WINDOW:
      /* The window is being hidden or closed, clean it up */
      g_message ("command: TERM_WINDOW");
      if (application->wait_for_window)
        g_main_loop_quit (application->wait_for_window);
      else
        clutter_main_quit ();
      exit (0);
      //test_fini (data);
      break;

    case APP_CMD_WINDOW_RESIZED:
      g_message ("command: window resized!");
      if (app->window != NULL)
        {
          int32_t width = ANativeWindow_getWidth (app->window);
          int32_t height = ANativeWindow_getHeight (app->window);
          ClutterStage *stage = clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());

          g_message ("resizing stage @ %ix%i", width, height);
          clutter_actor_set_size (CLUTTER_ACTOR (stage), width, height);
        }
      break;

    case APP_CMD_WINDOW_REDRAW_NEEDED:
      g_message ("command: REDRAW_NEEDED");
      if (app->window != NULL)
        {
          int32_t width = ANativeWindow_getWidth (app->window);
          int32_t height = ANativeWindow_getHeight (app->window);
          ClutterStage *stage = clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());
          ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (_clutter_stage_get_window (stage));

          g_message ("stage size %fx%f",
                     clutter_actor_get_width (CLUTTER_ACTOR (stage)),
                     clutter_actor_get_height (CLUTTER_ACTOR (stage)));
          if (clutter_actor_get_width (CLUTTER_ACTOR (stage)) != width ||
              clutter_actor_get_height (CLUTTER_ACTOR (stage)) != height)
            {
              g_message ("resizing stage @ %ix%i", width, height);
              cogl_android_onscreen_update_size (stage_cogl->onscreen,
                                                 width, height);
              clutter_actor_queue_relayout (CLUTTER_ACTOR (stage));
              /* clutter_actor_set_size (CLUTTER_ACTOR (stage), width, height); */
            }
        }
      break;

    case APP_CMD_CONTENT_RECT_CHANGED:
      g_message ("command: CONTENT_RECT_CHANGED");
      break;

    case APP_CMD_GAINED_FOCUS:
      g_message ("command: GAINED_FOCUS");
      break;

    case APP_CMD_LOST_FOCUS:
      /* When our app loses focus, we stop monitoring the accelerometer.
       * This is to avoid consuming battery while not being used. */
      g_message ("command: LOST_FOCUS");
      break;

    case APP_CMD_START:
      g_message ("command: START");
      break;

    case APP_CMD_STOP:
      g_message ("command: STOP");
      break;

    case APP_CMD_PAUSE:
      g_message ("command: PAUSE");
      break;

    case APP_CMD_DESTROY:
      g_message ("command: PAUSE");
      break;
    }
}

static ClutterEvent *
translate_motion_event (AInputEvent *a_event)
{
  int32_t action;
  ClutterEvent *event;
  ClutterDeviceManager *manager;
  ClutterInputDevice *pointer_device;

  manager = clutter_device_manager_get_default ();
  pointer_device =
    clutter_device_manager_get_core_device (manager,
                                            CLUTTER_POINTER_DEVICE);

  action = AMotionEvent_getAction (a_event);

  switch (action & AMOTION_EVENT_ACTION_MASK)
    {
    case AMOTION_EVENT_ACTION_DOWN:
      event = clutter_event_new (CLUTTER_BUTTON_PRESS);
      event->button.button = 1;
      event->button.click_count = 1;
      event->button.device = pointer_device;
      event->button.time = AMotionEvent_getEventTime (a_event);
      event->button.x = AMotionEvent_getX (a_event, 0);
      event->button.y = AMotionEvent_getY (a_event, 0);
      break;

    case AMOTION_EVENT_ACTION_UP:
      event = clutter_event_new (CLUTTER_BUTTON_RELEASE);
      event->button.button = 1;
      event->button.click_count = 1;
      event->button.device = pointer_device;
      event->button.time = AMotionEvent_getEventTime (a_event);
      event->button.x = AMotionEvent_getX (a_event, 0);
      event->button.y = AMotionEvent_getY (a_event, 0);
      break;

    case AMOTION_EVENT_ACTION_MOVE:
      event = clutter_event_new (CLUTTER_MOTION);
      event->motion.device = pointer_device;
       /* TODO: Following line is a massive hack for touch screen */
      event->motion.modifier_state = CLUTTER_BUTTON1_MASK;
      event->motion.time = AMotionEvent_getEventTime (a_event);
      event->motion.x = AMotionEvent_getX (a_event, 0);
      event->motion.y = AMotionEvent_getY (a_event, 0);
      break;

    default:
      g_message ("\tmeh? %i\n", action);
      return NULL;
    }

  event->any.stage =
    clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());
  _clutter_input_device_set_stage (pointer_device, event->any.stage);

  return event;
}

static ClutterEvent *
translate_key_event (AInputEvent *a_event)
{
  int32_t action;
  ClutterEvent *event;
  ClutterDeviceManager *manager;
  ClutterInputDevice *keyboard_device;
  ClutterAndroidApplication *application =
    _clutter_android_application_get_default ();
  int32_t new_modifier_state = application->modifier_state;

  action = AKeyEvent_getAction (a_event);

  manager = clutter_device_manager_get_default ();
  keyboard_device =
    clutter_device_manager_get_core_device (manager,
                                            CLUTTER_KEYBOARD_DEVICE);

  g_message ("\taction = %i flags = %x meta = %x keycode = %i",
             AKeyEvent_getAction (a_event),
             AKeyEvent_getFlags (a_event),
             AKeyEvent_getMetaState (a_event),
             AKeyEvent_getKeyCode (a_event));

  switch (action)
    {
    case AKEY_EVENT_ACTION_DOWN:
      g_message ("\tkey press");
      event = clutter_event_new (CLUTTER_KEY_PRESS);
      new_modifier_state |= AKeyEvent_getMetaState (a_event);
      break;

    case AKEY_EVENT_ACTION_UP:
      g_message ("\tkey release");
      event = clutter_event_new (CLUTTER_KEY_RELEASE);
      new_modifier_state &= ~AKeyEvent_getMetaState (a_event);
      break;

    case AKEY_EVENT_ACTION_MULTIPLE:
      g_message ("\tcomplex string");
      return NULL;

    default:
      g_message ("\tmeh? %i", action);
      return NULL;
    }

  _clutter_android_translate_key_event ((ClutterKeyEvent *) event,
                                        application->modifier_state,
                                        a_event);

  event->key.device = keyboard_device;
  event->any.stage =
    clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());
  _clutter_input_device_set_stage (keyboard_device, event->any.stage);

  application->modifier_state = new_modifier_state;

  return event;
}

/**
 * Process the next input event
 */
static int32_t
clutter_android_handle_input (struct android_app *app,
                              AInputEvent        *a_event)
{
  ClutterEvent *event = NULL;

  g_message ("input!");

  if (AInputEvent_getType (a_event) == AINPUT_EVENT_TYPE_KEY)
    {
      event = translate_key_event (a_event);
    }
  else if (AInputEvent_getType (a_event) == AINPUT_EVENT_TYPE_MOTION)
    {
      event = translate_motion_event (a_event);
    }

  if (event != NULL)
    {
      ClutterBackendAndroid *backend = (ClutterBackendAndroid *) clutter_get_default_backend ();

      _clutter_event_source_android_push_event (backend->android_source, event);

      return TRUE;
    }

  return FALSE;
}

void
clutter_android_application_run (ClutterAndroidApplication *application)
{
  g_return_if_fail (CLUTTER_IS_ANDROID_APPLICATION (application));

  /* XXX: eeew. We wait to have a window to initialize Clutter and
   * thus to enter the clutter main loop */
  if (!application->have_window)
    {
      g_message ("Waiting for the window");
      application->wait_for_window = g_main_loop_new (NULL, FALSE);
      g_main_loop_run (application->wait_for_window);
      g_main_loop_unref (application->wait_for_window);
      application->wait_for_window = NULL;
    }

  g_message ("entering main loop");
  clutter_main ();
}

AAssetManager *
clutter_android_application_get_asset_manager (ClutterAndroidApplication *application)
{
  g_return_val_if_fail (CLUTTER_IS_ANDROID_APPLICATION (application), NULL);

  return application->android_application->activity->assetManager;
}

void
clutter_android_application_show_keyboard (ClutterAndroidApplication *application,
                                           gboolean show_keyboard,
                                           gboolean implicit)
{
  jint ret;

  g_return_if_fail (CLUTTER_IS_ANDROID_APPLICATION (application));

  if (show_keyboard)
    {
      g_message ("showing keyboard");
      if (implicit)
        ret = _android_show_keyboard (application->android_application,
                                      JNI_TRUE,
                                      ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT);
      else
        ret = _android_show_keyboard (application->android_application,
                                      JNI_TRUE,
                                      ANATIVEACTIVITY_SHOW_SOFT_INPUT_IMPLICIT);
    }
  else
    {
      g_message ("hiding keyboard");
      if (implicit)
        ret = _android_show_keyboard (application->android_application,
                                      JNI_FALSE,
                                      ANATIVEACTIVITY_HIDE_SOFT_INPUT_IMPLICIT_ONLY);
      else
        ret = _android_show_keyboard (application->android_application,
                                      JNI_FALSE,
                                      ANATIVEACTIVITY_HIDE_SOFT_INPUT_NOT_ALWAYS);
    }

  g_message ("THE FucK %i", ret);
}

/*
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void
android_main (struct android_app* android_application)
{
  ClutterAndroidApplication *clutter_application;

  /* If we don't ask for the fullscreen flag on activity creation,
     using this API later kills the app... WHY?? WHYYYYY??? */
  if (android_application->activity)
    ANativeActivity_setWindowFlags (android_application->activity,
                                    AWINDOW_FLAG_FULLSCREEN, 0);

  /* Make sure glue isn't stripped */
  app_dummy ();

  g_type_init ();
  g_android_init ();

  clutter_application = _clutter_android_application_get_default ();

  android_application->userData = clutter_application;
  android_application->onAppCmd = clutter_android_handle_cmd;
  android_application->onInputEvent = clutter_android_handle_input;

  clutter_application->android_application = android_application;

  clutter_android_main (clutter_application);
}
