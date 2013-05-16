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


function display_cache_field_ui_form(&$form, &$form_state) {
  $entity_type = $form['#entity_type'];
  $bundle = $form['#bundle'];
  $view_mode = $form['#view_mode'];

  $settings = display_cache_get_settings($entity_type, $bundle, $view_mode);

  $form['display_cache'] = array(
    '#title' => t('Display Cache Display Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#access' => user_access('administer display cache'),
  );


  $display_cache_options = array(
    DISPLAY_CACHE_DISABLED => t('No caching'),
    DISPLAY_CACHE_ENABLED => t('Cache display'),
    DISPLAY_CACHE_FIELDS => t('Cache fields only'),
  );

  $use_default_value = 0;
  if (!empty($settings['default']['use'])) {
    $use_default_value = $settings['default']['use'];
  }
  $form['display_cache']['display_cache_use'] = array(
    '#type' => 'radios',
    '#title' => t('Cache the rendered HTML of this view mode.'),
    '#default_value' => $use_default_value,
    '#options' => $display_cache_options,
  );

  $page_granularity_options = array(
    0 => t('No page granularity'),
    DRUPAL_CACHE_PER_PAGE => t('Cache per URL'),
  );

  $page_default_value = 0;
  if (!empty($settings['default']['page_granularity'])) {
    $page_default_value = $settings['default']['page_granularity'];
  }

  $form['display_cache']['display_cache_page_granularity'] = array(
    '#type' => 'radios',
    '#title' => t('Cache per URL'),
    '#default_value' => $page_default_value,
    '#options' => $page_granularity_options,
    '#states' => array(
      'invisible' => array(
        'input[name="display_cache_use"]' => array('value' => 0),
      ),
    ),
  );

  $user_granularity_options = array(
    0 => t('No user granularity'),
    DRUPAL_CACHE_PER_ROLE => t('Cache per role'),
    DRUPAL_CACHE_PER_USER => t('Cache per user'),
  );

  $user_default_value = 0;
  if (!empty($settings['default']['user_granularity'])) {
    $user_default_value = $settings['default']['user_granularity'];
  }

  $form['display_cache']['display_cache_user_granularity'] = array(
    '#type' => 'radios',
    '#title' => t('User granularity'),
    '#default_value' => $user_default_value,
    '#options' => $user_granularity_options,
    '#states' => array(
      'invisible' => array(
        'input[name="display_cache_use"]' => array('value' => 0),
      ),
    ),
  );


    $field_instances = field_info_instances($entity_type, $bundle);
    foreach ($field_instances as $field_name => $field_instance) {
      if ($field_instance['display'][$view_mode]['type'] !== 'hidden') {
        $fields[$field_instance['display'][$view_mode]['weight']] = $field_name;
      }
    }

    ksort($fields);
    $form['display_cache_fields'] = array(
      '#title' => t('Display Cache Field Settings'),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#access' => user_access('administer display cache'),
      '#description' => t('Use this options to cache fields with less granularity.'),
    );

    foreach ($fields as $field_name) {
      $form_state['display_cache_field_settings'][] = $field_name;
      _display_cache_field_settings_form($form, $form_states, $settings, $field_name);
    }


  $form['#submit'][] = 'display_cache_view_mode_settings_form_submit';
}


function _display_cache_field_settings_form(&$form, &$form_states, $settings, $field_name) {

  $field_settings = array();
  if (!empty($settings[$field_name])) {
    $field_settings = $settings[$field_name];
  }

  $fieldset = 'display_cache_field_' . $field_name;
  $form['display_cache_fields'][$fieldset] = array(
    '#type' => 'fieldset',
    '#title' => $field_name,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  if ($settings['default']['use'] == DISPLAY_CACHE_DISABLED) {
    $form['display_cache_fields'][$fieldset]['#disabled'] = TRUE;
  }

  if (isset($field_settings['override']) && ($field_settings['override'] == DISPLAY_CACHE_ENABLED
    || $field_settings['override'] == DISPLAY_CACHE_DISABLED)) {
    $form['display_cache_fields'][$fieldset]['#collapsed'] = FALSE;
  }


  $display_cache_options = array(
    DISPLAY_CACHE_FIELDS => t('Use default'),
    DISPLAY_CACHE_ENABLED => t('Override display cache defaults'),
  );

 if (!empty($field_settings['override'])
    && $field_settings['override'] != 0
  ) {
    $override_default_value = $field_settings['override'];
  }
  else {
    $override_default_value = DISPLAY_CACHE_DISABLED;
  }

  if ($settings['default']['use'] === DISPLAY_CACHE_FIELDS) {
    $display_cache_options[DISPLAY_CACHE_DISABLED] = t('Do not cache this field');
  }


  $form['display_cache_fields'][$fieldset][$fieldset . '_override'] = array(
    '#type' => 'radios',
    '#title' => t('Override display cache defaults'),
    '#default_value' => $override_default_value,
    '#options' => $display_cache_options,
  );


  $page_granularity_options = array(0 => t('No page granularity'));
  $page_default_value = 0;

  if (!empty($settings['default']['page_granularity'])) {
    $page_granularity_options[DRUPAL_CACHE_PER_PAGE] = t('Cache per URL');
    if (!empty($field_settings['page_granularity'])
      && $field_settings['page_granularity'] != 0
    ) {
      $page_default_value = $field_settings['page_granularity'];
    }
  }


  $form['display_cache_fields'][$fieldset][$fieldset . '_page_granularity'] = array(
    '#type' => 'radios',
    '#title' => t('Cache per URL'),
    '#default_value' => $page_default_value,
    '#options' => $page_granularity_options,
    '#states' => array(
      'visible' => array(
        'input[name="' . $fieldset . '_override"]' => array('value' => 1),
      ),
    ),
  );


  $user_default_value = 0;
  if (!empty($field_settings['user_granularity'])
    && $field_settings['user_granularity'] != 0
  ) {
    $user_default_value = $field_settings['user_granularity'];
  }

  $user_granularity_options = array(0 => t('No user granularity'));
  if (!empty($settings['default']['user_granularity'])) {
    if ($settings['default']['user_granularity'] == DRUPAL_CACHE_PER_ROLE) {
      $user_granularity_options[DRUPAL_CACHE_PER_ROLE] = t('Cache per role');
    }
    else if ($settings['default']['user_granularity'] == DRUPAL_CACHE_PER_USER) {
      $user_granularity_options[DRUPAL_CACHE_PER_ROLE] = t('Cache per role');
      $user_granularity_options[DRUPAL_CACHE_PER_USER] = t('Cache per user');
    }
  }
  else {
    $user_default_value = 0;
  }

  $form['display_cache_fields'][$fieldset][$fieldset . '_user_granularity'] = array(
    '#type' => 'radios',
    '#title' => t('User granularity'),
    '#default_value' => $user_default_value,
    '#options' => $user_granularity_options,
    '#states' => array(
      'visible' => array(
        'input[name="' . $fieldset . '_override"]' => array('value' => 1),
      ),
    ),
  );
}


/**
 * Submit handler for display cache settings form.
 *
 * @see display_cache_form_field_ui_display_overview_form_alter()
 */
function display_cache_view_mode_settings_form_submit($form, $form_state) {

  $values = $form_state['values'];

  $settings['default']['use'] = $values['display_cache_use'];


  $settings['default']['page_granularity'] = $values['display_cache_page_granularity'];
  $settings['default']['user_granularity'] = $values['display_cache_user_granularity'];
  $settings['default']['granularity'] = $values['display_cache_page_granularity'] | $values['display_cache_user_granularity'];

  if (!empty($form_state['display_cache_field_settings'])) {

    foreach ($form_state['display_cache_field_settings'] as $field_name) {

      $field_override = $values['display_cache_field_' . $field_name . '_override'];
      if ($settings['default']['use'] != DISPLAY_CACHE_FIELDS && $field_override == DISPLAY_CACHE_DISABLED) {
        $settings[$field_name]['override'] = DISPLAY_CACHE_FIELDS;
      }
      else {
        $settings[$field_name]['override'] = $field_override;
      }

      if ($settings[$field_name]['override'] == DISPLAY_CACHE_ENABLED) {

        $field_page_granularity = $values['display_cache_field_' . $field_name . '_page_granularity'];
        if (!empty($settings['default']['page_granularity']) && $settings['default']['page_granularity'] <= $field_page_granularity) {
          $settings[$field_name]['page_granularity'] = $settings['default']['page_granularity'];
        }
        else {
          $settings[$field_name]['page_granularity'] = $field_page_granularity;
        }

        $field_user_granularity = $values['display_cache_field_' . $field_name . '_user_granularity'];
        if (!empty($settings['default']['user_granularity']) && $settings['default']['user_granularity'] <= $field_user_granularity) {
          $settings[$field_name]['user_granularity'] = $settings['default']['user_granularity'];
        }
        else {
          $settings[$field_name]['user_granularity'] = $field_user_granularity;
        }
      }
      else {
        $settings[$field_name]['page_granularity'] = $settings['default']['page_granularity'];
        $settings[$field_name]['user_granularity'] = $settings['default']['user_granularity'];
      }
      $settings[$field_name]['granularity'] = $settings[$field_name]['page_granularity'] | $settings[$field_name]['user_granularity'];
    }
  }


  $bundle = $form['#bundle'];
  $entity_type = $form['#entity_type'];
  $view_mode = $form['#view_mode'];

  variable_set('display_cache_' . $entity_type . '_' . $bundle . '_' . $view_mode, $settings);
}
