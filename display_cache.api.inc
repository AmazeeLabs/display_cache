<?php
/**
 * Api documentations.
 */

/**
 * Runs before an entity is rendered and cached.
 *
 * Usage example: Manipulate the renderable array.
 * @param array $render_array
 *   The rendarable array of an entity.
 * @param array $context
 *   ['entity_type'] string
 *     The entity type like 'node'.
 *   ['entity_id'] int
 *     The entity id.
 *   ['view_mode']  string
 *     View mode like 'full' or 'teaser'.
 *   ['roles']  array
 *     Array of machine readable names of roles. NULL if role awareness is
 *     deactivated.
 */
function hook_display_cache_pre_render_alter(&$render_array, $context) {

}

/**
 * Runs after a rendered entity was rendered.
 *
 * Usage example: Manipulate the rendered HTML or add JS with drupal_add_js.
 * @param string $html
 *   HTML of the rendered entity.
 * @param array $context
 *   ['entity_type'] string
 *     The entity type like 'node'.
 *   ['entity_id'] int
 *     The entity id.
 *   ['view_mode']  string
 *     View mode like 'full' or 'teaser'.
 *   ['roles']  array
 *     Array of machine readable names of roles. NULL if role awareness is
 *     deactivated.
 */
function hook_display_cache_post_render_alter(&$html, $context) {

}


/**
 * Alters the cache id for display cache entries.
 *
 * @param string $cid
 *   The actual cid.
 * @param array $context
 *   ['entity_type'] string
 *     The entity type like 'node'.
 *   ['entity_id'] int
 *     The entity id.
 *   ['view_mode']  string
 *     View mode like 'full' or 'teaser'.
 *   ['roles']  array
 *     Array of machine readable names of roles. NULL if role awareness is
 *     deactivated.
 */
function hook_display_cache_get_cid_alter(&$cid, $context) {

}
