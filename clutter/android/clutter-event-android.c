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
#include <unistd.h>

#include <glib.h>

#include "clutter-event-private.h"
#include "clutter-main.h"
#include "clutter-private.h"

#include "clutter-event-android.h"


typedef struct _ClutterEventSourceAndroid
{
  GSource source;
  GPollFD pfd;

  int pipe[2];
} ClutterEventSourceAndroid;

static gboolean
clutter_event_source_android_prepare (GSource *base, gint *timeout)
{
  gboolean retval;

  _clutter_threads_acquire_lock ();

  *timeout = -1;

  retval = clutter_events_pending ();

  _clutter_threads_release_lock ();

  return retval;
}

static gboolean
clutter_event_source_android_check (GSource *base)
{
  gboolean retval;
  ClutterEventSourceAndroid *source = (ClutterEventSourceAndroid *) base;

  _clutter_threads_acquire_lock ();

  retval = clutter_events_pending () || source->pfd.revents;

  _clutter_threads_release_lock ();

  return retval;
}

static gboolean
clutter_event_source_android_dispatch (GSource *base,
				       GSourceFunc callback,
				       gpointer data)
{
  ClutterEventSourceAndroid *source = (ClutterEventSourceAndroid *) base;
  ClutterEvent *event;
  int dummy;

  _clutter_threads_acquire_lock ();

  event = clutter_event_get ();

  if (source->pfd.revents)
    {
      read (source->pipe[0], &dummy, sizeof (dummy));
      source->pfd.revents = 0;
    }

  if (event)
    {
      /* forward the event into clutter for emission etc. */
      clutter_do_event (event);
      clutter_event_free (event);
    }

  _clutter_threads_release_lock ();

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

  if (pipe (source->pipe) == -1)
    {
      g_critical ("Cannot not create a pipe for event source");
      g_source_unref (&source->source);
      return NULL;
    }

  source->pfd.fd = source->pipe[0];
  source->pfd.events = G_IO_IN | G_IO_ERR;
  g_source_add_poll (&source->source, &source->pfd);

  return &source->source;
}

void
_clutter_event_source_android_push_event (GSource *source,
                                          ClutterEvent *event)
{
  ClutterEventSourceAndroid *asource = (ClutterEventSourceAndroid *) source;
  const int dummy = 42;

  _clutter_event_push (event, FALSE);

  write (asource->pipe[1], &dummy, sizeof (dummy));
}
