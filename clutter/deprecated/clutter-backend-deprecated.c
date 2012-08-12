#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib-object.h>

#define CLUTTER_DISABLE_DEPRECATION_WARNINGS

#include "clutter-backend-private.h"
#include "clutter-settings.h"
#include "deprecated/clutter-backend.h"

/**
 * clutter_backend_set_double_click_time:
 * @backend: a #ClutterBackend
 * @msec: milliseconds between two button press events
 *
 * Sets the maximum time between two button press events, used to
 * verify whether it's a double click event or not.
 *
 * Since: 0.4
 *
 * Deprecated: 1.4: Use #ClutterSettings:double-click-time instead
 */
void
clutter_backend_set_double_click_time (ClutterBackend *backend,
                                       guint           msec)
{
  ClutterSettings *settings = clutter_settings_get_default ();

  g_object_set (settings, "double-click-time", msec, NULL);
}

/**
 * clutter_backend_get_double_click_time:
 * @backend: a #ClutterBackend
 *
 * Gets the maximum time between two button press events, as set
 * by clutter_backend_set_double_click_time().
 *
 * Return value: a time in milliseconds
 *
 * Since: 0.4
 *
 * Deprecated: 1.4: Use #ClutterSettings:double-click-time instead
 */
guint
clutter_backend_get_double_click_time (ClutterBackend *backend)
{
  ClutterSettings *settings = clutter_settings_get_default ();
  gint retval;

  g_object_get (settings, "double-click-time", &retval, NULL);

  return retval;
}

/**
 * clutter_backend_set_double_click_distance:
 * @backend: a #ClutterBackend
 * @distance: a distance, in pixels
 *
 * Sets the maximum distance used to verify a double click event.
 *
 * Since: 0.4
 *
 * Deprecated: 1.4: Use #ClutterSettings:double-click-distance instead
 */
void
clutter_backend_set_double_click_distance (ClutterBackend *backend,
                                           guint           distance)
{
  ClutterSettings *settings = clutter_settings_get_default ();

  g_object_set (settings, "double-click-distance", distance, NULL);
}

/**
 * clutter_backend_get_double_click_distance:
 * @backend: a #ClutterBackend
 *
 * Retrieves the distance used to verify a double click event
 *
 * Return value: a distance, in pixels.
 *
 * Since: 0.4
 *
 * Deprecated: 1.4: Use #ClutterSettings:double-click-distance instead
 */
guint
clutter_backend_get_double_click_distance (ClutterBackend *backend)
{
  ClutterSettings *settings = clutter_settings_get_default ();
  gint retval;

  g_object_get (settings, "double-click-distance", &retval, NULL);

  return retval;
}

/**
 * clutter_backend_set_resolution:
 * @backend: a #ClutterBackend
 * @dpi: the resolution in "dots per inch" (Physical inches aren't
 *   actually involved; the terminology is conventional).
 *
 * Sets the resolution for font handling on the screen. This is a
 * scale factor between points specified in a #PangoFontDescription
 * and cairo units. The default value is 96, meaning that a 10 point
 * font will be 13 units high. (10 * 96. / 72. = 13.3).
 *
 * Applications should never need to call this function.
 *
 * Since: 0.4
 *
 * Deprecated: 1.4: Use #ClutterSettings:font-dpi instead
 */
void
clutter_backend_set_resolution (ClutterBackend *backend,
                                gdouble         dpi)
{
  ClutterSettings *settings;
  gint resolution;

  g_return_if_fail (CLUTTER_IS_BACKEND (backend));

  if (dpi < 0)
    resolution = -1;
  else
    resolution = dpi * 1024;

  settings = clutter_settings_get_default ();
  g_object_set (settings, "font-dpi", resolution, NULL);
}

/**
 * clutter_backend_set_font_name:
 * @backend: a #ClutterBackend
 * @font_name: the name of the font
 *
 * Sets the default font to be used by Clutter. The @font_name string
 * must either be %NULL, which means that the font name from the
 * default #ClutterBackend will be used; or be something that can
 * be parsed by the pango_font_description_from_string() function.
 *
 * Since: 1.0
 *
 * Deprecated: 1.4: Use #ClutterSettings:font-name instead
 */
void
clutter_backend_set_font_name (ClutterBackend *backend,
                               const gchar    *font_name)
{
  ClutterSettings *settings = clutter_settings_get_default ();

  g_object_set (settings, "font-name", font_name, NULL);
}

/**
 * clutter_backend_get_font_name:
 * @backend: a #ClutterBackend
 *
 * Retrieves the default font name as set by
 * clutter_backend_set_font_name().
 *
 * Return value: the font name for the backend. The returned string is
 *   owned by the #ClutterBackend and should never be modified or freed
 *
 * Since: 1.0
 *
 * Deprecated: 1.4: Use #ClutterSettings:font-name instead
 */
const gchar *
clutter_backend_get_font_name (ClutterBackend *backend)
{
  ClutterBackendPrivate *priv;
  ClutterSettings *settings;

  g_return_val_if_fail (CLUTTER_IS_BACKEND (backend), NULL);

  priv = backend->priv;

  settings = clutter_settings_get_default ();

  /* XXX yuck. but we return a const pointer, so we need to
   * store it in the backend
   */
  g_free (priv->font_name);
  g_object_get (settings, "font-name", &priv->font_name, NULL);

  return priv->font_name;
}
