<?php

/**
 * @file
 * Content type to expose rendered entities from display cache.
 */

$plugin = array(
  'title' => t('Display Cache: Rendered entity'),
  'defaults' => array('view_mode' => 'full', 'cache_by_role' => 0),
  'content type' => 'display_cache_entity_view_content_type_info',

);

/**
 * Get the entity content type info.
 */
function display_cache_entity_view_content_type_info($entity_type) {
  $types = display_cache_display_cache_entity_view_content_type_content_types();
  if (isset($types[$entity_type])) {
    return $types[$entity_type];
  }
}

/**
 * Implements hook_PLUGIN_content_type_content_types().
 */
function display_cache_display_cache_entity_view_content_type_content_types() {
  return _display_cache_get_types();
}

/**
 * Implements hook_PLUGIN_content_type_content_types().
 */
function display_cache_display_cache_entity_view_override_content_type_content_types() {
  return _display_cache_get_types(TRUE);
}

/**
 * Returns an edit form for a entity.
 */
function display_cache_display_cache_entity_view_content_type_edit_form($form, &$form_state) {

  $conf = $form_state['conf'];
  $entity_type = $form_state['subtype_name'];
  $entity_info = entity_get_info($entity_type);

  $options = array();
  if (!empty($entity_info['view modes'])) {
    foreach ($entity_info['view modes'] as $mode => $settings) {
      $options[$mode] = $settings['label'];
    }
  }

  if (count($options) > 1) {
    $form['view_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('View mode'),
      '#default_value' => $conf['view_mode'],
    );
  }
  else {
    $form['view_mode_info'] = array(
      '#type' => 'item',
      '#title' => t('View mode'),
      '#description' => t('Only one view mode is available for this entity type.'),
      '#markup' => $options ? current($options) : t('Default'),
    );

    $form['view_mode'] = array(
      '#type' => 'value',
      '#value' => $options ? key($options) : 'default',
    );

  }

  $form['cache_by_role'] = array(
    '#title' => t('Cache by role'),
    '#type' => 'checkbox',
    '#description' => t('If checked this entity will create a separate cache
    entry for each role.'),
    '#default_value' => $conf['cache_by_role'],
  );

  return $form;
}

/**
 * Returns an edit form for a entity.
 */
function display_cache_display_cache_entity_view_override_content_type_edit_form($form, &$form_state) {
  return display_cache_display_cache_entity_view_content_type_edit_form($form, $form_state);
}

/**
 * Saves selected view mode.
 */
function display_cache_display_cache_entity_view_content_type_edit_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['view_mode'])) {
    $form_state['conf']['view_mode'] = $form_state['values']['view_mode'];
  }
  if (isset($form_state['values']['cache_by_role'])) {
    $form_state['conf']['cache_by_role'] = $form_state['values']['cache_by_role'];
  }
}

/**
 * Saves selected view mode.
 */
function display_cache_display_cache_entity_view_override_content_type_edit_form_submit(&$form, &$form_state) {
  display_cache_display_cache_entity_view_content_type_edit_form_submit($form, $form_state);
}

/**
 * Implements hook_PLUGIN_content_type_render().
 * @see ctools_content_render()
 */
function display_cache_display_cache_entity_view_content_type_render($entity_type, $conf, $panel_args, $context) {
  if ($context->empty) {
    return;
  }
  $block = new stdClass();
  $block->module = 'entity';
  $block->delta = $entity_type . '-' . str_replace('-', '_', $conf['view_mode']);

  $entity_id = $context->argument;

  $roles = NULL;
  if ($conf['cache_by_role']) {
    global $user;
    $roles = $user->roles;
  }

  $block->content = display_cache_get_rendered_entity($entity_type, $entity_id, $conf['view_mode'], $roles);
  return $block;
}

/**
 * Implements hook_PLUGIN_content_type_render().
 */
function display_cache_display_cache_entity_view_override_content_type_render($entity_type, $conf, $panel_args, $context) {
  return display_cache_display_cache_entity_view_content_type_render($entity_type, $conf, $panel_args, $context);
}

/**
 * Implements hook_PLUGIN_content_type_admin_title().
 */
function display_cache_display_cache_entity_view_content_type_admin_title($entity_type, $conf, $contexts) {
  $view_mode = $conf['view_mode'];
  return _display_cache_get_admin_title($entity_type, $view_mode);
}

/**
 * Implements hook_PLUGIN_content_type_admin_title().
 */
function display_cache_display_cache_entity_view_override_content_type_admin_title($entity_type, $conf, $contexts) {
  $view_mode = $conf['view_mode'];
  return _display_cache_get_admin_title($entity_type, $view_mode, TRUE);
}

/**
 * Returns available entities which supports entity_view().
 * @param bool $override
 *    Selects title for panes which are overridden.
 * @return array
 */
function _display_cache_get_types($override = FALSE) {
  $title_text = 'Display Cache: Rendered @entity_type';
  if ($override) {
    $title_text = 'Rendered @entity_type | Cached by Display Cache';
  }
  $types = array();

  $entities = entity_get_info();

  foreach ($entities as $entity_type => $info) {
    if (entity_type_supports($entity_type, 'view')) {
      $types[$entity_type] = array(
        'title' => t($title_text, array('@entity_type' => $info['label'])),
        'category' => t('Entity'),
        'required context' => new ctools_context_required(t('Entity'), $entity_type),
      );
    }
  }

  return $types;
}

/**
 * Returns admin title for panes.
 * @param $entity_type
 *   Entity type like 'node' or 'comment'.
 * @param $view_mode
 *   View mode like 'full' or 'teaser'.
 * @param bool $override
 *   Selects title for panes which are overridden.
 * @return null|string
 */
function _display_cache_get_admin_title($entity_type, $view_mode, $override = FALSE) {
  $title_text = 'Display Cache: Rendered @entity_type using view mode "@view_mode"';
  if ($override) {
    $title_text = 'Rendered @entity_type using view mode "@view_mode" | Cached by Display Cache';
  }
  $entity_info = entity_get_info($entity_type);
  if (isset($entity_info['view modes'][$view_mode])) {
    $view_mode = $entity_info['view modes'][$view_mode]['label'];
  }
  return t($title_text, array('@entity_type' => $entity_info['label'], '@view_mode' => $view_mode));
}
