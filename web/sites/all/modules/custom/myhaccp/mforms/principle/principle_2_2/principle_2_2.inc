<?php

/**
 * Principle 2, Question (part) 4.
 */
function principle_2_2(&$form_state, &$next_step) {
  // Define following step callback. If none set, that implies it is
  // the last step.
  $next_step = 'principle_3';

  // Retrieve submitted values.
  $data = mforms_get_vals('myhaccp', 'principle_2_2');

  // If we have the data it means we arrived here from back action, so show
  // them in form as default vals.
  $values = myhaccp_prepare_values($data, $form_state);

  // Set the page title.
  drupal_set_title('Principle 2, question 4: Specify control measures.');

  $form = array();
  $form['#attributes']['class'][] = 'principle-2-question-4';

  // Wrap the form section in a fieldset.
  $form['principle_2_2'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  // Determine how many instances we need to display. This is based on the data
  // from Principle 1 Question 3.
  $store = mforms_store_get('myhaccp');
  // Fetch the previous step data.
  $steps = MformsSteps::getInstance($store);
  $prev_step = $steps->getPrev();
  $previous_data = $store->getStore($prev_step);

  // Fetch the data from principle 1 question 3.
  $hazard_data = mforms_get_vals('myhaccp', 'principle_1_3');

  $items = array();

  // Iterate through the previous data.
  if (isset($hazard_data)) {
    foreach ($hazard_data['principle_1_3']['instance_table'] as $key => $item) {
      $hazard = unserialize($item['hazard']);
      $hazard['control_measure'] = $item['control_measure'];
      $items[] = $hazard;
    }
  }

  // Ensure data has been entered on principle_1_question_1.
  if (myhaccp_ensure_hazards_exist() == 0) {
    // There were no hazards.
    drupal_set_message('You must add at least a step number and step name in step 1.1 as well as at least a hazard for the step. Please return to 1.1', 'error');
    return $form;
  }

  // Ensure there are hazards over the threshold.
  if (myhaccp_ensure_over_threshold() == 0) {
    drupal_set_message('You have no significant hazards so you cannot continue', 'error');
    return $form;
  }

  $instance_count = count($items);
  // Bail out here with a message if there are no instances of the fieldsets
  // filled out correctly in the previous screen.
  if ($instance_count == 0) {
    drupal_set_message('You have no significant hazards so you cannot continue', 'error');
    return $form;
  }

  // Create the fieldset container.
  $form['principle_2_2'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  // Create the table.
  $form['principle_2_2']['instance_table'] = array(
    '#theme' => 'myhacpp_p2q4_table',
  );

  // Based on the information in the previous stage, build out the decision tree
  // radios.
  $decision_tree = array_get($previous_data, 'principle_2_1.decision_tree', '0');
  $tree_type = array_get($previous_data, 'principle_2_1.which_tree', '0');

  $form['principle_2_2']['instance_table']['decision_tree'] = array(
    '#type' => 'hidden',
    '#value' => $decision_tree,
  );

  // Build out the rows (three columns).
  foreach ($items as $key => $item) {
    // Prepare the hazard.
    $hazard = $item;
    // Prepare the text.
    $process_step_text = '(' . $item['step_number'] . ') ' . $item['step_name'];
    // The hazard column text.
    $hazard_text = '<div class="label hazard">' . $item['name'] . '</div><div class="hazard-description">' . $item['description'] . '</div>';

    $form['principle_2_2']['instance_table']['row_' . $key]['process_step'] = array(
      '#markup' => $process_step_text,
    );

    $form['principle_2_2']['instance_table']['row_' . $key]['hazard'] = array(
      '#type' => 'hidden',
      '#value' => serialize($hazard),
      '#prefix' => $hazard_text,
    );

    $form['principle_2_2']['instance_table']['row_' . $key]['control_measure'] = array(
      '#markup' => '<div class="hazard-control-measure">' . $item['control_measure'] . '</div>',
    );

    if ($decision_tree !== 'no') {
      $num = 1;
      for ($q = 1; $q <= 6; $q++) {
        $id = 'Q' . $num;
        if ($tree_type == 'codex') {
          if ($q == 2) {
            $id = 'Q1a';
            $num = 2;
          }
          else {
            $num++;
          }
        }
        else {
          if ($q == 3) {
            $id = 'Q2a';
            $num = 3;
          }
          else {
            $num++;
          }
        }

        $form['principle_2_2']['instance_table']['row_' . $key]['decision_tree'][$id] = array(
          '#type' => 'radios',
          '#options' => array(
            'yes' => t('Yes'),
            'no' => t('No'),
            'n/a' => t('N/A'),
          ),
          '#title' => $id,
          '#default_value' => array_get($values, "principle_2_2.instance_table.row_$key.decision_tree.$id", array()),
          '#attributes' => array(
            'data-parsley-required' => 'true',
            'data-parsley-error-message' => t('Please indicate \'Yes\', \'No\' or \'N/A\' for this question in the decision tree.'),
          ),
        );
      }

    }
    else {
      $form['principle_2_2']['instance_table']['row_' . $key]['decision_tree'] = array(
        '#type' => 'hidden',
        '#value' => $decision_tree,
      );
    }

    $form['principle_2_2']['instance_table']['row_' . $key]['ccp'] = array(
      '#type' => 'radios',
      '#options' => array(
        'yes' => t('Yes'),
        'no' => t('No'),
      ),
      '#default_value' => array_get($values, "principle_2_2.instance_table.row_$key.ccp", array()),
      '#attributes' => array(
        'data-parsley-required' => 'true',
        'data-parsley-error-message' => t('Please indicate whether this process step is a critical control point.'),
      ),
    );
  }

  // Add the validation handler.
  $form['#validate'][] = 'principle_2_2_validate';

  return $form;
}

/**
 * Validation handler for Principle 2 Question 4.
 */
function principle_2_2_validate($form, &$form_state) {
  $validator = new validator($form, $form_state, 'principle_2_2');

  if ($validator->allFieldsAreEmpty()) {
    return;
  }

  $checks = array();

  // Checks is an array with the key being the element being targeted (in dot
  // notation) and the value being the callback to use to check whether the
  // field is empty.
  $rows = array_get($form_state, 'values.principle_2_2.instance_table', array());
  // Get rid of the hidden decision_tree row as that is needed only for the
  // table theme function.
  unset($rows['decision_tree']);
  // Iterate over each row of the table.
  foreach ($rows as $key => $row) {
    // Within each row there may be multiple elements of a decision tree. There
    // may also be no decision tree.
    $tree = array_get($row, "decision_tree", array());
    if (is_array($tree)) {
      foreach ($tree as $id => $element) {
        $checks["principle_2_2.instance_table.$key.decision_tree.$id"] = 'fieldIsNotEmpty';
      }
    }
    // Ensure there is a CCP value.
    $checks["principle_2_2.instance_table.$key.ccp"] = 'fieldIsNotEmpty';
  }

  // Check all the errors.
  foreach ($checks as $target => $method) {
    $validator->validate($method, $target);
  }

  $validator->setErrorMessages();

}
