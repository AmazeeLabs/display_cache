<?php

/**
 * @file
 * Row style plugin for displaying the results as entities using display cache.
 */

/**
 * Plugin class for displaying Views results with
 * display_cache_get_rendered_entity().
 */
class DisplayCacheViewsPluginRowEntityView extends entity_views_plugin_row_entity_view {

  public $renderedContent;

  /**
   * Returns option definitions.
   */
  public function option_definition() {

    $options = parent::option_definition();
    $options['cache_by_role'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  /**
   * Returns options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['cache_by_role'] = array(
      '#title' => t('Cache by role'),
      '#type' => 'checkbox',
      '#description' => t('If checked this entity will create a separate cache entry for each role.'),
      '#default_value' => $this->options['cache_by_role'],
    );

    return $form;
  }

  /**
   * Pre render callback.
   */
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
        if (!empty($this->options['cache_by_role']) && $this->options['cache_by_role']) {
          global $user;
          $roles = $user->roles;
        }

        $this->renderedContent[$entity_id] = display_cache_get_rendered_entity($entity_type, $entity_id, $view_mode, $roles);
      }
    }
  }


  /**
   * Returns the rendered and cached entity.
   */
  public function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->renderedContent[entity_id($this->entity_type, $entity)];
    }
  }
}
