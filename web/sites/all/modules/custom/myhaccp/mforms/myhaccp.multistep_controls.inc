<?php

/**
 * @file
 * Single class that is an implementation of controls.
 *
 * This implementation provides advanced controls for multistep form
 * where each step gets own button.
 */

/**
 * Provides navigation functionality for forms.
 */
class MyHACCPMultiStepControls extends MformsControls {

  private static $instances = array();

  protected $steps_definition = array();

  protected $label_back;
  protected $label_continue;
  protected $label_submit;
  protected $label_delete;
  protected $label_save;

  protected $weight_nav = 20;
  protected $weight_submit = 20;
  protected $weight_delete = 12;
  protected $weight_save = 5;

  protected $attributes_back = array();
  protected $attributes_continue = array();
  protected $attributes_submit = array();
  protected $attributes_delete = array();
  protected $attributes_save = array();

  protected $steps_label;


  /**
   * Gets a singleton instance.
   */
  public static function getInstance(MformsIstore $store, MformsSteps $steps, $steps_definition = NULL) {
    if (empty(self::$instances[$store->getKey()])) {
      $class = get_called_class();
      self::$instances[$store->getKey()] = new $class($store, $steps, $steps_definition);
    }

    return self::$instances[$store->getKey()];
  }

  /**
   * Constructor method.
   *
   * @param \MformsIstore $store
   * @param \MformsSteps $steps
   * @param array $steps_definition
   *   Array of step definitions.
   */
  protected function __construct(MformsIstore $store, MformsSteps $steps, $steps_definition) {
    $this->store = $store;
    $this->steps = $steps;
    $this->steps_definition = $steps_definition;

    // Init default labels.
    $this->label_continue = t('Save and continue');
    $this->label_back = t('Previous');
    $this->label_save = t('Save this page');
    $this->label_submit = t('Submit');
    $this->label_delete = t('Reset');
  }

  /**
   * Gets the controls for the form.
   */
  public function getControls($form) {
    $form_controls = array();

    if ($this->isSingleStep()) {
      return $form_controls;
    }

    $wrapper_id = $form['mforms_info']['#value']['wrapper_id'];

    // The form steps are displayed above the form. Create a wrapper for them.
    $form_controls['steps_btns_wrapper'] = array(
      '#type' => 'container',
      '#weight' => -1000,
      '#attributes' => array(
        'class' => array('pager-wrapper'),
      ),
    );

    $form_controls['steps_btns_wrapper']['preparatory'] = array(
      '#type' => 'container',
      '#title' => $this->getStepsLabel(),
      '#attributes' => array(
        'class' => array('preparatory-wrapper'),
      ),
    );
    $form_controls['steps_btns_wrapper']['preparatory']['pager_controls'] = array(
      '#type' => 'container',
      '#prefix' => '<h3>' . t('Preparatory:') . ' </h3>',
      '#attributes' => array(
        'class' => array('pager-controls'),
      ),
    );
    $form_controls['steps_btns_wrapper']['principles'] = array(
      '#type' => 'container',
      '#title' => $this->getStepsLabel(),
      '#attributes' => array(
        'class' => array('principles-wrapper'),
      ),
    );
    $form_controls['steps_btns_wrapper']['principles']['pager_controls'] = array(
      '#type' => 'container',
      '#prefix' => '<h3>' . t('Principles:') . ' </h3>',
      '#attributes' => array(
        'class' => array('pager-controls'),
      ),
    );

    // Get the dependency injection container.
    global $_myhaccp_container;
    // Get the current study.
    $study = $_myhaccp_container['study'];

    // Iterate through all the form steps and display them one after another.
    foreach ($this->steps_definition as $id => $definition) {
      $stage = $study->getStage($id);
      // Fetch the stage status.
      $status = $stage->getStatus();

      $unvisited = TRUE;
      if ($this->store->getStore($id) && !$this->isSingleStep()) {
        $unvisited = FALSE;
      }

      $class = 'pager-item';

      if ($this->steps->getCurr() == $id) {
        $class .= ' current-step';
      }
      elseif ($unvisited) {
        $class .= ' unvisited-step';
      }

      switch ($status) {
        case NOT_STARTED:
          $class .= ' not-started';
          break;

        case INVALID:
          $class .= ' needs-review';
          break;

        case VALID:
          $class .= ' valid';
          break;
      }

      // Show the step button.
      $form_controls['steps_btns_wrapper'][$definition['stage']]['pager_controls']['callback_' . $id] = array(
        '#type' => 'submit',
        '#value' => $definition['value'],
        '#weight' => $definition['weight'],
        '#attributes' => array('class' => array($class)),
        '#limit_validation_errors' => array(),
        '#submit' => array('myhaccp_pager_submit'),
        '#store_key' => $this->store->getKey(),
        '#mforms_step' => $id,
      );

      if (strpos($id, 'review') || strpos($id, 'intro')) {
        $form_controls['steps_btns_wrapper'][$definition['stage']]['pager_controls']['callback_' . $id]['#name'] = $id;
      }

      if ($this->ajaxified) {
        $form_controls['steps_btns_wrapper'][$definition['stage']]['pager_controls']['callback_' . $id]['#ajax'] = array(
          'callback' => '_mforms_ajax_get_form',
          'wrapper' => $wrapper_id,
          'method' => 'replace',
          'effect' => 'fade',
        );
      }
    }

    // Create a wrapper for the continue and previous buttons.
    $form_controls['form_nav_wrapper'] = array(
      '#type' => 'container',
      '#weight' => $this->weight_nav,
      '#attributes' => array(
        'class' => array('form-nav-wrapper'),
      ),
    );

    // Check if there is a next step (screen).
    if ($this->steps->getNext() != NULL) {
      $form_controls['form_nav_wrapper']['continue'] = array(
        '#type' => 'submit',
        '#value' => $this->label_continue,
        '#weight' => 20,
        '#attributes' => $this->attributes_continue,
        '#submit' => array('myhaccp_continue_submit'),
        '#store_key' => $this->store->getKey(),
        '#attributes' => array(
          'class' => array('form-continue'),
        ),
      );

      // If this is a review or intro stage change the wording.
      if ($this->reviewOrIntro($this->steps->getCurr())) {
        $form_controls['form_nav_wrapper']['continue']['#value'] = t('Continue');
      }
    }

    if ($this->steps->getPrev() != NULL) {
      $form_controls['form_nav_wrapper']['previous'] = array(
        '#type' => 'submit',
        '#value' => $this->label_back,
        '#weight' => 10,
        '#store_key' => $this->store->getKey(),
        '#mforms_step' => $this->steps->getPrev(),
        '#submit' => array('myhaccp_back_submit'),
        '#attributes' => array(
          'class' => array('form-previous'),
        ),
      );
    }

    // Hide the save button on a review or intro stage.
    if (!$this->reviewOrIntro($this->steps->getCurr())) {

      // Create a wrapper for the form save button and accompanying message.
      $form_controls['form_save_wrapper'] = array(
        '#type' => 'container',
        '#weight' => $this->weight_save,
        '#attributes' => array(
          'class' => array('form-save-wrapper'),
        ),
      );

      // Add a save button. This button simply saves the form state, much as the
      // previous and next buttons do.
      $form_controls['form_save_wrapper']['save'] = array(
        '#type' => 'submit',
        '#value' => $this->label_save,
        '#weight' => 20,
        '#attributes' => $this->attributes_save,
        '#store_key' => $this->store->getKey(),
        '#mforms_step' => $this->steps->getCurr(),
        '#submit' => array('myhaccp_save_submit'),
        '#attributes' => array(
          'class' => array('form-save'),
        ),
      );

      $form_controls['form_save_wrapper']['message'] = array(
        '#markup' => '<div class="help">' . t('Navigating within the tool will save your progress. If you are leaving the tool entirely you can') . ' </div>',
        '#weight' => 10,
      );
    }

    return $form_controls;
  }

  /**
   * Submits the form on clicked actions.
   */
  public function doSubmitOnClickedAction() {
    $step_definition = $this->steps_definition[$this->steps->getCurr()];
    return isset($step_definition['submit']) ? $step_definition['submit'] : FALSE;
  }

  /**
   * Determines whether the stage is a review or intro stage.
   *
   * @param string $id
   *   The stage id.
   *
   * @return bool
   *   Boolean indicating whether the stage is a review or intro or not.
   */
  public function reviewOrIntro($id) {
    if (strpos($id, 'review') || strpos($id, 'intro')) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Sets custom attributes for controls.
   *
   * @param array $back
   *   Back button attributes.
   * @param array $continue
   *   Continue button attributes.
   * @param array $submit
   *   Submit button attributes.
   * @param array $delete
   *   Delete/reset button attributes.
   */
  public function setControlsAttributes($back, $continue, $submit, $delete) {
    if (is_array($back)) {
      $this->attributes_back = $back;
    }
    if (is_array($continue)) {
      $this->attributes_continue = $continue;
    }
    if (is_array($submit)) {
      $this->attributes_submit = $submit;
    }
    if (is_array($delete)) {
      $this->attributes_delete = $delete;
    }
  }

  /**
   * Sets custom labels for form controls.
   *
   * @param string $back
   *   Back button label.
   * @param string $continue
   *   Continue button label.
   * @param string $submit
   *   Submit button label.
   * @param string $delete
   *   Delete/reset button label.
   */
  public function setControlsLabels($back, $continue, $submit, $delete) {
    if ($back != NULL && trim($back) != "") {
      $this->label_back = $back;
    }
    if ($continue != NULL && trim($continue) != "") {
      $this->label_continue = $continue;
    }
    if ($submit != NULL && trim($submit) != "") {
      $this->label_submit = $submit;
    }
    if ($delete != NULL && trim($delete) != "") {
      $this->label_delete = $delete;
    }
  }

  /**
   * Sets weights for form control elements.
   *
   * @param int $back
   *   Back button weight.
   * @param int $continue
   *   Continue button weight.
   * @param int $submit
   *   Submit button weight.
   * @param int $delete
   *   Delete/reset button weight.
   */
  public function setControlsWeights($back, $continue, $submit, $delete) {
    $this->weight_back = $back;
    $this->weight_continue = $continue;
    $this->weight_submit = $submit;
    $this->weight_delete = $delete;
  }

  /**
   * Sets label displayed above steps buttons.
   *
   * @param string $steps_label
   *   Label text.
   */
  public function setStepsLabel($steps_label) {
    $this->steps_label = $steps_label;
  }

  /**
   * Gets steps buttons label.
   *
   * @return string
   *   Label text.
   */
  public function getStepsLabel() {
    return $this->steps_label;
  }

}
