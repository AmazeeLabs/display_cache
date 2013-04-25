<?php

/**
 * @file
 * Views hooks file.
 */

/**
 * Implements hook_views_plugins().
 *
 * Stolen from Entity Api.
 */
function display_cache_views_plugins() {
  // Have views cache the table list for us so it gets cleared at the
  // appropriate times.
  $data = views_cache_get('entity_base_tables', TRUE);
  if (!empty($data->data)) {
    $base_tables = $data->data;
  }
  else {
    $base_tables = array();
    foreach (views_fetch_data() as $table => $data) {
      if (!empty($data['table']['entity type']) && !empty($data['table']['base'])) {
        $base_tables[] = $table;
      }
    }
    views_cache_set('entity_base_tables', $base_tables, TRUE);
  }
  $plugins = array();
  if (!empty($base_tables)) {
    $plugins = array(
      'module' => 'display_cache',
      'row' => array(
        'display_cache' => array(
          'title' => t('Display Cache: Rendered entity'),
          'help' => t('Renders a cached single entity in a specific view mode (e.g. teaser).'),
          'handler' => 'DisplayCacheViewsPluginRowEntityView',
          'uses fields' => FALSE,
          'uses options' => TRUE,
          'type' => 'normal',
          'base' => $base_tables,
        ),
      ),
    );
  }

  return $plugins;
}

/**
 * Implements hook_views_plugins_alter().
 *
 * Implemented to override plugins provided by Entity API with display cache.
 */
function display_cache_views_plugins_alter(&$plugins) {
  if (variable_get('override_entity', 0) === DISPLAY_CACHE_ENTITY_OVERRIDE) {
    $plugins['row']['entity']['title']
      = t('Rendered entity | Cached by Display Cache');

    $plugins['row']['entity']['help']
      = t('Renders a cached single entity in a specific view mode (e.g. teaser).');

    $plugins['row']['entity']['handler']
      = 'DisplayCacheViewsPluginRowEntityView';

    $plugins['row']['entity']['module'] = 'display_cache';

    $plugins['row']['entity']['theme path']
      = drupal_get_path('module', 'display_cache');
    $plugins['row']['entity']['theme file'] = 'display_cache.views.inc';

    $plugins['row']['entity']['path']
      = drupal_get_path('module', 'display_cache');

    $plugins['row']['entity']['file']
      = 'DisplayCacheViewsPluginRowEntityView.inc';

    $plugins['row']['entity']['name'] = 'display_cache';
  }
}


/**
 * Implements hook_views_data().
 *
 * Stolen from Entity Api.
 */
function display_cache_views_data() {
  $data = array();

  foreach (entity_get_info() as $entity_type => $info) {
    if (entity_type_supports($entity_type, 'view')) {
      $data['views_entity_' . $entity_type]['display_cache_rendered_entity'] = array(
        'title' => t('Display Cache: Rendered @entity-type', array('@entity-type' => $info['label'])),
        'help' => t('The @entity-type of the current relationship rendered using a view mode. Provided by Entity cached by Display Cache.', array('@entity-type' => $info['label'])),
        'field' => array(
          'handler' => 'DisplayCacheViewsHandlerFieldEntity',
          'type' => $entity_type,
          'real field' => 'entity object',
        ),
      );
    }
  }

  return $data;
}

/**
 * Implements hook_entity_views_field_handlers_alter().
 */
function display_cache_entity_views_field_handlers_alter(&$field_handlers) {
  if (variable_get('override_entity', 0) === DISPLAY_CACHE_ENTITY_OVERRIDE) {
    $field_handlers['entity'] = 'DisplayCacheViewsHandlerFieldEntity';
  }
}

/**
 * Implements hook_views_data_alter().
 *
 * Implemented to override plugins provided by Entity API with display cache.
 */
function display_cache_views_data_alter(&$data) {
  if (variable_get('override_entity', 0) === DISPLAY_CACHE_ENTITY_OVERRIDE) {
    foreach (entity_get_info() as $entity_type => $info) {
      if (entity_type_supports($entity_type, 'view')) {

        $data['views_entity_' . $entity_type]['rendered_entity']['title']
          = t('Rendered @entity-type  | Cached by Display Cache', array('@entity-type' => $info['label']));

        $data['views_entity_' . $entity_type]['rendered_entity']['help']
          = t('The @entity-type of the current relationship rendered using a view mode.', array('@entity-type' => $info['label']));

        $data['views_entity_' . $entity_type]['rendered_entity']['field']['handler']
          = 'DisplayCacheViewsHandlerFieldEntity';
      }
    }
  }
}
