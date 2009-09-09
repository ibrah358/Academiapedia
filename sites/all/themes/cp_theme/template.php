<?php

/**
* Hack a bit the attachment fieldset.
*/
function cp_theme_upload_form_new($form) {
 unset($form['new']['upload']['#title']);
 unset($form['new']['upload']['#description']);
 drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');
 return drupal_render($form);
}

/**
* Massive hack of the upload form.
*/
function cp_theme_upload_form_current(&$form) {
 drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');

 foreach (element_children($form) as $key) {
   // Add class to group weight fields for drag and drop.
   $form[$key]['weight']['#attributes']['class'] = 'upload-weight';
   $row = array('');
   $output = '';
   // Description: we save the URL, remove it as a description and change the size of the input
   $url = $form[$key]['description']['#description'];
   unset($form[$key]['description']['#description']);
   $form[$key]['description']['#size'] = 20;
   $form[$key]['description']['#attributes'] = array('class' => 'rename');
   $output .= drupal_render($form[$key]['description']);
   // Size & URL
   $output .= '<span class="details">'. drupal_render($form[$key]['size']) .' - '. $url .'</span>';
   $row[] = array(
     'data' => $output,
     'class' => 'file container-inline'
   );
   // Remove
   $form[$key]['remove']['#attributes'] = array('class' => 'remove');
   $form[$key]['remove']['#suffix'] = ' '. t('Remove');
   $row[] = array(
     'data' => drupal_render($form[$key]['remove']),
     'class' => 'remove container-inline'
   );
   // List
   $form[$key]['list']['#suffix'] = ' '. t('List');
   $row[] = array(
     'data' => drupal_render($form[$key]['list']),
     'class' => 'list container-inline'
   );
   // Weight
   $row[] = drupal_render($form[$key]['weight']);
   // Add the extension as a class for styling
   $extension = strtolower(substr(strrchr($form[$key]['filename']['#value'], '.'), 1));
   $rows[] = array('data' => $row, 'class' => 'draggable mime-'. $extension);
 }
 $output = theme('table', array(), $rows, array('id' => 'upload-attachments'));
 $output .= drupal_render($form);
 return $output;
}

/**
* Theme the attachments output.
*/
function cp_theme_upload_attachments($files) {
 $items = array();
 foreach ($files as $file) {
   $file = (object)$file;
   if ($file->list && empty($file->remove)) {
     $extension = strtolower(substr(strrchr($file->filename, '.'), 1));
     $href = file_create_url($file->filepath);
     $text = $file->description ? $file->description : $file->filename;
     $items[] = array(
       'data' => l($text, $href) .' - '. format_size($file->filesize),
       'class' => 'mime-'. $extension,
     );
   }
 }
 if (count($items)) {
   return theme('item_list', $items, $title = NULL, $type = 'ul', array('class' => 'attachment-list', 'id' => 'attachments'));
 }
}

/**
 * Form theming for the block customizer settings form.
 * 
 * Overridden to remove tabledrag and the weights from this customizer
 */
function cp_theme_spaces_block_customizer_settings_form($form) {
  // Add draggable weights
  drupal_add_js('misc/tableheader.js');
  drupal_add_css(drupal_get_path('module', 'spaces') .'/spaces.css');
  $output = '';

  $contexts = element_children($form['contexts']);
  foreach ($contexts as $identifier) {
    $output .= "<div class='spaces-block-customizer clear-block'>";

    // Add a context heading if there is more than 1 context in this feature
    if (count($contexts) > 1) {
      $output .= "<h3>{$form['contexts'][$identifier]['#title']}</h3>";
    }

    // List of block regions that should force an empty display
    $force_empty = array('content');
    global $theme_key;
    init_theme();
    $regions = system_region_list($theme_key);
    foreach ($force_empty as $region) {
      if (empty($form['contexts'][$identifier][$region]) && !empty($regions[$region])) {
        $output .= "<div class='region-{$region}'>";
        $output .= "<strong class='region'>{$regions[$region]}</strong>";
        $output .= "<div class='spaces-empty'>". t('There are no options available for this region.') ."</div>";
        $output .= "</div>";
      }
    }

    foreach (element_children($form['contexts'][$identifier]) as $a) {
      $rows = array();
      foreach (element_children($form['contexts'][$identifier][$a]) as $b) {
        $form['contexts'][$identifier][$a][$b]['weight']['#type'] = "hidden";
        $row = array(
          'status' => drupal_render($form['contexts'][$identifier][$a][$b]['status']),
          'title' => array('data' => drupal_render($form['contexts'][$identifier][$a][$b]['subject']).drupal_render($form['contexts'][$identifier][$a][$b]['weight']), 'class' => 'fill'),
        );
        $rows[] = array('data' => $row);
      }
      $output .= "<div class='region-{$a}'>";
      $output .= "<strong class='region'>{$form['contexts'][$identifier][$a]['#title']}</strong>";
      $output .= theme('table', array(), $rows, array('id' => "spaces-customizer-blocks-{$identifier}-{$a}"));
      $output .= "</div>";
    }

    $output .= "</div>";
  }

  $output .= drupal_render($form);
  return $output;
}

/**
 * Form theme function for customization items.
 * 
 * Overridden: So that they remain in fieldsets
 */
function cp_theme_spaces_customize_item($form) {
  $output = '';
  $rows = array();
  foreach (element_children($form) as $element) {
    if ($form[$element]['#type'] == 'fieldset') {
      $title = $form[$element]['#title'];
      unset($form[$element]['#title']);
      //unset($form[$element]['#type']);
      $rows[] = array(
        "<strong>$title</strong>",
        "<div class='fieldset-wrapper'>".drupal_render($form[$element])."</div>",
      );
    }
  }
  $output .= theme('table', array(), $rows);
  return $output;
}
