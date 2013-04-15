<?php

/**
 * @file
 * Contains the display_cache_views_handler_field_entity class.
 */

/**
 * A handler to provide proper displays for entities using display cache.
 * @ingroup views_field_handlers
 */
class display_cache_views_handler_field_entity extends entity_views_handler_field_entity {


  public function option_definition() {
    $options = parent::option_definition();
    $options['view_mode'] = array('default' => 'default');
    return $options;
  }

  // Overrides form.
  public function options_form(&$form, &$form_state) {
    views_handler_field::options_form($form, $form_state);

    // Stolen from entity_views_plugin_row_entity_view.
    $entity_info = entity_get_info($this->field_entity_type);
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
        '#default_value' => $this->options['view_mode'],
        '#dependency' => array('edit-options-display' => array('view')),
      );
    }
    else {
      $form['view_mode'] = array(
        '#type' => 'value',
        '#value' => $options ? key($options) : 'default',
      );
    }
  }

  // Render entity using display cache.
  public function render($values) {
    $entity_type = $this->field_entity_type;
    $entity_id = $values->{$this->field_alias};
    $view_mode = $this->options['view_mode'];
    return display_cache_get_rendered_entity($entity_type, $entity_id, $view_mode);
  }
}
