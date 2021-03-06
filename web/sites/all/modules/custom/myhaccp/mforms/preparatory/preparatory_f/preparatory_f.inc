<?php

/**
 * @file
 * Preparatory F stage form and validation handler.
 */

/**
 * Preparatory F.
 */
function preparatory_f(&$form_state, &$next_step) {
  // @ignore comment_comment_eg:function

  // Define following step callback. If none set, that implies it is
  // the last step.
  $next_step = 'preparatory_g';

  // Retrieve submitted values. This comes in handy when back action
  // occurred and we need to display values that were originally submitted.
  $data = mforms_get_vals('myhaccp', 'preparatory_f');

  // If we have the data it means we arrived here from back action, so show
  // them in form as default values.
  $values = myhaccp_prepare_values($data, $form_state);

  // Get the dependency injection container.
  global $_myhaccp_container;
  // Get the current study.
  $study = $_myhaccp_container['study'];
  // Get the stage.
  $stage = $study->getCurrentStage();
  drupal_set_title($stage->title, PASS_THROUGH);

  $form = array();
  $form['#attributes']['class'][] = 'prep-f';

  // Wrap the form section in a fieldset.
  $form['preparatory_f'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array(
      'class' => array(
        'stage-fieldset',
      ),
    ),
  );

  $form['preparatory_f']['intro'] = array(
    '#prefix' => '<p class="intro">',
    '#suffix' => '</p>',
    '#markup' => t('Knowledge of your consumer target group will help you later when you identify hazards that may become significant for vulnerable consumers.'),
  );

  // 1. Name of the product?
  $form['preparatory_f']['f_1'] = array(
    '#type' => 'textfield',
    '#title' => t('1. What is the name of the product(s)?'),
    '#help_link' => 19,
    '#help_text' => t('Specify the product name.'),
    '#default_value' => array_get($values, 'preparatory_f.f_1', FALSE),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please specify the name of the product.'),
    ),
  );

  // 2. Is the product specifically designed for consumption by the general public?
  $form['preparatory_f']['f_2'] = array(
    '#type' => 'radios',
    '#options' => array(
      'yes' => t('Yes', array(), array('context' => 'Is the product suitable for consumption by the general public?')),
      'no' => t('No', array(), array('context' => 'Is the product suitable for consumption by the general public?')),
    ),
    '#title' => t('2. Is the product suitable for consumption by the general public?'),
    '#help_link' => 19,
    '#help_text' => t("If your product is intended for a particular group of consumers, such as institutional caterers, travelers, etc. or for vulnerable groups of the population (young, elderly, immunocompromised, people with allergies, pregnant ladies) select 'no' as the response to this question.  It is very important that you understand your target group, as this will help to prepare you for the section on HACCP Principles (e.g. identify and list potential hazards)."),
    '#default_value' => array_get($values, 'preparatory_f.f_2', array()),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please specify if the product is suitable for the general public.'),
    ),
  );

  // 3. Is the product specifically designed for the young?
  $form['preparatory_f']['f_3'] = array(
    '#type' => 'radios',
    '#options' => array(
      'yes' => t('Yes', array(), array('context' => 'Is the product suitable for the young?')),
      'no' => t('No', array(), array('context' => 'Is the product suitable for the young?')),
    ),
    '#title' => t('3. Is the product suitable for the young?'),
    '#help_link' => 19,
    '#help_text' => t('Babies are more vulnerable to choking hazards certain products are not recommended to be consumed by babies e.g. honey.'),
    '#default_value' => array_get($values, 'preparatory_f.f_3', array()),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please specify if the product is suitable for the young.'),
    ),
  );

  // 4. Is the product specifically designed for the elderly?
  $form['preparatory_f']['f_4'] = array(
    '#type' => 'radios',
    '#options' => array(
      'yes' => t('Yes', array(), array('context' => 'Is the product suitable for the elderly?')),
      'no' => t('No', array(), array('context' => 'Is the product suitable for the elderly?')),
    ),
    '#title' => t('4. Is the product suitable for the elderly?'),
    '#help_link' => 19,
    '#help_text' => t('Elderly people may require that food is of a smaller size than that to be consumed by non-vulnerable groups.'),
    '#default_value' => array_get($values, 'preparatory_f.f_4', array()),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please specify if the product is suitable for the elderly.'),
    ),
  );

  // 5. Is the product specifically designed for pregnant ladies?
  $form['preparatory_f']['f_5'] = array(
    '#type' => 'radios',
    '#options' => array(
      'yes' => t('Yes', array(), array('context' => 'Is the product suitable for pregnant ladies?')),
      'no' => t('No', array(), array('context' => 'Is the product suitable for pregnant ladies?')),
    ),
    '#title' => t('5. Is the product suitable for pregnant ladies?'),
    '#help_link' => 19,
    '#help_text' => t('Certain foods should not be consumed by pregnant ladies because they contain certain microorganisms that may cause a still birth, miscarriage or severe illness.'),
    '#default_value' => array_get($values, 'preparatory_f.f_5', array()),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please specify if the product is suitable for pregnant ladies.'),
    ),
  );

  // 6. Is the product specifically designed for the immunosuppressed / immunocompromised?
  $form['preparatory_f']['f_6'] = array(
    '#type' => 'radios',
    '#options' => array(
      'yes' => t('Yes', array(), array('context' => 'Is the product suitable for the immunosuppressed / immunocompromised?')),
      'no' => t('No', array(), array('context' => 'Is the product suitable for the immunosuppressed / immunocompromised?')),
    ),
    '#title' => t('6. Is the product suitable for the immunosuppressed / immunocompromised?'),
    '#help_link' => 19,
    '#help_text' => t('Consider the use of low microbial diets to reduce the risk of foodborne disease.'),
    '#default_value' => array_get($values, 'preparatory_f.f_6', array()),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please specify if the product is suitable for the immunosuppressed / immunocompromised.'),
    ),
  );

  // 7. Is the product specifically designed for people with allergies or intolerances?
  $form['preparatory_f']['f_7'] = array(
    '#type' => 'radios',
    '#options' => array(
      'yes' => t('Yes', array(), array('context' => 'Is the product suitable for people with allergies or intolerances?')),
      'no' => t('No', array(), array('context' => 'Is the product suitable for people with allergies or intolerances?')),
    ),
    '#title' => t('7. Is the product suitable for people with allergies or intolerances?'),
    '#help_link' => 19,
    '#help_text' => t('Does the product contain any ingredients from the list on substances or products causing allergies or intolerances.'),
    '#default_value' => array_get($values, 'preparatory_f.f_7', array()),
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please specify if the product is suitable for people with allergies or intolerances.'),
    ),
  );

  // 8. Detail below how you have considered the likely abuse/unintended use of the product by the consumer or purchaser.
  $form['preparatory_f']['f_8'] = array(
    '#type' => 'textarea',
    '#title' => t('8. Detail below how you have considered the likely abuse/unintended use of the product by the consumer or purchaser.'),
    '#help_link' => 19,
    '#help_text' => t('e.g. It could be possible that a very young child may be fed this product despite it being aimed at those over 36 months.'),
    '#default_value' => array_get($values, 'preparatory_f.f_8', FALSE),
    '#resizable' => FALSE,
    '#attributes' => array(
      'data-parsley-required' => 'true',
      'data-parsley-error-message' => t('Please specify how how you have considered the likely abuse / unintended use of the product by the consumer.'),
    ),
  );

  $form['#validate'][] = __FUNCTION__ . '_validate';

  return $form;
}

/**
 * Validation handler.
 */
function preparatory_f_validate($form, &$form_state) {
  // Get the dependency injection container.
  global $_myhaccp_container;
  // Get the current study.
  $study = $_myhaccp_container['study'];
  // Get the stage.
  $stage = $study->getCurrentStage();
  $validator = $stage->getValidator();
  $validator->setFormState($form_state);
  $validator->setForm($form);

  if ($validator->pagerButtonUsed() && $validator->allFieldsAreEmpty()) {
    return;
  }

  $checks = array(
    'preparatory_f.f_1' => 'fieldIsNotEmpty',
    'preparatory_f.f_2' => 'fieldIsNotEmpty',
    'preparatory_f.f_3' => 'fieldIsNotEmpty',
    'preparatory_f.f_4' => 'fieldIsNotEmpty',
    'preparatory_f.f_5' => 'fieldIsNotEmpty',
    'preparatory_f.f_6' => 'fieldIsNotEmpty',
    'preparatory_f.f_7' => 'fieldIsNotEmpty',
    'preparatory_f.f_8' => 'fieldIsNotEmpty',
  );

  foreach ($checks as $target => $method) {
    $validator->validate($method, $target);
  }

  $validator->setErrorMessages();
  $form_state = $validator->getFormState();
}
