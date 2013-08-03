<?php 


/**
 * @file
 * 
 *  Agence Web Clients Admin 
 *  
 *  Contient toutes les fonctions permettant d'administrer les clients.
 *
 * @package AgenceWeb
 *
 * @author
 *  Papa Pathe SENE <pathe.sene@gmail.com>
 *  
 * @copyright
 * 	Xarala  
 * 
 * @link http://www.xarala.sn
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
 * @return  
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
    $ligne->lien_afficher = l( t('afficher'),  'admin/agence-web/clients/' . $ligne->id );
    $ligne->lien_modifier = l( t('modifier'),  'admin/agence-web/clients/' . $ligne->id . '/modifier' );
    $ligne->lien_supprimer= l( t('supprimer'), 'admin/agence-web/clients/' . $ligne->id . '/supprimer' );

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
  $table['pager_table'] = array( 
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
 *   Un tableau asscociatif affecté du theme par défaut pour drupal 
 *   pour les tableaux.
 * 
 */
function agence_web_clients_a_recouvrer() {
  $header = array(
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
 * @param $form       
 *   Le formulaire un tableau asociatif contenant les champs à  rajouter dans le formulaire
 *   mais aussi avec pour chaque chaque champ ses parametres qui lui sont propres
 * 
 * @param  $form_state 
 *   Le formulaire avec les données
 * 
 * @return  
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
    '#required'   => TRUE
  );

  $form['tabs']['legal']['ninea'] = array(
    '#type'        => 'textfield',
    '#title'       => t('ninéa'),
    '#description' => t('le numéro identification national des entreprises et des associations.'),
    '#required'    => TRUE,
    '#default'     => isset($form_state['values']['ninea']) ? $form_state['values']['ninea'] : ''
  );

  $form['tabs']['legal']['rc'] = array(
    '#type'        => 'textfield',
    '#title'       => t('numéro de registre de commerce (RC)'),
    '#description' => t('le numéro identification au registre de commerce.'),
    '#required'    => TRUE
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
    '#required'    => TRUE
  );


  $form['tabs']['client']['capital_social'] = array(
    '#type'        => 'textfield',
    '#title'       => t('le capital social'),
    '#description' => t('le patrimoine de la société. Distinct de celui des associés.'),
    '#required'    => FALSE,
    '#default_value'     => !empty($form_state['values']['capital_social']) ? $form_state['values']['capital_social'] : 0
  );

  $form['tabs']['client']['siege_social'] = array(
    '#type'        => 'textarea',
    '#title'       => t('le siège social'),
    '#description' => t('fournir ici une adresse pour le client.'),
    '#required'    => TRUE
  );

  $form['tabs']['client']['objet_social'] = array(
    '#type'        => 'textarea',
    '#title'       => t('objet social de la société'),
    '#description' => t('lister les services et produits fournis par cette société.'),
    '#required'    => TRUE
  );

  /**
   * Troisiéme groupe avec les informations de contact
   */  
  $form['tabs']['contact'] = array(
    '#type'       => 'fieldset',
    '#title'      => t('Informations de contact'),
    '#required'   => TRUE
  );

  $form['tabs']['contact']['telephone'] = array(
    '#type'        => 'textfield',
    '#title'       => t('le numéro de téléphone'),
    '#description' => t('Le numéro de téléphone offfiel de la société.'),
    '#required'    => TRUE
  );

  $form['tabs']['contact']['fax'] = array(
    '#type'        => 'textfield',
    '#title'       => t('le numéro de fax'),
    '#description' => t('Le numéro de fax offfiel de la société.'),
    '#required'    => FALSE
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
   * Il est unique dans la table client
   */
  if (0 == preg_match(variable_get('agence_web_clients_motif_ninea'), $form_state['values']['ninea'])) {
    form_set_error('ninea', "Ce ninéa n'est pas valide!");
  }
  
  // On vérifie l'unicité de ce ninéa
  _valeur_existe_dans_la_table('agence_web_clients', 'ninea', $form_state['values']['ninea']);

  // FIN VALIDATION NINEA --------------------------------------------------------------------

  /**
   * Validation du RC
   * 
   * Exemple de RC valide
   *   SN DKR 2216 B 9989
   */ 
  if (0 == preg_match(variable_get('agence_web_clients_motif_rc'), $form_state['values']['rc'])) {
    form_set_error('rc', "Ce numéro de RC n'est pas valide!");
  }

  // on vérifie l'unicité de ce RC
  _valeur_existe_dans_la_table('agence_web_clients', 'rc', $form_state['values']['rc']);

  // FIN VALIDATION RC -----------------------------------------------------------------------
  

  /**
   * Validation du champ capital social s'il est renseigné par l'utilisateur
   * 
   */ 
  if (!empty($form_state['values']['capital_social'])) {

    if (!is_numeric($form_state['values']['capital_social'])) { // le capital social est un nombre
      form_set_error('capital_social', "Le capital social n'est pas un numérique.");
    }

    $capital_social = variable_get('agence_web_clients_capital_social_minimum');

    if ($form_state['values']['capital_social'] < $capital_social) { // le capital social admet une valeur minimale
      form_set_error('capital_social', "le capital social doit être supérieur à $capital_social");
    }

  }

  /**
   * Validation du numéro de téléphone
   * 
   * Les numéros de téléphones sont composés du préfixe opérateur
   * suivi de sept chiffres décimaux sans espaces.
   * 
   */ 
  if (0 == preg_match(variable_get('agence_web_clients_motif_telephone'), $form_state['values']['telephone'])) {
    form_set_error('telephone', "Ce numéro de téléphone n'est pas valide!");
  }

  _valeur_existe_dans_la_table('agence_web_clients', 'telephone', $form_state['values']['telephone']);

  // FIN VALIDATION TELEPHONE -----------------------------------------------------------

}


/**
 * Implements hook_views_form_submit().
 */
function agence_web_clients_formulaire_creation_submit($form, &$form_state) {	

	db_insert('agence_web_clients')->
  fields(
	  array(
	  	'ninea' => $form_state['values']['ninea'],
	  	'rc'  => $form_state['values']['rc'],
	  	'raison_sociale' => $form_state['values']['raison_sociale'],
	  	'capital_social' => isset($form_state['values']['capital_social']) ? $form_state['values']['capital_social'] : 0,
	  	'siege_social' => $form_state['values']['siege_social'],
	  	'objet_social' => $form_state['values']['objet_social'],
	  	'telephone'    => $form_state['values']['telephone'   ],
	  	// 'courriel'       => $form_state['values']['courriel'],
	  	'fax'   => $form_state['values']['fax']
	  )
	)->
  execute();

	$form_state['redirect'] = 'admin/agence-web/clients';

	drupal_set_message(t('la société a été créee avec succès.'));
}



/**
 * CONFICURATIONS----------------------------------------------------------
 */

/**
 * Configuration du module agence_web_clients
 * 
 * @return  
 *   un tableau associatif contenant les champs d'un formulaire
 * 
 */
function agence_web_clients_formulaire_configuration() {

  /**
   * Champ texte pour configurer le capital social minimum
   */
  $form['agence_web_clients_capital_social_minimum'] = array(    
    '#type'  => 'textfield',
    '#title' => t('le capital social minimum.'),
    '#description'    => t("Le capital social minimum pour une société de capitaux ou une société d'actions."),
    '#required'       => TRUE,
    '#default_value'  => variable_get('agence_web_clients_capital_social_minimum', 1000000)
  );

  $form['tabs']['contact'] = array(
    '#type'       => 'fieldset',
    '#title'      => t('Paramètres de validation'),
    '#required'   => TRUE
  );

  /**
   * Champ texte pour configurer le motif des numéros de téléphone des sociétés
   */
  $form['tabs']['contact']['agence_web_clients_motif_telephone'] = array(
    '#type'  => 'textfield',
    '#title' => t('le motif à utiliser pour valider les numéros de téléphone.'),
    '#description'    => t("Le motif à utiliser pour valider les numéros de téléphone.[Ne changer que si vous êtes expert]"),
    '#required'       => TRUE,
    '#default_value'  => variable_get('agence_web_clients_motif_telephone', '/^(33|70|76|77|78)\d{7}$/')
  );

  /**
   * Champ texte pour configurer le motif du ninea
   */
  $form['tabs']['contact']['agence_web_clients_motif_ninea'] = array(
    '#type'  => 'textfield',
    '#title' => t('le motif à utiliser pour valider le ninea.'),
    '#description'    => t("Le motif à utiliser pour valider les numéros d'identification national des entreprises et des associations.[Ne changer que si vous êtes expert]"),
    '#required'       => TRUE,
    '#default_value'  => variable_get('agence_web_clients_motif_ninea', '/^\d{3,7}$/')
  );

    /**
   * Champ texte pour configurer le motif du ninea
   */
  $form['tabs']['contact']['agence_web_clients_motif_rc'] = array(
    '#type'  => 'textfield',
    '#title' => t('le motif à utiliser pour valider le rc.'),
    '#description'    => t("Le motif à utiliser pour valider les numéros de registre de commerce.[Ne changer que si vous êtes expert]"),
    '#required'       => TRUE,
    '#default_value'  => variable_get('agence_web_clients_motif_rc', '/^SN DKR \d{4} [A-Z]{1} \d{4}$/')
  );


  return system_settings_form($form);

}


/**
 * Affiche les détails du client
 *
 * @param integer $id_client
 *  un entier qui représente  l'identifiant d'une ligne dans la table agence_web_clients
 *
 * @return
 *  un tableau associant avec le contenu de la page
 *
 */
function agence_web_details_client($id_client) {
  
  $content = array();

  $requete = db_select('agence_web_clients', 'c')
              ->fields('c', array('raison_sociale', 'ninea', 'rc', 'id'))
              ->condition('c.id', $id_client);  

  $client  = $requete->execute()->fetchObject();




  $content['raw_markup'] = array(
    '#type' => 'markup',
    '#markup' => $client->raison_sociale
  );

  return $content;
}

/**
 * Permet de supprimer un client de la base
 *
 *
 * @param integer $id_client
 *  un entier qui représente  l'identifiant d'une ligne dans la table agence_web_clients
 *
 */
function agence_web_supprimer_client($id_client)
{

  // on vérifie si un client avec comme id $id_client existe

  $client_request = db_select('agence_web_clients', 'c')
                    ->fields('c', array('id'))
                    ->condition('c.id', $id_client);

  $client_result = $client_request->execute()->fetchObject();

  if ($client_result) { // si ce client est présent dans la base de données
    // construction et exécution de la requête de suppression
    db_delete('agence_web_clients')
      ->condition('id', $id_client)
      ->execute();


    // affichage d'un message de succès
    drupal_set_message(t("un client vient d'être supprimé de la base de données."), 'status', FALSE);

    // redirection vers la  page de gestion des clients
    drupal_goto('admin/agence-web/clients', array());  
  }
  else { // sinon

    // affichage d'un message d'erreur
    drupal_set_message(t("le client avec l'identidiant {$id_client} n'existe pas."), 'error', FALSE);

    // redirection vers la  page de gestion des clients
    drupal_goto('admin/agence-web/clients', array());  
  }

}


function agence_web_modifier_client($id_client)
{
  $content['raw_markup'] = array(
    '#type' => 'markup',
    '#markup' => 'TODO: modifier le client.'
  );

  return $content;
}

function agence_web_projets_client($id_client)
{
  $content['raw_markup'] = array(
    '#type' => 'markup',
    '#markup' => 'TODO: afficher les projets réalisés ou en cours pour ce client.'
  );

  return $content;
}

/**
 * Cette fonction vérifie une contrainte d'unicité dans la table
 *  
 * La fonction vérifie dans la table donnée en argument que
 * $value n'existe pas encore pour le champ $field.
 * 
 * @param $table
 *   La table où vérifier la contrainte d'unicité
 * 
 * @param $field
 *   Le champ à vérifier
 * 
 * @param $value
 *   La valeur à chercher dans la table
 */
function _valeur_existe_dans_la_table($table, $field, $value)
{

  $requete  = db_select($table, 'c');

  $resultat = $requete
       ->fields('c', array($field))
       ->condition("c.$field", $value)
       ->execute();
  /**
   * Si le nombre de lignes retourné par la requête est différent de zéro
   * On ajoute une notification d'erreur dans le formulaire.
   */ 
  if (count($resultat->fetchAll()) != 0) { 
    form_set_error($field, check_plain("Ce $field existe déjà dans la table."));
  }

}
