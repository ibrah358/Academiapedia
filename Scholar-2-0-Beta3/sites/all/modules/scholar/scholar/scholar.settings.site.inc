<?php 
require_once(drupal_get_path('module', 'vsite_design')  .'/includes/vsite_design.settings.site.inc');

/**
 * Provides theme selection for each site
 */
class scholar_setting_site extends vsite_design_setting_site implements space_setting {

  function form($space, $value = array()) {
    $form = array('#title'=>'Site Information' );
    
    $s_def_headline = isset($value['scholar_headline'])?$value['scholar_headline']:false;
    $s_def_subheadline = isset($value['sub_heading'])?$value['sub_heading']:false;
    
    $content_profile = array();
    if(!$s_def_headline || !$s_def_subheadline){
    	$content_profile = content_profile_load('vsite_users_profile',$space->group->uid);
    }
    
    
    if(!$s_def_headline && is_array($content_profile->vsite_users_last_name)){
    	$s_def_headline = "{$content_profile->vsite_users_first_name[0]['value']} {$content_profile->vsite_users_last_name[0]['value']}";
    }
    
    if(!$s_def_subheadline  && is_array($content_profile->vsite_users_title)){
      $s_def_subheadline = $content_profile->vsite_users_title[0]['value'];
    }
    
    $form['scholar_headline'] = array(
      '#type' => 'textfield',
      '#title' => t('Main Headline'),
      '#default_value' => $s_def_headline,
      '#size' => 60,
      '#description' => 'Usually: First and Last name ex.(John Q. Smith)',
      '#maxlength' => 128,
      '#required' => TRUE,
      '#attributes' => array('onChange' => '$("#site_example .headline").val(this.value);'),
    );
    $form['scholar_link_to_home'] = array(
      '#type' => 'checkbox',
      '#title' => t('Link to Home Page'),
      '#default_value' => isset($value['scholar_link_to_home'])?$value['scholar_link_to_home']:1,
    );
    $form['scholar_sub_heading'] = array(
      '#type' => 'textarea',
      '#title' => t('Sub Headline'),
      '#default_value' => $s_def_subheadline,
      '#size' => 60,
      '#description' => 'Usually: Title  ex.(Director of Mathematics)',
      '#maxlength' => 128,
      '#required' => TRUE,
      '#attributes' => array('onChange' => '$("#site_example .sub_heading").val(this.value);'),
    );
    $form['scholar_sub_heading_extra'] = array(
      '#type' => 'textfield',
      '#title' => t('Address Line'),
      '#default_value' => isset($value['scholar_sub_heading_extra'])?$value['scholar_sub_heading_extra']:'',
      '#size' => 60,
      '#description' => 'Usually: Your Address ex.(Cambridge, MA 02118)',
      '#maxlength' => 128,
      '#required' => FALSE,
      '#attributes' => array('onChange' => '$("#site_example .sub_heading_extra").val(this.value);'),
    );
    $form['scholar_link_to_contact'] = array(
      '#type' => 'checkbox',
      //'#attributes' => array('disabled' => 'disabled'), //disable till we get og_contacts in place
      '#title' => t('Add Link to contact form?'),
      '#default_value' => isset($value['scholar_link_to_contact'])?$value['scholar_link_to_contact']:1,
    );
  
    return $form;
  }
  
  function submit($space, $value) {

    $value['headline'] = $value['scholar_link_to_home']?"<a href=\"".url("home")."\">{$value['scholar_headline']}</a>":$value['scholar_headline'];
    $value['sub_heading_extra'] = $value['scholar_sub_heading_extra'];
   
    $value['sub_heading'] = trim($value['scholar_sub_heading']);
    $value['sub_heading'] = (strstr("\n",$value['sub_heading']))? "<span>".str_replace("\n","</span><span>",$value['sub_heading'])."</span>":$value['sub_heading'];
    
    $space->group->title = $value['scholar_headline'];
    $space->group->og_description = $value['sub_heading'];
    node_save($space->group);
    
    return $value;
  }
  
}