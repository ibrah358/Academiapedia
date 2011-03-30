<?php
function scholar_publication_settings($space) {

  include_once drupal_get_path('module', 'biblio') .'/biblio.admin.inc';

  $s_style = biblio_get_style();

  $form['biblio_no_year_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Unpublished Label'),
    '#default_value' => variable_get('biblio_no_year_text', t('Submitted')),
    '#description' => t('This label will show if you do not enter a publication date.'),
  );

  $form['biblio_style'] = array(
    '#type' => 'radios',
    '#title' => t('Select your preferred bibliographic format'),
    //'#description' => t('This alters the bibliographic format of your publications.'),
    '#default_value' => $s_style,
    '#options' => _biblio_admin_get_styles(),
  );

  $form['biblio_export_links'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select which export formats should accompany each bibliographic record'),
    '#default_value' => variable_get('biblio_export_links', array('tagged', 'xml', 'bibtex')),
    '#options' => array(
      'tagged'  => t('EndNote Tagged'),
      'xml'     => t('EndNote XML'),
      'bibtex'  => t('BibTex'),
    ),
    '#description' => t('Each selected format will appear in its own export link.'),
  );

  $form['biblio_google_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link to Google Scholar'),
    '#return_value' => 1,
    '#default_value' => variable_get('biblio_google_link', 1),
    '#description' => t('This adds the necessary metadata to your web site to allow Google Scholar to properly index your publication.'),
  );

  $form['scholar_publications_note_in_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the "note" content in teaser?'),
    //'#description' => t('This alters the bibliographic format of your publications.'),
    '#default_value' => variable_get('scholar_publications_note_in_teaser',false),
    '#description' => t('If this is checked and you add a note to a publication then it will show as part of a publications teaser.'),
  );

  $form['scholar_publications_external_link_name'] = array(
    '#type' => 'textfield',
    '#title' => t('External Link Name'),
    '#description' => t('What to call links to resources not on this page.'),
    '#default_value' => variable_get('scholar_publications_external_link_name', 'Website'),
  );

  $form['biblio_rowsperpage'] = array(
    '#type' => 'textfield',
    '#title' => t('Publications per page'),
    '#size' => 5,
    '#default_value' => variable_get('biblio_rowsperpage',25),
    '#description' => t('Select the number of publication to be displayed on each page.'),
  );

  $form['biblio_sort'] = array(
    '#type' => 'radios',
    '#title' => t('By default, sort publications by which category'),
    '#default_value' => variable_get('biblio_sort','year'),
    '#options' => array('author' => t('Author'), 'keyword' => t('Keyword'), 'title' => t('Title'), 'type' => t('Type'), 'year' => t('Year')),
  );

  $form['biblio_order'] = array(
    '#type' => 'radios',
    '#title' => t('Select default order of the sort category'),
    '#default_value' => variable_get('biblio_order', 'DESC'),
    '#options' => array('DESC' => t('Descending'), 'ASC' => t('Ascending')),
  );

  $form['biblio_sort_tabs'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Allow additional sorting of your publications by'),
    '#default_value' => variable_get('biblio_sort_tabs', array('author'=>'author', 'title'=>'title', 'type'=>'type', 'year'=>'year')),
    '#options' => array('author' => t('Author'), 'title' => t('Title'), 'type' => t('Type'), 'year' => t('Year')),
    //'#description' => t('You turn the sorting links at the top of the /biblio page here.'),
  );

  $form['bkn'] = array(
    '#type' => 'fieldset',
    '#title' =>t('Author Lookup Services'),
  );

  // check these with others

   $form['bkn']['bkn_allow_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Autocomplete'),
    '#default_value' => variable_get('bkn_allow_autocomplete',1),
    '#return_value' => 1,
    '#description' => t('Should contributor field use autocomplete?')
  );
  $form['bkn']['bkn_mode'] = array(
    '#type' => 'radios',
    '#title' => t('How should the data be retrieved from BKN?'),
    '#default_value' => variable_get('bkn_mode', 2),
    '#options' => array(t('Lookup via autocomplete (many requests to bkn)'), t('User Initiated (single request)'), t('Disabled')),
  );

  return system_settings_form($form);
}