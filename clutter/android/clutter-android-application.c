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

#if 1
#define DEBUG_KEY(args...) g_message (args)
#else
#define DEBUG_KEY(args...)
#endif

#if 0
#define DEBUG_BUTTON(args...) g_message (args)
#else
#define DEBUG_BUTTON(args...)
#endif

#if 0
#define DEBUG_TOUCH(args...) g_message (args)
#else
#define DEBUG_TOUCH(args...)
#endif

#if 1
#define DEBUG_APP(args...) g_message (args)
#else
#define DEBUG_APP(args...)
#endif

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
  DEBUG_APP ("ready! %p", application->android_application->window);

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
  self->touch_enabled = TRUE;
}

ClutterAndroidApplication *
clutter_android_application_get_default (void)
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
                            int32_t             cmd,
                            void               *data)
{
  ClutterAndroidApplication *application;

  application = CLUTTER_ANDROID_APPLICATION (app->userData);

  switch (cmd)
    {
    case APP_CMD_INIT_WINDOW:
      /* The window is being shown, get it ready */
      DEBUG_APP ("command: INIT_WINDOW");
      if (app->window != NULL)
        {
          gboolean initialized;
          ClutterStage *stage = clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());

          DEBUG_APP ("window = %p", application->android_application->window);

          /* Remove the fullscreen we ask at activity creation to be
             able to use it later if needed. */
          if (!application->had_window_once)
            {
              cogl_android_set_native_window (application->android_application->window);
              application->had_window_once = TRUE;
              ANativeActivity_setWindowFlags (application->android_application->activity,
                                              0, AWINDOW_FLAG_FULLSCREEN);

              g_signal_emit (application, signals[READY], 0, &initialized);
            }
          else
            {
              if (stage)
                {
                  ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (_clutter_stage_get_window (stage));

                  stage_cogl->onscreen = application->saved_onscreen;
                  application->saved_onscreen = NULL;

                  cogl_android_onscreen_update_native_window (stage_cogl->onscreen,
                                                              application->android_application->window);

                  clutter_actor_queue_relayout (CLUTTER_ACTOR (stage));
                  clutter_actor_queue_redraw (CLUTTER_ACTOR (stage));
                }
              else
                cogl_android_set_native_window (application->android_application->window);
            }

          application->have_window = TRUE;

          if (application->wait_for_window)
            {
              DEBUG_APP ("Waking up the waiting main loop");
              g_main_loop_quit (application->wait_for_window);
            }
        }
      break;

    case APP_CMD_TERM_WINDOW:
      /* The window is being hidden or closed, clean it up */
      DEBUG_APP ("command: TERM_WINDOW");
      application->have_window = FALSE;
      if (application->state == CLUTTER_ANDROID_APPLICATION_STATE_DESTROYED)
        {
          DEBUG_APP ("quitting app");

          if (application->wait_for_window)
            g_main_loop_quit (application->wait_for_window);
          else
            clutter_main_quit ();
        }
      else
        {
          ClutterStage *stage =
            clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());

          DEBUG_APP ("saving Cogl onscreen in case of resume");

          if (stage)
            {
              ClutterStageCogl *stage_cogl =
                CLUTTER_STAGE_COGL (_clutter_stage_get_window (stage));

              application->saved_onscreen = stage_cogl->onscreen;
              stage_cogl->onscreen = NULL;
            }
        }
      break;

    case APP_CMD_WINDOW_RESIZED:
      DEBUG_APP ("command: window resized!");
      if (app->window != NULL)
        {
          int32_t width = ANativeWindow_getWidth (app->window);
          int32_t height = ANativeWindow_getHeight (app->window);
          ClutterStage *stage = clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());

          DEBUG_APP ("resizing stage @ %ix%i", width, height);
          clutter_actor_set_size (CLUTTER_ACTOR (stage), width, height);
        }
      break;

    case APP_CMD_WINDOW_REDRAW_NEEDED:
      DEBUG_APP ("command: REDRAW_NEEDED");
      if (app->window != NULL)
        {
          int32_t width = ANativeWindow_getWidth (app->window);
          int32_t height = ANativeWindow_getHeight (app->window);
          ClutterStage *stage = clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());
          ClutterStageCogl *stage_cogl = CLUTTER_STAGE_COGL (_clutter_stage_get_window (stage));

          DEBUG_APP ("stage size %fx%f",
                     clutter_actor_get_width (CLUTTER_ACTOR (stage)),
                     clutter_actor_get_height (CLUTTER_ACTOR (stage)));
          if (clutter_actor_get_width (CLUTTER_ACTOR (stage)) != width ||
              clutter_actor_get_height (CLUTTER_ACTOR (stage)) != height)
            {
              DEBUG_APP ("resizing stage @ %ix%i", width, height);
              cogl_android_onscreen_update_size (stage_cogl->onscreen,
                                                 width, height);
              clutter_actor_queue_relayout (CLUTTER_ACTOR (stage));
              /* clutter_actor_set_size (CLUTTER_ACTOR (stage), width, height); */
            }
        }
      break;

    case APP_CMD_CONTENT_RECT_CHANGED:
      DEBUG_APP ("command: CONTENT_RECT_CHANGED");
      break;

    case APP_CMD_GAINED_FOCUS:
      DEBUG_APP ("command: GAINED_FOCUS");
      break;

    case APP_CMD_LOST_FOCUS:
      /* When our app loses focus, we stop monitoring the accelerometer.
       * This is to avoid consuming battery while not being used. */
      DEBUG_APP ("command: LOST_FOCUS");
      break;

    case APP_CMD_RESUME:
      DEBUG_APP ("command: RESUME");
      break;

    case APP_CMD_START:
      application->state = CLUTTER_ANDROID_APPLICATION_STATE_STARTED;
      DEBUG_APP ("command: START");
      break;

    case APP_CMD_STOP:
      application->state = CLUTTER_ANDROID_APPLICATION_STATE_STOPPED;
      DEBUG_APP ("command: STOP");
      break;

    case APP_CMD_PAUSE:
      application->state = CLUTTER_ANDROID_APPLICATION_STATE_PAUSED;
      DEBUG_APP ("command: PAUSE");
      break;

    case APP_CMD_DESTROY:
      application->state = CLUTTER_ANDROID_APPLICATION_STATE_DESTROYED;
      DEBUG_APP ("command: DESTROYED");
      break;
    }
}

static gboolean
translate_motion_event_to_pointer_event (AInputEvent *a_event)
{
  int32_t action;
  ClutterEvent *event;
  ClutterDeviceManager *manager;
  ClutterInputDevice *pointer_device;
  ClutterBackendAndroid *backend;

  manager = clutter_device_manager_get_default ();
  pointer_device =
    clutter_device_manager_get_core_device (manager,
                                            CLUTTER_POINTER_DEVICE);

  action = AMotionEvent_getAction (a_event);

  switch (action & AMOTION_EVENT_ACTION_MASK)
    {
    case AMOTION_EVENT_ACTION_DOWN:
      DEBUG_BUTTON ("BUTTON press\n");
      event = clutter_event_new (CLUTTER_BUTTON_PRESS);
      event->button.button = 1;
      event->button.click_count = 1;
      event->button.device = pointer_device;
      event->button.time = AMotionEvent_getEventTime (a_event);
      event->button.x = AMotionEvent_getX (a_event, 0);
      event->button.y = AMotionEvent_getY (a_event, 0);
      break;

    case AMOTION_EVENT_ACTION_UP:
      DEBUG_BUTTON ("BUTTON release\n");
      event = clutter_event_new (CLUTTER_BUTTON_RELEASE);
      event->button.button = 1;
      event->button.click_count = 1;
      event->button.device = pointer_device;
      event->button.time = AMotionEvent_getEventTime (a_event);
      event->button.x = AMotionEvent_getX (a_event, 0);
      event->button.y = AMotionEvent_getY (a_event, 0);
      break;

    case AMOTION_EVENT_ACTION_MOVE:
      DEBUG_BUTTON ("BUTTON move\n");
      event = clutter_event_new (CLUTTER_MOTION);
      event->motion.device = pointer_device;
       /* TODO: Following line is a massive hack for touch screen */
      event->motion.modifier_state = CLUTTER_BUTTON1_MASK;
      event->motion.time = AMotionEvent_getEventTime (a_event);
      event->motion.x = AMotionEvent_getX (a_event, 0);
      event->motion.y = AMotionEvent_getY (a_event, 0);
      break;

    default:
      DEBUG_BUTTON ("BUTTON meh? %i/%x\n", action, action);
      return FALSE;
    }

  event->any.stage =
    clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());
  _clutter_input_device_set_stage (pointer_device, event->any.stage);

  backend = (ClutterBackendAndroid *) clutter_get_default_backend ();
  _clutter_event_source_android_push_event (backend->android_source, event);

  return TRUE;
}

static gboolean
translate_motion_event_to_touch_event (ClutterAndroidApplication *application,
                                       AInputEvent *a_event)
{
  int32_t pointer_index;
  int32_t i, nb_pointers;
  int32_t action;
  int64_t current_time;
  ClutterStage *stage;
  ClutterEvent *event;
  ClutterDeviceManager *manager;
  ClutterInputDevice *pointer_device;
  ClutterBackendAndroid *backend;

  backend = (ClutterBackendAndroid *) clutter_get_default_backend ();

  stage =
    clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());

  manager = clutter_device_manager_get_default ();
  pointer_device =
    clutter_device_manager_get_core_device (manager,
                                            CLUTTER_POINTER_DEVICE);
  _clutter_input_device_set_stage (pointer_device, stage);

  action = AMotionEvent_getAction (a_event);
  pointer_index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >>
    AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
  action &= AMOTION_EVENT_ACTION_MASK;
  nb_pointers = AMotionEvent_getPointerCount (a_event);

  current_time = AMotionEvent_getEventTime (a_event);

  DEBUG_TOUCH ("TOUCH id=%i nb_pointers=%i action=%x\n",
               pointer_index, nb_pointers, action);

  for (i = 0; i < nb_pointers; i++)
    {
      int32_t current_id = AMotionEvent_getPointerId (a_event, i);

      if (i == pointer_index)
        {
          switch (action)
            {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
              DEBUG_TOUCH ("\ttouch begin on id=%i/%i\n", current_id, i);
              event = clutter_event_new (CLUTTER_TOUCH_BEGIN);
              break;

            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
              DEBUG_TOUCH ("\ttouch end on id=%i/%i\n", current_id, i);
              event = clutter_event_new (CLUTTER_TOUCH_END);
              break;

            case AMOTION_EVENT_ACTION_OUTSIDE: /* TODO: unsure about this one */
              DEBUG_TOUCH ("\ttouch outside id=%i/%i!\n", current_id, i);
              event = clutter_event_new (CLUTTER_TOUCH_CANCEL);
              break;

            case AMOTION_EVENT_ACTION_CANCEL:
              DEBUG_TOUCH ("\ttouch cancel id=%i/%i!\n", current_id, i);
              event = clutter_event_new (CLUTTER_TOUCH_CANCEL);
              break;

            case AMOTION_EVENT_ACTION_MOVE:
              event = clutter_event_new (CLUTTER_TOUCH_UPDATE);
              break;

            default:
              DEBUG_TOUCH ("\tmeh? action=%i\n", action);
              continue;
            }
        }
      else
        {
          DEBUG_TOUCH ("\ttouch update on id=%i\n", current_id);
          event = clutter_event_new (CLUTTER_TOUCH_UPDATE);
        }

      event->touch.time = current_time;
      event->touch.x = AMotionEvent_getX (a_event, i);
      event->touch.y = AMotionEvent_getY (a_event, i);
      event->touch.device = pointer_device;
      event->touch.modifier_state = application->modifier_state;
      /* TODO: We should be allocating proper structures for
         sequences, but for now it's ok (that explains the ugly kludge
         with + 1) */
      event->touch.sequence = (gpointer) (current_id + 1);

      event->any.stage = stage;

      _clutter_event_source_android_push_event (backend->android_source, event);
    }

  return TRUE;
}

static gboolean
translate_motion_event (AInputEvent *a_event)
{
  int32_t source = AInputEvent_getSource (a_event);
  ClutterAndroidApplication *application =
    clutter_android_application_get_default ();

  if ((source != AINPUT_SOURCE_MOUSE) &&
      application->touch_enabled)
    return translate_motion_event_to_touch_event (application, a_event);

  return translate_motion_event_to_pointer_event (a_event);
}

static gboolean
translate_key_event (AInputEvent *a_event)
{
  int32_t action;
  ClutterEvent *event;
  ClutterBackendAndroid *backend;
  ClutterDeviceManager *manager;
  ClutterInputDevice *keyboard_device;
  ClutterAndroidApplication *application =
    clutter_android_application_get_default ();
  int32_t new_modifier_state = application->modifier_state;

  action = AKeyEvent_getAction (a_event);

  manager = clutter_device_manager_get_default ();
  keyboard_device =
    clutter_device_manager_get_core_device (manager,
                                            CLUTTER_KEYBOARD_DEVICE);

  DEBUG_KEY ("KEY action = %i flags = %x meta = %x keycode = %i",
             AKeyEvent_getAction (a_event),
             AKeyEvent_getFlags (a_event),
             AKeyEvent_getMetaState (a_event),
             AKeyEvent_getKeyCode (a_event));

  switch (action)
    {
    case AKEY_EVENT_ACTION_DOWN:
      DEBUG_KEY ("\tkey press");
      event = clutter_event_new (CLUTTER_KEY_PRESS);
      new_modifier_state |= AKeyEvent_getMetaState (a_event);
      break;

    case AKEY_EVENT_ACTION_UP:
      DEBUG_KEY ("\tkey release");
      event = clutter_event_new (CLUTTER_KEY_RELEASE);
      new_modifier_state &= ~AKeyEvent_getMetaState (a_event);
      break;

    case AKEY_EVENT_ACTION_MULTIPLE:
      DEBUG_KEY ("\tcomplex string");
      return FALSE;

    default:
      DEBUG_KEY ("\tmeh? %i", action);
      return FALSE;
    }

  _clutter_android_translate_key_event ((ClutterKeyEvent *) event,
                                        application->modifier_state,
                                        a_event);

  event->key.device = keyboard_device;
  event->any.stage =
    clutter_stage_manager_get_default_stage (clutter_stage_manager_get_default ());
  _clutter_input_device_set_stage (keyboard_device, event->any.stage);

  application->modifier_state = new_modifier_state;

  backend = (ClutterBackendAndroid *) clutter_get_default_backend ();
  _clutter_event_source_android_push_event (backend->android_source, event);

  return TRUE;
}

/**
 * Process the next input event
 */
static int32_t
clutter_android_handle_input (struct android_app *app,
                              AInputEvent        *a_event)
{
  if (AInputEvent_getType (a_event) == AINPUT_EVENT_TYPE_KEY)
    return translate_key_event (a_event);
  else if (AInputEvent_getType (a_event) == AINPUT_EVENT_TYPE_MOTION)
    return translate_motion_event (a_event);

  return FALSE;
}

void
clutter_android_application_run (ClutterAndroidApplication *application)
{
  g_return_if_fail (CLUTTER_IS_ANDROID_APPLICATION (application));

  /* XXX: eeew. We wait to have a window to initialize Clutter and
   * thus to enter the clutter main loop */
  if (/* (application->state != CLUTTER_ANDROID_APPLICATION_STATE_DESTROYED && */
      /*  application->state != CLUTTER_ANDROID_APPLICATION_STATE_NONE) && */
      !application->have_window)
    {
      DEBUG_APP ("Waiting for the window");
      application->wait_for_window = g_main_loop_new (NULL, FALSE);
      g_main_loop_run (application->wait_for_window);
      g_main_loop_unref (application->wait_for_window);
      application->wait_for_window = NULL;
    }

  DEBUG_APP ("entering main loop");
  clutter_main ();
}

AAssetManager *
clutter_android_application_get_asset_manager (ClutterAndroidApplication *application)
{
  g_return_val_if_fail (CLUTTER_IS_ANDROID_APPLICATION (application), NULL);

  return application->android_application->activity->assetManager;
}

ANativeActivity *
clutter_android_application_get_native_activity (ClutterAndroidApplication *application)
{
  g_return_val_if_fail (CLUTTER_IS_ANDROID_APPLICATION (application), NULL);

  return application->android_application->activity;
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
      DEBUG_APP ("showing keyboard");
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
      DEBUG_APP ("hiding keyboard");
      if (implicit)
        ret = _android_show_keyboard (application->android_application,
                                      JNI_FALSE,
                                      ANATIVEACTIVITY_HIDE_SOFT_INPUT_IMPLICIT_ONLY);
      else
        ret = _android_show_keyboard (application->android_application,
                                      JNI_FALSE,
                                      ANATIVEACTIVITY_HIDE_SOFT_INPUT_NOT_ALWAYS);
    }
}

void
clutter_android_application_set_enable_touch (ClutterAndroidApplication *application,
                                              gboolean touch_enabled)
{
  g_return_if_fail (CLUTTER_IS_ANDROID_APPLICATION (application));

  application->touch_enabled = !!touch_enabled;
}

gboolean
clutter_android_application_get_enable_touch (ClutterAndroidApplication *application)
{
  g_return_val_if_fail (CLUTTER_IS_ANDROID_APPLICATION (application), FALSE);

  return application->touch_enabled;
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

  clutter_application = clutter_android_application_get_default ();

  DEBUG_APP ("app_state=%i", clutter_application->state);

  if (clutter_application->state != CLUTTER_ANDROID_APPLICATION_STATE_NONE)
    {
      clutter_main ();
      return;
    }

  android_application->userData = clutter_application;
  android_application->onAppCmd = clutter_android_handle_cmd;
  android_application->onInputEvent = clutter_android_handle_input;

  clutter_application->android_application = android_application;

  clutter_android_main (clutter_application);
}
