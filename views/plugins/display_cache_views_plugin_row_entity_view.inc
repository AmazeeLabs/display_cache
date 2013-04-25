<?php

/**
 * @file
 * Row style plugin for displaying the results as entities using display cache.
 */

/**
 * Plugin class for displaying Views results with
 * display_cache_get_rendered_entity().
 */
class display_cache_views_plugin_row_entity_view extends entity_views_plugin_row_entity_view {

  public $rendered_content;

  public function option_definition() {
    $options = parent::option_definition();
    $options['cache_by_role'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['cache_by_role'] = array(
      '#title' => t('Cache by role'),
      '#type' => 'checkbox',
      '#description' => t('If checked this entity will create a separate cache
    entry for each role.'),
      '#default_value' => $this->options['cache_by_role'],
    );

    return $form;
  }

  public function pre_render($values) {

    if (!empty($values)) {
      list($this->entity_type, $this->entities) = $this->view->query->get_result_entities($values, !empty($this->relationship) ? $this->relationship : NULL, isset($this->field_alias) ? $this->field_alias : NULL);
    }

    // Get rendered entities.
    if ($this->entities) {

      foreach ($this->entities as $entity) {
        $entity_id = entity_id($this->entity_type, $entity);
        $entity_type = $this->entity_type;
        $view_mode = $this->options['view_mode'];
        $roles = NULL;
        if ($this->options['cache_by_role']) {
          global $user;
          $roles = $user->roles;
        }

        $this->rendered_content[$entity_id] = display_cache_get_rendered_entity($entity_type, $entity_id, $view_mode, $roles);
      }
    }
  }


  public function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->rendered_content[entity_id($this->entity_type, $entity)];
    }
  }
}
