<?php

/**
 * @file
 * Principles review stage.
 */

/**
 * Principles review.
 */
function principle_review(&$form_state, &$next_step) {

  // Get the dependency injection container.
  global $_myhaccp_container;
  // Get the current study.
  $study = $_myhaccp_container['study'];
  $stage = $study->getCurrentStage();

  // Set the page title.
  drupal_set_title($stage->title, PASS_THROUGH);

  // Get the stage.
  $phase = $study->getPhase('principle');

  $message_output = '<div class="validation-stages">';
  // Iterate over all the statuses of all the stages.
  foreach ($phase->getStages() as $stage) {

    if (strpos($stage->name, 'review') || strpos($stage->name, 'intro')) {
      // Do nothing for these stages.
      continue;
    }

    // Construct a url to the stage.
    $url = 'tool/study/' . $study->getIID() . '/' . $stage->getUrl();

    // Determine what is displayed and the class name for a pass or fail.
    $status = $stage->getStatus();
    switch ($status) {
      case VALID:
        $state = t('OK');
        $class = 'passed';
        break;

      case INVALID:
        $state = l(t('Needs review'), $url);
        $class = 'review';
        break;

      case NOT_STARTED:
        $state = l(t('Not started'), $url);
        $class = 'not-started';
        break;
    }

    $message_output .= '<div class="validation-stage ' . $class . '">';
    $message_output .= '<h3>' . filter_xss($stage->title) . '</h3>';
    $message_output .= '<div class="validation-status">' . $state . '</div>';
    $message_output .= '</div>';
  }
  $message_output .= '</div>';

  // Get the two blocks used for the text.
  $intro = block_load('bean', 'principle_review_intro');
  $render = _block_render_blocks(array($intro));
  $render_array = _block_get_renderable_array($render);
  $intro_output = drupal_render($render_array);

  $main = block_load('bean', 'principle_review_main');
  $render = _block_render_blocks(array($main));
  $render_array = _block_get_renderable_array($render);
  $main_output = drupal_render($render_array);

  $form = array();

  $form['principle_review']['validation'] = array(
    '#type' => 'container',
  );

  $form['principle_review']['validation']['messages'] = array(
    '#markup' => $intro_output . $message_output,
  );

  $form['principle_review']['node'] = array(
    '#markup' => $main_output,
  );

  $form['principle_review']['finish'] = array(
    '#markup' => l(t('Finish study'), 'tool/study/' . $study->getIID() . '/finish', array(
      'attributes' => array(
        'class' => array('finish'),
      ),
    )),
  );

  return $form;
}
