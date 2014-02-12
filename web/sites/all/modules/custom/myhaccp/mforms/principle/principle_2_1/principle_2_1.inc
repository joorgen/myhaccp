<?php

function principle_2_1(&$form_state, &$next_step) {

  // Define following step callback. If none set, that implies it is
  // the last step.
  $next_step = 'principle_2_2';

  // Retrieve submitted values. This comes in handy when back action
  // occurred and we need to display values that were originally submitted.
  $data = mforms_get_vals('myhaccp', 'principle_2_1');

  // If we have the data it means we arrived here from back action, so show
  // them in form as default values.
  $values = myhaccp_prepare_values($data, $form_state);

  drupal_set_title('Principle 2, questions 1-3: Determine Critical Control Points (CCPs)', PASS_THROUGH);

  $form = array();
  $form['#attributes']['class'][] = 'principle-2-question-1-3';

  // Wrap the form section in a fieldset.
  $form['principle_2_1'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  // Ensure data has been entered on principle_1_1.
  $principle_1_1 = mforms_get_vals('myhaccp', 'principle_1_1');
  if (myhaccp_ensure_hazards_exist($principle_1_1) == 0) {
    // There were no hazards.
    drupal_set_message('You must add at least a step number and step name in step 1.1 as well as at least a hazard for the step. Please return to 1.1', 'error');
    return $form;
  }

  // Ensure there are hazards over the threshold.
  if (myhaccp_ensure_over_threshold() == 0) {
    drupal_set_message('You have no significant hazards so you cannot continue', 'error');
    return $form;
  }

  // 1. Are you using a decision tree?
  $form['principle_2_1']['decision_tree'] = array(
    '#type' => 'radios',
    '#title' => t('Are you using a decision tree?'),
    '#help_link' => 27,
    '#help_text' => t('A ‘decision tree’ is a tool that  can be used to  help identify a critical control point.  You do not have to use a decision tree.'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
    '#default_value' => array_get($values, 'principle_2_1.decision_tree', array()),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please confirm whether you are using a decision tree.'),
    ),
  );

  // 2. Which decision tree are you using?
  $form['principle_2_1']['which_tree'] = array(
    '#type' => 'select',
    '#options' => array(
      '0' => t('-- Please select --'),
      'codex' => t('Codex'),
      'campden' => t('Campden'),
    ),
    '#title' => t('Which decision tree are you using?'),
    '#help_link' => 27,
    '#help_text' => t('You can select which decision tree you wish to use to help you, but you can use one  of your own or none at all.  More information on the  decision trees can be found.'),
    '#states' => array(
      'visible' => array(
        ':input[name="principle_2_1[decision_tree]"]' => array('value' => 'yes'),
      ),
    ),
    '#default_value' => array_get($values, 'principle_2_1.which_tree', array()),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-notequal' => '0',
      'data-parsley-error-message' => t('Please select a decision tree, or none if you are not using one.'),
    ),
  );

  // 3. If you are not using  either of the decision trees, please explain how you are deciding on your CCPs.
  $form['principle_2_1']['ccp_decision'] = array(
    '#type' => 'textarea',
    '#resizable' => FALSE,
    '#title' => t('Please explain how you are deciding on your CCPs. !tip', array(
      '!tip' => _help_tip(),
    )),
    '#states' => array(
      'visible' => array(
        ':input[name="principle_2_1[decision_tree]"]' => array('value' => 'no'),
      ),
    ),
    '#default_value' => array_get($values, 'principle_2_1.ccp_decision', FALSE),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please state how you are deciding your on your Critical Control Points.'),
    ),
  );

  $form['#validate'][] = 'principle_2_1_validate';

  return $form;
}

/**
 * Validation handler.
 */
function principle_2_1_validate($form, &$form_state) {
  $validator = new validator($form, $form_state, 'principle_2_1');

  if ($validator->allFieldsAreEmpty()) {
    return;
  }

  $checks = array(
    'principle_2_1.decision_tree' => 'fieldIsNotEmpty',
  );

  foreach ($checks as $target => $method) {
    $validator->validate($method, $target);
  }

  $tree = array_get($form_state, 'values.principle_2_1.decision_tree', FALSE);

  // If not using a decision tree.
  if ($tree == 'no') {
    $target = 'principle_2_1.ccp_decision';
    $validator->validate($method, $target);
  }
  elseif ($tree == 'yes') {
    // Using a decision tree so check which tree.
    $target = 'principle_2_1.which_tree';
    $validator->validate($method, $target);
  }

  $validator->setErrorMessages();
}