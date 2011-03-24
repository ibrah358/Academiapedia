<?php

/**
 * Vsite space object
 *
 * @author rbrandon
 * @todo verify that the menu_access workflow is fine (code at bottom of page)
 */
class vsite extends space_og {

  //Set to allow feature access even if it is not enabled
  public static $override_allow_feature_access = false;
  
  //Store the current provider in our multi-provider system
  public $purl_provider = false;
  
  function feature_options() {
    return array(0 => t('Disabled'), 1 => t('Public'), 2 => 'Private');
  }
  
  /**
   * Return the absolute URL to this vsite
   */
  public function get_absolute_url($path = ""){
    
    return url($path, array( 'purl' => array( 'provider' => $this->purl_provider,'id' => $this->id), 'absolute' => true));
    
  }
  
  /**
   * Override of activate().
   */
  function activate() {
    if (parent::activate()) {
      
	    if ($this->group->og_theme && !$this->group->theme) {
	      $this->group->theme = $this->group->og_theme;
	    }//This must be set on load so that when you save the value will persist

      return TRUE;
    }
    return FALSE;
  }
  
  /**
    * Implementation of space->router().
    */
  public function router($op, $object = NULL) {
  	//Fixes bug where when menu's are loaded any menu that points to a node causes redirection, leaving this not so eligant code in place till the following is resolved
  	// http://drupal.org/node/936234
  	if(arg(0) == 'admin') return;
  	
  	global $user;
    switch ($op) {
    	default:
	    	$a_generic_settings = $this->controllers->variable->get('vsite_setting_generic');
	      if ($this->active && $a_generic_settings['private_vsite'] && !og_is_group_member($this->id)) {
	        $this->redirect('private');
	      }
      break;
    }
    parent::router($op, $object);
  }

  public function redirect($op = 'home'){
    switch ($op) {
      case 'home' :
        if (! empty($this->purl)) {
        	
        	$home = 'home';  //Hardcoded for now
        	
          // Use the menu path of the selected feature as homepage
          if (menu_get_item($home)) {
            purl_goto($home, array( 'purl' => array( 'provider' => $this->purl_provider,'id' => $this->id)));
            break; //if we get here we have an infinate redirect situation and are on the correct page
          }

          // The previous checks fail, there is no valid homepage set
          if ($this->admin_access() && user_access('configure spaces features')) {
            drupal_set_message(t("Please setup your group by choosing a homepage setting."));
            purl_goto("cp/settings/features", array( 'purl' => array('provider' => $this->purl_provider, 'id' => $this->id) ));
          }
        }
        else {
          drupal_goto('node/' . $this->id . '/edit');
        }
        menu_set_active_item('spaces-access-denied');
        break;
      case 'features' :
        purl_goto("cp/settings/features", array('purl' => array( 'provider' => $this->purl_provider, 'id' => $this->id)));
        break;
      case 'private':
      	purl_goto("private_site", array('purl' => array( 'provider' => $this->purl_provider, 'id' => $this->id)));
      	break;
    }
  }
  
	/**
	 * Returns the vocabularies associated with a vsite.
	 * If a content type is provided, return vocabs associated
	 * with that content type only.
	 * @param $node_type
	 * @return array
	 */
	public function get_vocabs($node_type = NULL){
	  
	  if ($this->group) {
	    
	    if (isset($this->group->og_vocabularies)) {
	      if (!$node_type) {
	        return $this->group->og_vocabularies;
	      }
	      
	      $result = array();
	      
	      foreach ( $this->group->og_vocabularies as $v ) {
	        if (in_array($node_type, array_keys($v->nodes))) {
	          $result[$v->vid] = $v;
	        }
	      }
	      
	      return $result;
	    }
	  }
	}
	
  /**
   * Override of access_feature().
   */
  function access_feature($op = 'view', $feature, $account = NULL) {
    
    if(self::$override_allow_feature_access){
      //allow access to all feature menu items on this page for display purposes
      //Used in CP so that all features menu entries will be displayed for sorting
      return true;
    }
    
    if($feature == 'vsite' && user_access('access content')) $access = true;
    
    if(!$access) $access = parent::access_feature($op, $feature, $account);

    return $access;
  }
	
	/**
	 *
	 * @param $op
	 * @param $feature
	 * @param $account
	 */
  function privacy_access_feature($privacy = array(FEATURE_PUBLIC), $feature, $account = NULL) {
    $access = $this->access_feature('view', $feature, $account);

    if(!$access) return $access;

    return $access;
  }
  
  /**
   * If this space is loaded outside a active context (ie) externally from the home page
   * then determine which purl_provider would be best
   *
   * domain if they have that set otherwise path
   */
  protected function verify_purl() {
    
    $domain_method = variable_get('purl_method_vsite_domain',false);
    $elements = purl_active()->get($domain_method);
    foreach ($elements as $element) {
      if ($element->provider == "vsite_domain" && $element->id == $this->id) {
        $this->purl_provider = $domain_method;
        return TRUE;
      }
    }
    
    $method = variable_get("purl_method_spaces_{$this->type}", 'path');
    $elements = purl_active()->get($method);
    foreach ($elements as $element) {
      if ($element->provider == "spaces_{$this->type}" && $element->id == $this->id) {
        $this->purl_provider = $method;
        return TRUE;
      }
    }
    
    //No provider loaded
    return FALSE;
  }
  
  
}