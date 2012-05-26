/*
 * Clutter.
 *
 * An OpenGL based 'interactive canvas' library.
 *
 * Copyright (C) 2012  Intel Corporation.
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

 * Authors:
 *  Lionel Landwerlin <lionel.g.landwerlin@linux.intel.com>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>
#include <stdlib.h>

#include "clutter-event.h"
#include "clutter-main.h"

#include "clutter-event-android.h"

typedef struct _ClutterEventSourceAndroid
{
  GSource source;
} ClutterEventSourceAndroid;

static gboolean
clutter_event_source_android_prepare (GSource *base, gint *timeout)
{
  gboolean retval;

  clutter_threads_enter ();

  *timeout = -1;

  retval = clutter_events_pending ();

  clutter_threads_leave ();

  return retval;
}

static gboolean
clutter_event_source_android_check (GSource *base)
{
  gboolean retval;

  clutter_threads_enter ();

  retval = clutter_events_pending ();

  clutter_threads_leave ();

  return retval;
}

static gboolean
clutter_event_source_android_dispatch (GSource *base,
				       GSourceFunc callback,
				       gpointer data)
{
  ClutterEvent *event;

  clutter_threads_enter ();

  event = clutter_event_get ();

  if (event)
    {
      /* forward the event into clutter for emission etc. */
      clutter_do_event (event);
      clutter_event_free (event);
    }

  clutter_threads_leave ();

  return TRUE;
}

static GSourceFuncs clutter_event_source_android_funcs = {
    clutter_event_source_android_prepare,
    clutter_event_source_android_check,
    clutter_event_source_android_dispatch,
    NULL
};

GSource *
_clutter_event_source_android_new (void)
{
  ClutterEventSourceAndroid *source;

  source = (ClutterEventSourceAndroid *)
    g_source_new (&clutter_event_source_android_funcs,
                  sizeof (ClutterEventSourceAndroid));

  return &source->source;
}
