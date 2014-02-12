<?php

function preparatory_a(&$form_state, &$next_step) {
  // Define following step callback. If none set, that implies it is
  // the last step.
  $next_step = 'preparatory_b';

  // Retrieve submitted values. This comes in handy when back action
  // occurred and we need to display values that were originally submitted.
  $data = mforms_get_vals('myhaccp', 'preparatory_a');

  // If we have the data it means we arrived here from back action, so show
  // them in form as default values.
  $values = myhaccp_prepare_values($data, $form_state);

  drupal_set_title('Preparatory Stage A: Prerequisites');

  $form = array();
  $form['#attributes']['class'][] = 'prep-a';

  // Wrap the form section in a fieldset.
  $form['preparatory_a'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  $form['preparatory_a']['a_1'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Mark all boxes where you can demonstrate that you have control of the prerequisites'),
'#help_link' => 13,
    '#help_text' => t("These are 'pre-requirements' to the application and implementation of a HACCP system, to demonstrate you have control they should be well established, fully operational (including records) and verified."),
    '#options' => drupal_map_assoc(array(
      t('Supplier approval'),
      t('Packaging'),
      t('Incoming material specifications'),
      t('Finished product specification'),
      t('Training (incl. training needs analysis, job descriptions)'),
      t('Contract services (i.e. waste/laundry)'),
      t('Pest Control'),
      t('Glass and plastic management'),
      t('Calibration'),
      t('Standard Operating Procedures SOP\'s'),
      t('Distribution'),
      t('Product recall'),
      t('Document control'),
      t('Audit schedule (incl. HACCP/internal audits)'),
      t('Customer complaints'),
      t('Tracking non-conformances'),
      t('Microbiological control'),
      t('Preventative maintenance'),
      t('Traceability'),
      t('Utilities (air, water, energy)'),
      t('Waste disposal'),
      t('Equipment suitability, cleaning and maintenance'),
      t('Measures to prevent cross-contamination'),
      t('Cleaning and sanitising'),
      t('Personnel hygiene and employee facilities'),
      t('Rework'),
      t('Warehousing'),
      t('Product information / consumer awareness'),
      t('Food defence incl. Biovigilance and bioterrorism'),
      t('Contamination control'),
      t('Return to work'),
      t('Other'),
    )),
    '#default_value' => array_get($values, 'preparatory_a.a_1', array()),
    '#attributes' => array(
      'data-parsley-group' => 'checkgroup',
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please choose at least one option from the prerequisites field.'),
      'data-parsley-error-container' => '#edit-prep-a-a_1',
    ),
  );

  $form['preparatory_a']['a_2'] = array(
    '#type' => 'textarea',
    '#resizable' => FALSE,
    '#title' => t('Please specify the location of the above documents.'),
    '#help_text' => t("State where you have stored the information.  You may wish to specify a manual e.g. manual of prerequisites,  where information on these is filed e.g. all documents filed under prerequisites, cabinet 1 or reference a particular document e.g.  PRP 01."),
    '#help_link' => 13,
    '#default_value' => array_get($values, 'preparatory_a.a_2', FALSE),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Enter the location of your documents.'),
    ),
  );

  $form['#validate'][] = __FUNCTION__ . '_validate';

  return $form;
}

/**
 * Validation handler.
 */
function preparatory_a_validate($form, &$form_state) {
  $validator = new validator($form, $form_state, 'preparatory_a');

  if ($validator->allFieldsAreEmpty()) {
    return;
  }

  // Validation: Some content.
  $checks = array(
    'preparatory_a.a_1' => 'fieldIsNotEmpty',
    'preparatory_a.a_2' => 'fieldIsNotEmpty',
  );

  foreach ($checks as $target => $method) {
    $validator->validate($method, $target);
  }

  $validator->setErrorMessages();
}