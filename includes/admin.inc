<?php
/**
 * @file
 * Administrative form.
 */

/**
 *
 */
function mysql_counter_admin_form($form, &$form_state) {
  $form = array(
    'mysql_count_admin' => array(
      'mysql_counter_execution_interval' => array(
        '#type' => 'textfield',
        '#title' => t('MySql execution interval'),
        '#description' => t('Time between executions in minutes. Minimum of 5.'),
        '#element_validate' => array(
          'element_validate_integer_positive',
          'mysql_counter_admin_execution_validate',
        ),
        '#default_value' => variable_get('mysql_counter_execution_interval', 15),
      ),
      'mysql_counter_error_log' => array(
        '#type' => 'textfield',
        '#title' => t('Log location'),
        '#description' => t('Full path to log to save to. Must be writable by the webserver user. Defaults /tmp/mysql_counter.log'),
        '#default_value' => variable_get('mysql_counter_error_log', '/tmp/mysql_counter.log'),
      ),
      '#validate' => array('mysql_counter_admin_form_validate'),
    ),
  );
  return system_settings_form($form);
}

function mysql_counter_admin_execution_validate($element, &$form_state) {
  $value = $element['#value'];
  if (intval($value) < 5) {
    form_error($element, t('Execution interval must be 5 minutes or larger.'));
  }
}

function mysql_counter_admin_form_validate($form, &$form_state) {
  if (!isset($form_state['values']['mysql_counter_error_log'])) {
    form_set_error('mysql_counter_error_log', t('Must provide a logfile to write to.'));
  }
  else {
    $value = $form_state['values']['mysql_counter_error_log'];
    if (!file_exists($value)) {
      try {
        $fp = @fopen($value, 0644);
        if ($fp === FALSE) {
          form_set_error('mysql_counter_error_log', t('Unable to create file at !f', array('!f' => $value)));
        }
        else {
          fclose($fp);
        }
      } catch (Exception $e) {
        form_set_error('mysql_counter_error_log', t('Unable to create file at !f', array('!f' => $value)));
      }
    }
    elseif (!is_writable($value)) {
      form_set_error('mysql_counter_error_log', t('Unable to write file at !f', array('!f' => $value)));
    }
  }
}
