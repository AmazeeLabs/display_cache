<?php

/**
 * @file
 * Row style plugin for displaying the results as entities using display cache.
 */

/**
 * Plugin class for displaying Views results with display_cache_get_rendered_entity().
 */
class display_cache_views_plugin_row_entity_view extends entity_views_plugin_row_entity_view {

  public $rendered_content;

  public function pre_render($values) {

    if (!empty($values)) {
      list($this->entity_type, $this->entities) = $this->view->query->get_result_entities($values, !empty($this->relationship) ? $this->relationship : NULL, isset($this->field_alias) ? $this->field_alias : NULL);
    }

    // Get rendered entities.
    if ($this->entities) {

      foreach ($this->entities as $entity) {
        $entity_id = entity_id($this->entity_type, $entity);
        $this->rendered_content[$entity_id] = display_cache_get_rendered_entity($this->entity_type, $entity_id, $this->options['view_mode']);
      }
    }
  }


  public function render($values) {
    if ($entity = $this->get_value($values)) {
      return $this->rendered_content[entity_id($this->entity_type, $entity)];
    }
  }
}
