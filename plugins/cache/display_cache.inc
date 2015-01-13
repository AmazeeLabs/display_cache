<?php

/**
 * @file
 * Provides a Display Cache plugin for panelizers.
 */

// Plugin definition
$plugin = array(
  'title' => t("Display Cache"),
  'description' => t('Display cache plugin will allow the panelizers to be cached.'),
  'cache get' => 'display_cache_panels_get_cache',
  'cache set' => 'display_cache_panels_set_cache',
  'cache clear' => 'display_cache_panels_clear_cache',
  'settings form' => 'display_cache_panels_settings_form',
  'settings form submit' => 'display_cache_panels_settings_form_submit',
);

/**
 * Get cached content.
 */
function display_cache_panels_get_cache($conf, $display, $args, $contexts, $pane = NULL) {
  // This must be a panelizer! Also, for the moment we do not cache panes, only
  // entire panels. The reason is that the panes can contain different types of
  // content, so we have to implement the logic to cache only fields or other
  // entities.
  if (empty($contexts['panelizer']) || !empty($pane)) {
    return FALSE;
  }
  $cid = display_cache_panels_get_id($conf, $display, $args, $contexts, $pane);
  $cache = cache_get($cid, DISPLAY_CACHE_CACHE_BIN);
  if (!$cache) {
    return FALSE;
  }

  return $cache->data;
}

/**
 * Set cached content.
 */
function display_cache_panels_set_cache($conf, $cache_object, $display, $args, $contexts, $pane = NULL) {
  $cid = display_cache_panels_get_id($conf, $display, $args, $contexts, $pane);
  cache_set($cid, $cache_object, DISPLAY_CACHE_CACHE_BIN);
}

/**
 * Clear cached content.
 *
 * Cache clears are always for an entire display, regardless of arguments.
 */
function display_cache_panels_clear_cache($display) {
  // @todo: we have to implement this if needed.
  /*$cid = 'panels_simple_cache';

  // This is used in case this is an in-code display, which means did will be something like 'new-1'.
  if (isset($display->owner) && isset($display->owner->id)) {
    $cid .= ':' . $display->owner->id;
  }
  $cid .= ':' . $display->did;

  cache_clear_all($cid, 'cache', TRUE);*/
}

/**
 * Figure out an id for our cache based upon input and settings.
 */
function display_cache_panels_get_id($conf, $display, $args, $contexts, $pane) {
  $entity_type = $contexts['panelizer']->type[2];
  list($entity_id,,) = entity_extract_ids($entity_type, $contexts['panelizer']->data);
  $words = explode(':', $display->cache_key);
  $view_mode = $words[3];
  // We have enabled this only for entities at the moment.
  $element = 'entity';

  // We should not mix render cache with display cache, because the display
  // cache caches the panels_cache_object object.
  $keys =  array('cache_type' => 'display_cache_panels');

  $keys += display_cache_get_cache_keys($entity_type, $entity_id, $view_mode, $element);
  $granularity = $conf['page_granularity'] | $conf['user_granularity'];
  $cid_parts = array(
    '#cache' => array(
      'keys' => $keys,
      'granularity' => $granularity,
    ),
  );

  return drupal_render_cid_create($cid_parts);
}

/**
 * Configuration form for the pane.
 */
function display_cache_panels_settings_form($conf, $display, $pid) {
  // Add radios with granularity options for cache-per-page.
  $page_granularity_options = array(
    0 => t('No page granularity'),
    DRUPAL_CACHE_PER_PAGE => t('Cache per URL'),
  );

  $form['page_granularity'] = array(
    '#type' => 'radios',
    '#title' => t('Cache per URL'),
    '#default_value' => !empty($conf['page_granularity']) ? $conf['page_granularity'] : 0,
    '#options' => $page_granularity_options,
  );

  // Add radios with granularity options for users.
  $user_granularity_options = array(
    0 => t('No user granularity'),
    DRUPAL_CACHE_PER_ROLE => t('Cache per role'),
    DRUPAL_CACHE_PER_USER => t('Cache per user'),
  );

  $form['user_granularity'] = array(
    '#type' => 'radios',
    '#title' => t('User granularity'),
    '#default_value' => !empty($conf['user_granularity']) ? $conf['user_granularity'] : 0,
    '#options' => $user_granularity_options,
  );
  return $form;
}

/**
 * Pane configuration form submit handler.
 */
function display_cache_panels_settings_form_submit($form, &$form_state) {

}
