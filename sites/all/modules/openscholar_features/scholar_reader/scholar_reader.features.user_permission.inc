<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function scholar_reader_user_default_permissions() {
  $permissions = array();

  // Exported permission: create feed content
  $permissions['create feed content'] = array(
    'name' => 'create feed content',
    'roles' => array(
      '0' => 'scholar admin',
      '1' => 'scholar user',
    ),
  );

  // Exported permission: delete own feed content
  $permissions['delete own feed content'] = array(
    'name' => 'delete own feed content',
    'roles' => array(
      '0' => 'scholar admin',
      '1' => 'scholar user',
    ),
  );

  // Exported permission: edit own feed content
  $permissions['edit own feed content'] = array(
    'name' => 'edit own feed content',
    'roles' => array(
      '0' => 'scholar admin',
      '1' => 'scholar user',
    ),
  );

  // Exported permission: override feed promote to front page option
  $permissions['override feed promote to front page option'] = array(
    'name' => 'override feed promote to front page option',
    'roles' => array(
      '0' => 'scholar admin',
      '1' => 'scholar user',
    ),
  );

  // Exported permission: override feed published option
  $permissions['override feed published option'] = array(
    'name' => 'override feed published option',
    'roles' => array(
      '0' => 'scholar admin',
      '1' => 'scholar user',
    ),
  );

  // Exported permission: override feed revision option
  $permissions['override feed revision option'] = array(
    'name' => 'override feed revision option',
    'roles' => array(),
  );

  // Exported permission: override feed sticky option
  $permissions['override feed sticky option'] = array(
    'name' => 'override feed sticky option',
    'roles' => array(
      '0' => 'scholar admin',
      '1' => 'scholar user',
    ),
  );

  return $permissions;
}