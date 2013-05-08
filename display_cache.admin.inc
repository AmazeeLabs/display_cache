<?php
/**
 * @file
 * Provides the Display Cache administrative interface.
 */

/**
 * Returns Display Cache settings forms.
 */
function display_cache_settings_forms() {
  $forms = array();
  $forms[] = drupal_get_form('display_cache_flush_caches_form');

  return $forms;
}

function display_cache_flush_caches_form($form, $form_state) {
  $form['display_cache_flush_caches'] = array(
    '#type' => 'submit',
    '#value' => t('Flush all display caches'),
  );

  return $form;
}

function display_cache_flush_caches_form_submit($form, $form_state) {
  cache_clear_all('*', 'cache_display_cache', TRUE);
}
