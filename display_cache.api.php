<?php
/**
 * @file
 * Hooks provided by the Display Cache module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alters the keys to build a cid.
 *
 * @param array $keys
 *   Array of keys. Used to build the cache id.
 *
 * @see drupal_render_cid_create()
 * @see drupal_render_cid_parts()
 */
function hook_display_cache_cache_keys_alter(&$keys) {

}

/**
 * @} End of "addtogroup hooks".
 */
