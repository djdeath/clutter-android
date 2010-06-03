/**
 * SECTION:clutter-actor-meta
 * @Title: ClutterActorMeta
 * @Short_Description: Base class of actor modifiers
 * @See_Also: #ClutterAction, #ClutterConstraint
 *
 * #ClutterActorMeta is an abstract class providing a common API for
 * modifiers of #ClutterActor
 *
 * A #ClutterActorMeta can only be owned by a single #ClutterActor at
 * any time
 *
 * #ClutterActorMeta is available since Clutter 1.4
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "clutter-actor-meta-private.h"

#include "clutter-debug.h"
#include "clutter-private.h"

struct _ClutterActorMetaPrivate
{
  ClutterActor *actor;
  guint destroy_id;

  gchar *name;

  guint is_enabled : 1;
};

enum
{
  PROP_0,

  PROP_ACTOR,
  PROP_NAME,
  PROP_ENABLED
};

G_DEFINE_ABSTRACT_TYPE (ClutterActorMeta,
                        clutter_actor_meta,
                        G_TYPE_INITIALLY_UNOWNED);

static void
on_actor_destroy (ClutterActor     *actor,
                  ClutterActorMeta *meta)
{
  meta->priv->actor = NULL;
}

static void
clutter_actor_meta_real_set_actor (ClutterActorMeta *meta,
                                   ClutterActor     *actor)
{
  if (meta->priv->actor == actor)
    return;

  if (meta->priv->destroy_id != 0)
    g_signal_handler_disconnect (meta->priv->actor, meta->priv->destroy_id);

  meta->priv->actor = actor;

  if (meta->priv->actor != NULL)
    meta->priv->destroy_id = g_signal_connect (meta->priv->actor, "destroy",
                                               G_CALLBACK (on_actor_destroy),
                                               meta);
}

static void
clutter_actor_meta_set_property (GObject      *gobject,
                                 guint         prop_id,
                                 const GValue *value,
                                 GParamSpec   *pspec)
{
  ClutterActorMeta *meta = CLUTTER_ACTOR_META (gobject);

  switch (prop_id)
    {
    case PROP_NAME:
      clutter_actor_meta_set_name (meta, g_value_get_string (value));
      break;

    case PROP_ENABLED:
      clutter_actor_meta_set_enabled (meta, g_value_get_boolean (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
    }
}

static void
clutter_actor_meta_get_property (GObject    *gobject,
                                 guint       prop_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
  ClutterActorMeta *meta = CLUTTER_ACTOR_META (gobject);

  switch (prop_id)
    {
    case PROP_ACTOR:
      g_value_set_object (value, meta->priv->actor);
      break;

    case PROP_NAME:
      g_value_set_string (value, meta->priv->name);
      break;

    case PROP_ENABLED:
      g_value_set_boolean (value, meta->priv->is_enabled);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (gobject, prop_id, pspec);
      break;
    }
}

static void
clutter_actor_meta_finalize (GObject *gobject)
{
  ClutterActorMetaPrivate *priv = CLUTTER_ACTOR_META (gobject)->priv;

  if (priv->destroy_id != 0 && priv->actor != NULL)
    g_signal_handler_disconnect (priv->actor, priv->destroy_id);

  g_free (priv->name);

  G_OBJECT_CLASS (clutter_actor_meta_parent_class)->finalize (gobject);
}

void
clutter_actor_meta_class_init (ClutterActorMetaClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GParamSpec *pspec;

  g_type_class_add_private (klass, sizeof (ClutterActorMetaPrivate));

  klass->set_actor = clutter_actor_meta_real_set_actor;

  gobject_class->set_property = clutter_actor_meta_set_property;
  gobject_class->get_property = clutter_actor_meta_get_property;
  gobject_class->finalize = clutter_actor_meta_finalize;

  /**
   * ClutterActorMeta:actor:
   *
   * The #ClutterActor attached to the #ClutterActorMeta instance
   *
   * Since: 1.4
   */
  pspec = g_param_spec_object ("actor",
                               P_("Actor"),
                               P_("The actor attached to the meta"),
                               CLUTTER_TYPE_ACTOR,
                               CLUTTER_PARAM_READABLE);
  g_object_class_install_property (gobject_class, PROP_ACTOR, pspec);

  /**
   * ClutterActorMeta:name:
   *
   * The unique name to access the #ClutterActorMeta
   *
   * Since: 1.4
   */
  pspec = g_param_spec_string ("name",
                               P_("Name"),
                               P_("The name of the meta"),
                               NULL,
                               CLUTTER_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_NAME, pspec);

  /**
   * ClutterActorMeta:enabled:
   *
   * Whether or not the #ClutterActorMeta is enabled
   *
   * Since: 1.4
   */
  pspec = g_param_spec_boolean ("enabled",
                                P_("Enabled"),
                                P_("Whether the meta is enabled"),
                                TRUE,
                                CLUTTER_PARAM_READWRITE);
  g_object_class_install_property (gobject_class, PROP_ENABLED, pspec);
}

void
clutter_actor_meta_init (ClutterActorMeta *self)
{
  self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self,
                                            CLUTTER_TYPE_ACTOR_META,
                                            ClutterActorMetaPrivate);

  self->priv->is_enabled = TRUE;
}

/**
 * clutter_actor_meta_set_name:
 * @meta: a #ClutterActorMeta
 * @name: the name of @meta
 *
 * Sets the name of @meta
 *
 * The name can be used to identify the #ClutterActorMeta instance
 *
 * Since: 1.4
 */
void
clutter_actor_meta_set_name (ClutterActorMeta *meta,
                             const gchar      *name)
{
  g_return_if_fail (CLUTTER_IS_ACTOR_META (meta));

  if (g_strcmp0 (meta->priv->name, name) == 0)
    return;

  g_free (meta->priv->name);
  meta->priv->name = g_strdup (name);

  g_object_notify (G_OBJECT (meta), "name");
}

/**
 * clutter_actor_meta_get_name:
 * @meta: a #ClutterActorMeta
 *
 * Retrieves the name set using clutter_actor_meta_set_name()
 *
 * Return value: (transfer none): the name of the #ClutterActorMeta
 *   instance, or %NULL if none was set. The returned string is owned
 *   by the #ClutterActorMeta instance and it should not be modified
 *   or freed
 *
 * Since: 1.4
 */
G_CONST_RETURN gchar *
clutter_actor_meta_get_name (ClutterActorMeta *meta)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR_META (meta), NULL);

  return meta->priv->name;
}

/**
 * clutter_actor_meta_set_enabled:
 * @meta: a #ClutterActorMeta
 * @is_enabled: whether @meta is enabled
 *
 * Sets whether @meta should be enabled or not
 *
 * Since: 1.4
 */
void
clutter_actor_meta_set_enabled (ClutterActorMeta *meta,
                                gboolean          is_enabled)
{
  g_return_if_fail (CLUTTER_IS_ACTOR_META (meta));

  is_enabled = !!is_enabled;

  if (meta->priv->is_enabled == is_enabled)
    return;

  meta->priv->is_enabled = is_enabled;

  g_object_notify (G_OBJECT (meta), "enabled");
}

/**
 * clutter_actor_meta_get_enabled:
 * @meta: a #ClutterActorMeta
 *
 * Retrieves whether @meta is enabled
 *
 * Return value: %TRUE if the #ClutterActorMeta instance is enabled
 *
 * Since: 1.4
 */
gboolean
clutter_actor_meta_get_enabled (ClutterActorMeta *meta)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR_META (meta), FALSE);

  return meta->priv->is_enabled;
}

/*
 * _clutter_actor_meta_set_actor
 * @meta: a #ClutterActorMeta
 * @actor: a #ClutterActor or %NULL
 *
 * Sets or unsets a back pointer to the #ClutterActor that owns
 * the @meta
 *
 * Since: 1.4
 */
void
_clutter_actor_meta_set_actor (ClutterActorMeta *meta,
                               ClutterActor     *actor)
{
  g_return_if_fail (CLUTTER_IS_ACTOR_META (meta));
  g_return_if_fail (actor == NULL || CLUTTER_IS_ACTOR (actor));

  CLUTTER_ACTOR_META_GET_CLASS (meta)->set_actor (meta, actor);
}

/**
 * clutter_actor_meta_get_actor:
 * @meta: a #ClutterActorMeta
 *
 * Retrieves a pointer to the #ClutterActor that owns @meta
 *
 * Return value: (transfer none): a pointer to a #ClutterActor or %NULL
 *
 * Since: 1.4
 */
ClutterActor *
clutter_actor_meta_get_actor (ClutterActorMeta *meta)
{
  g_return_val_if_fail (CLUTTER_IS_ACTOR_META (meta), NULL);

  return meta->priv->actor;
}

/*
 * ClutterMetaGroup: a collection of ClutterActorMeta instances
 */

G_DEFINE_TYPE (ClutterMetaGroup, _clutter_meta_group, G_TYPE_OBJECT);

static void
_clutter_meta_group_dispose (GObject *gobject)
{
  _clutter_meta_group_clear_metas (CLUTTER_META_GROUP (gobject));

  G_OBJECT_CLASS (_clutter_meta_group_parent_class)->dispose (gobject);
}

static void
_clutter_meta_group_class_init (ClutterMetaGroupClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

  gobject_class->dispose = _clutter_meta_group_dispose;
}

static void
_clutter_meta_group_init (ClutterMetaGroup *self)
{
}

/*
 * _clutter_meta_group_add_meta:
 * @group: a #ClutterMetaGroup
 * @meta: a #ClutterActorMeta to add
 *
 * Adds @meta to @group
 *
 * This function will remove the floating reference of @meta or, if the
 * floating reference has already been sunk, add a reference to it
 */
void
_clutter_meta_group_add_meta (ClutterMetaGroup *group,
                              ClutterActorMeta *meta)
{
  if (meta->priv->actor != NULL)
    {
      g_warning ("The meta of type '%s' with name '%s' is "
                 "already attached to actor '%s'",
                 G_OBJECT_TYPE_NAME (meta),
                 meta->priv->name != NULL
                   ? meta->priv->name
                   : "<unknown>",
                 clutter_actor_get_name (meta->priv->actor) != NULL
                   ? clutter_actor_get_name (meta->priv->actor)
                   : G_OBJECT_TYPE_NAME (meta->priv->actor));
      return;
    }

  group->meta = g_list_append (group->meta, meta);
  g_object_ref_sink (meta);

  _clutter_actor_meta_set_actor (meta, group->actor);
}

/*
 * _clutter_meta_group_remove_meta:
 * @group: a #ClutterMetaGroup
 * @meta: a #ClutterActorMeta to remove
 *
 * Removes @meta from @group and releases the reference being held on it
 */
void
_clutter_meta_group_remove_meta (ClutterMetaGroup *group,
                                 ClutterActorMeta *meta)
{
  if (meta->priv->actor != group->actor)
    {
      g_warning ("The meta of type '%s' with name '%s' is not "
                 "attached to the actor '%s'",
                 G_OBJECT_TYPE_NAME (meta),
                 meta->priv->name != NULL
                   ? meta->priv->name
                   : "<unknown>",
                 clutter_actor_get_name (group->actor) != NULL
                   ? clutter_actor_get_name (group->actor)
                   : G_OBJECT_TYPE_NAME (group->actor));
      return;
    }

  _clutter_actor_meta_set_actor (meta, NULL);

  group->meta = g_list_remove (group->meta, meta);
  g_object_unref (meta);
}

/*
 * _clutter_meta_group_peek_metas:
 * @group: a #ClutterMetaGroup
 *
 * Returns a pointer to the #ClutterActorMeta list
 *
 * Return value: a const pointer to the #GList of #ClutterActorMeta
 */
G_CONST_RETURN GList *
_clutter_meta_group_peek_metas (ClutterMetaGroup *group)
{
  return group->meta;
}

/*
 * _clutter_meta_group_clear_metas:
 * @group: a #ClutterMetaGroup
 *
 * Clears @group of all #ClutterActorMeta instances and releases
 * the reference on them
 */
void
_clutter_meta_group_clear_metas (ClutterMetaGroup *group)
{
  g_list_foreach (group->meta, (GFunc) _clutter_actor_meta_set_actor, NULL);

  g_list_foreach (group->meta, (GFunc) g_object_unref, NULL);
  g_list_free (group->meta);
  group->meta = NULL;
}

/*
 * _clutter_meta_group_get_meta:
 * @group: a #ClutterMetaGroup
 * @name: the name of the #ClutterActorMeta to retrieve
 *
 * Retrieves a named #ClutterActorMeta from @group
 *
 * Return value: a #ClutterActorMeta for the given name, or %NULL
 */
ClutterActorMeta *
_clutter_meta_group_get_meta (ClutterMetaGroup *group,
                              const gchar      *name)
{
  GList *l;

  for (l = group->meta; l != NULL; l = l->next)
    {
      ClutterActorMeta *meta = l->data;

      if (g_strcmp0 (meta->priv->name, name) == 0)
        return meta;
    }

  return NULL;
}
