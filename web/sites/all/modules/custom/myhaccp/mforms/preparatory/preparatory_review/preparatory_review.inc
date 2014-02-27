<?php

function preparatory_review(&$form_state, &$next_step) {

  // Define following step callback. If none set, that implies it is
  // the last step.
  $next_step = 'principle_1_question_1';

  // Get the dependency injection container.
  global $container;
  // Get the current study.
  $study = $container['study'];
  $stage = $study->getCurrentStage();

  // Set the page title.
  drupal_set_title($stage->title, PASS_THROUGH);

  // Get the node output.
  $node = node_load(35);
  $node_output = node_view($node);

  // Get the stage.
  $phase = $study->getPhase('preparatory');

  $message_output = '';
  // Iterate over all the statuses of all the stages.
  foreach ($phase->getStages() as $stage) {

    if (strpos($stage->name, 'review') || strpos($stage->name, 'intro')) {
      // Do nothing for these stages.
      continue;
    }

    // Construct a url to the stage.
    $url = 'tool/study/' . $_SESSION['iid'] . '/' . $stage->url;

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
        $class = 'not_started';
        break;
    }

    $message_output .= '<div class="stage-validation ' . $class . '">';
    $message_output .= '<h3>' . $stage->title . '</h3>';
    $message_output .= '<div class="validation-status">' . $state . '</div>';
    $message_output .= '</div>';
  }

  $form = array();

  $form['preparatory_review'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
  );

  $form['preparatory_review']['validation'] = array(
    '#type' => 'container',
  );

  $form['preparatory_review']['validation']['messages'] = array(
    '#markup' => $message_output,
  );


  $form['preparatory_review']['node'] = array(
    '#markup' => drupal_render($node_output),
  );

  return $form;
}