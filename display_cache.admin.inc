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
  $forms[] = drupal_get_form('display_cache_settings_form');

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


/**
 * Display Cache settings form.
 *
 * Returns a form to chose between no overrides and overrides of panels and
 * views plugins provided by the module Entity API.
 */
function display_cache_settings_form($form, $form_state) {
  $active = array(
    DISPLAY_CACHE_NO_OVERRIDE => t('No overrides'),
    DISPLAY_CACHE_ENTITY_OVERRIDE => t('Override Entity API'),
  );
  $form['display_cache_override_entity'] = array(
    '#type' => 'radios',
    '#title' => t('Override Entity'),
    '#default_value' => variable_get('display_cache_override_entity', DISPLAY_CACHE_NO_OVERRIDE),
    '#options' => $active,
    '#description' => t('<em>Display Cache</em> will add content panes and views
    plugins to provide cached displays in views and panels. If <em>Override
    Entity</em> is selected, <em>Display Cache</em> overrides panels and views
    plugins provided by the module <em>Entity API</em> with the same behavior.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'display_cache_settings_form_submit';
  return $form;
}

/**
 * Submit handler to flush views caches.
 */
function display_cache_settings_form_submit($form, $form_state) {
  cache_clear_all('*', 'cache_views', TRUE);
  cache_clear_all('*', 'cache_views_data', TRUE);
}
