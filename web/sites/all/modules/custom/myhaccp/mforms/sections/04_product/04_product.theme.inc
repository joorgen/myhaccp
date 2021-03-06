<?php

/**
 * @file
 * Theme function for stage 4.
 *
 * Prepares the passed form_state data and processes it for output.
 */

/**
 * Preprocess function for 04_product.
 *
 * Prepare the form_state data for presentation in the template file.
 *
 * @param array $variables
 *   Form state data to process into variables for the template
 *   to display.
 */
function myhaccp_preprocess_04_product(&$variables) {
  // This section is made up of data from two stages.
  $study = $variables['study'];
  $data = $study->getStage('preparatory_e')->getData();
  $preparatory_e = $data['preparatory_e'];
  $data = $study->getStage('preparatory_f')->getData();
  $preparatory_f = $data['preparatory_f'];

  $variables['e_1_title'] = t('Ingredients');
  $variables['e_1'] = $preparatory_e['e_1'];

  $variables['e_2_title'] = t('Physical properties');
  $variables['e_2'] = $preparatory_e['e_2'];

  $variables['e_3_title'] = t('Processing');
  $variables['e_3'] = $preparatory_e['e_3'];

  $variables['e_4_title'] = t('Packaging');
  $variables['e_4'] = $preparatory_e['e_4'];

  $variables['e_5_title'] = t('Storage');
  $variables['e_5'] = $preparatory_e['e_5'];

  $variables['e_6_title'] = t('Shelf life of product');
  $variables['e_6'] = $preparatory_e['e_6'];

  $variables['e_7_title'] = t('Storing, handling and preparation advice');
  $variables['e_7'] = $preparatory_e['e_7'];

  // Name of the product.
  if (!empty($preparatory_f['f_1'])) {
    $variables['f_1'] = $preparatory_f['f_1'];
  }

  // 2. Is the product specifically designed for consumption by the general public?
  if (!empty($preparatory_f['f_2'])) {
    $variables['f_2'] = t('The product !isnot suitable for consumption by the general public.', array(
      '!isnot' => $preparatory_f['f_2'] == 'yes' ? t('is', array(), array('context' => 'is/is not')) : t('is not', array(), array('context' => 'is/is not')),
    ));
  }

  // 3. Is the product specifically designed for the young?
  if (!empty($preparatory_f['f_3'])) {
    $variables['f_3'] = t('The product !isnot suitable for the young.', array(
      '!isnot' => $preparatory_f['f_3'] == 'yes' ? t('is', array(), array('context' => 'is/is not')) : t('is not', array(), array('context' => 'is/is not')),
    ));
  }

  // 4. Is the product specifically designed for the elderly?
  if (!empty($preparatory_f['f_4'])) {
    $variables['f_4'] = t('The product !isnot suitable for the elderly.', array(
      '!isnot' => $preparatory_f['f_4'] == 'yes' ? t('is', array(), array('context' => 'is/is not')) : t('is not', array(), array('context' => 'is/is not')),
    ));
  }

  // 5. Is the product specifically designed for pregnant ladies?
  if (!empty($preparatory_f['f_5'])) {
    $variables['f_5'] = t('The product !isnot suitable for pregnant ladies.', array(
      '!isnot' => $preparatory_f['f_5'] == 'yes' ? t('is', array(), array('context' => 'is/is not')) : t('is not', array(), array('context' => 'is/is not')),
    ));
  }

  // 6. Is the product specifically designed for the immunosuppressed / immunocompromised?
  if (!empty($preparatory_f['f_6'])) {
    $variables['f_6'] = t('The product !isnot suitable for the immunosuppressed / immunocompromised.', array(
      '!isnot' => $preparatory_f['f_6'] == 'yes' ? t('is', array(), array('context' => 'is/is not')) : t('is not', array(), array('context' => 'is/is not')),
    ));
  }

  // 7. Is the product specifically designed for people with allergies or intolerances?
  if (!empty($preparatory_f['f_7'])) {
    $variables['f_7'] = t('The product !isnot suitable for people with allergies or intolerances.', array(
      '!isnot' => $preparatory_f['f_7'] == 'yes' ? t('is', array(), array('context' => 'is/is not')) : t('is not', array(), array('context' => 'is/is not')),
    ));
  }

  // 8. Detail below how you have considered the likely abuse/unintended use of the product by the consumer or purchaser.
  if (!empty($preparatory_f['f_8'])) {
    $variables['f_8_title'] = t('We have considered the likely abuse / unintended use of the product by the consumer in the following ways:');
    $variables['f_8'] = $preparatory_f['f_8'];
  }
}
