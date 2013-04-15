<?php 


/**
 * @file
 * 
 *  Agence Web Clients Admin 
 *  
 *  Contient toutes les fonctions permettant d'administrer les clients.
 *
 * @author
 *  Papa Pathe SENE <pathe.sene@gmail.com>
 *  
 * @copyright
 * 	Xarala  
 * 
 * 
 */

/**
 * Page d'accueil de la gestion des clients
 * 
 * Selectionne les clients depuis la base, et les affiche 
 * sous forme tabulée avec pagination s'il y a lieu.
 *
 * @see agence_web_clients_menu()
 * 
 * @return array 
 * Un tableau associatif themé avec la fonction theme_table().
 *  
 */
function agence_web_gestion_clients() {

  $header = array
  (
    array('data' => t('raison sociale'), 'field' => 'c.raison_sociale'),
    array('data' => t('ninéa'), 'field' => 'c.ninea'),
    array('data' => t('n° registre de commerce'), 'field' => 'c.rc'),
    array('data' => t('actions'), 'colspan' => '3')
  );

  /**
   * On crée un objet requete
   * 
   * Celui étend les classes de pagination et de tri de Drupal
   */ 
  $requete  = db_select('agence_web_clients', 'c')->extend('Tablesort')->extend('PagerDefault');

  /**
   * On selectionne les champs à afficher
   */ 
  $requete->fields('c', array('raison_sociale', 'ninea', 'rc', 'id'));

  /**
   * On inclue les critéres de tri renseignés par l'utilisateur sinon les défauts.
   */ 
  $resultat = $requete
                ->orderByHeader($header)
                ->limit(15)
                ->execute();  

  $lignes = array();
  
  foreach ($resultat as $ligne) {

    /**
     * Création des liens vers le contenu et les actions qui y sont associées
     *   -- affichage
     *   -- edition
     *   -- suppression
     */ 
    $ligne->lien_afficher = l( t('afficher'),  '/agence-web/clients/'. $ligne->id );
    $ligne->lien_modifier = l( t('modifier'),  '/agence-web/clients/'. $ligne->id .'/modifier' );
    $ligne->lien_supprimer= l( t('supprimer'), '/agence-web/clients/'. $ligne->id .'/supprimer' );

    /**
     * On supprime le champ id du résultat et ainsi il n'est pas affiché.
     * On en avait juste besoin pour pouvoir afficher des liens vers le contenu.
     */
    unset($ligne->id);  

    /**
     * On ajoute la ligne courante dans le tableau à afficher.
     */ 
    $lignes[] = array('data' => (array) $ligne);
  }

  /**
   * On crée un tableau de rendu
   */ 
  $table['pager_table'] = array ( 
    '#theme'  => 'table',
    '#header' => $header, 
    '#rows'   => $lignes,
    '#empty'  => t('La table clients est vide. Cliquez sur le lien nouveau client pour ajouter un client.')
  );

  $table['pager'] = array('#theme' => 'pager');

  return $table;
  
}





/**
 * Page qui affiche les clients à recouvrer
 * 
 * @return array 
 * Un tableau asscociatif affecté du theme par défaut pour drupal 
 * pour les tableaux.
 * 
 */
function agence_web_clients_a_recouvrer() {
  $header = array
  (
    t('raison sociale'), 
    t('date échéance'), 
    t('montant restant à recouver'),
  );

  $table = theme('table', array('header' => $header, 'rows' => array()));

  return $table;
}


/**
 * Formulaire de création dun client 
 * 
 * Dans ce formulaire, nous allons ajouter tous les champs contenant des renseignements de 
 * la table sociétés.
 * 
 * @param  array $form       
 *   Le formulaire un tableau asociatif contenant les champs à  rajouter dans le formulaire
 *   mais aussi avec pour chaque chaque champ ses parametres qui lui sont propres
 * 
 * @param  array $form_state 
 *   Le formulaire avec les données
 * 
 * @return array $form           
 *   Un tableau associatif contenant le formulaire, ses attributs et ses champs
 */
function agence_web_clients_formulaire_creation($form, &$form_state) {
  $form = array();

  $form['tabs'] = array(
    '#type'       => 'vertical_tabs',
    '#title'      => t('Créer un nouveau client pour votre agence web')
  );

  /**
   * Groupe de champs avec les informations légales
   */ 

  $form['tabs']['legal'] = array(
    '#type'       => 'fieldset',
    '#title'      => t('Informations légales'),
    '#required'   => true
  );


  $form['tabs']['legal']['ninea'] = array(
    '#type'        => 'textfield',
    '#title'       => t('ninéa'),
    '#description' => t('le numéro identification national des entreprises et des associations.'),
    '#required'    => true,
    '#default'     => isset($form_state['values']['ninea']) ? $form_state['values']['ninea'] : ''
  );


  $form['tabs']['legal']['rc'] = array(
    '#type'        => 'textfield',
    '#title'       => t('numéro de registre de commerce (RC)'),
    '#description' => t('le numéro identification au registre de commerce.'),
    '#required'    => true
  );


  /**
   * Groupe de champs concernant les infos de base de la société
   */
  $form['tabs']['client'] = array(
    '#type'       => 'fieldset',
    '#title'      => t('Informations sociales')
  );

  $form['tabs']['client']['raison_sociale'] = array(
    '#type'        => 'textfield',
    '#title'       => t('raison sociale'),
    '#description' => t('Quel est la dénomination du client.'),
    '#required'    => true
  );


  $form['tabs']['client']['capital_social'] = array(
    '#type'        => 'textfield',
    '#title'       => t('le capital social'),
    '#description' => t('le patrimoine de la société. Distinct de celui des associés.'),
    '#required'    => false
  );

  $form['tabs']['client']['siege_social'] = array(
    '#type'        => 'textarea',
    '#title'       => t('le siège social'),
    '#description' => t('fournir ici une adresse pour le client.'),
    '#required'    => true
  );

  $form['tabs']['client']['objet_social'] = array(
    '#type'        => 'textarea',
    '#title'       => t('objet social de la société'),
    '#description' => t('lister les services et produits fournis par cette société.'),
    '#required'    => true
  );


  /**
   * Troisiéme groupe avec les informations de contact
   */  
  $form['tabs']['contact'] = array(
    '#type'       => 'fieldset',
    '#title'      => t('Informations de contact'),
    '#required'   => true
  );

  $form['tabs']['contact']['telephone'] = array(
    '#type'        => 'textfield',
    '#title'       => t('le numéro de téléphone'),
    '#description' => t('Le numéro de téléphone offfiel de la société.'),
    '#required'    => true
  );

  $form['tabs']['contact']['fax'] = array(
    '#type'        => 'textfield',
    '#title'       => t('le numéro de fax'),
    '#description' => t('Le numéro de fax offfiel de la société.'),
    '#required'    => false
  );


  // ajout des boutons de contrôle
  // 
  $form['buttons']['submit'] = array(
    '#type'        => 'submit',
    '#value'       => t('enregistrer')
  );

  return $form;
}

/**
 * Implements hook_validate().
 * 
 */
function agence_web_clients_formulaire_creation_validate($form, &$form_state) {

  /**
   * Validation du NINEA
   * 
   * Le ninea est compose de 3 à 7 chiffres
   */
  if (0 == preg_match('/^\d{3,7}$/', $form_state['values']['ninea'])) {
    form_set_error('ninea', "Ce ninéa n'est pas valide!");
  }

  /**
   * Validation du RC
   * 
   * Exemple de RC valide
   *   SN DKR 2216 B 9989
   */ 
  if (0 == preg_match('/^SN DKR \d{4} [A-Z]{1} \d{4}$/', $form_state['values']['rc'])) {
    form_set_error('rc', "Ce numéro de RC n'est pas valide!");
  }

  /**
   * Validation du champ capital social s'il est renseigné par l'utilisateur
   * 
   */ 
  if (!empty($form_state['values']['capital_social'])) {

    if (!is_numeric($form_state['values']['capital_social'])) { // le capital social est un nombre
      form_set_error('capital_social', "Le capital social n'est pas un numérique.");
    }

    if ($form_state['values']['capital_social'] < 500000) { // le capital social admet une valeur minimale
      form_set_error('capital_social', "le capital social doit être supérieur à 500000");
    }

  }

  /**
   * Validation du numéro de téléphone
   * 
   * Les numéros de téléphones sont composés du préfixe opérateur
   * suivi de sept chiffres décimaux sans espaces.
   * 
   */ 
  if (0 == preg_match('/^(33|70|76|77|78)\d{7}$/', $form_state['values']['telephone'])) {
    form_set_error('telephone', "Ce numéro de téléphone n'est pas valide!");
  }

}


/**
 * Implements hook_views_form_submit().
 */
function agence_web_clients_formulaire_creation_submit($form, &$form_state) {	

	db_insert('agence_web_clients')
	->fields(
	  array(
	  	'ninea'          => $form_state['values']['ninea'],
	  	'rc'             => $form_state['values']['rc'],
	  	'raison_sociale' => $form_state['values']['raison_sociale'],
	  	'capital_social' => $form_state['values']['capital_social'],
	  	'siege_social'   => $form_state['values']['siege_social'],
	  	'objet_social'   => $form_state['values']['objet_social'],
	  	'telephone'      => $form_state['values']['telephone'   ],
	  	// 'courriel'       => $form_state['values']['courriel'],
	  	'fax'            => $form_state['values']['fax']
	  )
	)
	->execute();

	$form_state['redirect'] = 'admin/agence-web/clients';

	drupal_set_message(t('la société a été créee avec succès.'));
}

