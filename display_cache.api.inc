<?php
/**
 * Api documentations.
 */

/**
 * Runs before an entity is rendered and cached.
 * Usage example: Manipulate the rendarable array.
 * @param array $render_array
 *   The rendarable array of an entity.
 * @param array $context
 *   ['entity_type'] string
 *     The entity type like 'node'.
 *   ['entity_id'] int
 *     The entity id.
 *   ['view_mode']  string
 *     View mode like 'full' or 'teaser'.
 */
function hook_display_cache_pre_render_alter(&$render_array, $context) {

}

/**
 * Runs after a rendered entity was rendered.
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
 */
function hook_display_cache_post_cache(&$html, $context) {

}


