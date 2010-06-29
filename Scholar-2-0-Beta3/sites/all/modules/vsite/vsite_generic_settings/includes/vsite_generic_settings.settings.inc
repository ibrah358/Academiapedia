<?php

/**
 * Provides front page settings for each site
 */
class vsite_generic_settings_settings implements space_setting {
	public $weight = 100;

  var $id;
  function __construct($id = NULL) {
    if ($id) {
      $this->id = $id;
    }
    else {
      $this->id = 'generic';
    }
  }

  function form($space, $value = array()) {

    $form = array('#title' => "Other Settings");

    $form['admin_menu'] = array(
		  '#type' => 'checkbox',
		  '#title' => t('Display Admin Menu Bar.'),
      '#description' => "The admin menu bar displays at the top of the page when you are logged in as a site administrator.",
      '#default_value' => (is_array($value) && array_key_exists('admin_menu',$value))?$value['admin_menu']:0,
		);
		
//    $form['contact_form'] = array(
//      '#type' => 'checkbox',
//      '#title' => t('Allow Contact Form'),
//      '#default_value' => is_array($value) && array_key_exists('contact_form',$value) ? $value['contact_form']:1,
//      '#description' => "When this is checked a contact form is displayed on your site allowing others to contact you.",
//    );
    
    $form['contact_form_anonymous'] = array(
      '#type' => 'checkbox',
      '#title' => t('Anonymous Contact'),
      '#default_value' => is_array($value) && array_key_exists('contact_form_anonymous',$value) ? $value['contact_form_anonymous']:1,
      '#description' => "When this is checked a user does not need to be logged in to contact you.",
    );
		
		$s_default_shield = drupal_get_path('module','vsite_generic_settings')."/theme/shields/harvard_shield.png";
    $s_shield_dir = drupal_get_path('module','vsite_generic_settings')."/theme/shields";
    $a_shields = file_scan_directory($s_shield_dir, '.*', array('.', '..'));
    
    $shield_options = array();
    
    foreach ($a_shields as $shield) {
      $shield_options[$shield->filename] = theme('vsite_design_theme_shieldpicker', $shield); 
    }  
  
    
    $form['disable_shield'] = array(
      '#prefix' => "<br />",
      '#type' => 'checkbox',
      '#title' => t('Disable Shields'),
      '#attributes' => array('onChange' => "vsite_generic_grey_out(this);"),
      '#default_value' => is_array($value) && array_key_exists('disable_shield',$value) ? $value['disable_shield']:0,
      '#description' => "Select the shield that you wish to appear in the 'shield widget' on your site.  Click disable to display none.",
    );
    
    $form['shield'] = array(
      '#prefix' => "<div class='shield_wrapper'>",
      '#title' => t('Shield'),
      '#type' => 'radios',
      '#options' => $shield_options,
      '#default_value' => is_array($value) && array_key_exists('shield',$value) ? $value['shield']:$s_default_shield,
      '#append' => '</div><div class="clearfix"></div>',
    );
    
    return $form;
  }

  function validate($space, $value) {
    return $value;
  }

  function submit($space, $value) {
    return $value;
  }
}
