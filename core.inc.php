<?php 

/**
 * Display the web agency home page
 * 
 * @return void
 */
function agence_web_home_page()
{
	$content =  array(
		'raw_markup' => array(
			'#type' => 'markup', 
			'#content' => '<h3>Agence WEB</h3>'
		)
	);
	return $content;
}




/**
 * Tgestion des protocoles
 *
 * @return void
 * 
 */
function agence_web_gestion_protocoles()
{
	$content =  array(
		'raw_markup' => array(
			'#type' => 'markup', 
			'#content' => '<h3>Gestion des clients</h3>'
		)
	);
	return $content;
}


/**
 * Gestion des pack sites
 *
 * @return void
 * 
 */
function agence_web_gestion_packs()
{

	$contenu['content'] =  array(
		'#markup' => "<ul class='action-links'><li>" . l(t('CREER UN PACK'), 'admin/agence-web/packs/creer') ."</li></ul>"
	);
	return $contenu;
}


/**
 * Création du formulaire pour les packs
 * @return array
 */
function formulaire_creer_pack($form, &$form_state)
{
	// création d'un tableau pour conserver le données du formulaire
	// 
	$form = array();

	// création d'une enveloppe permettant de grouper certains champs du formulaire
	$form['pack'] = array(
		'#type' => 'fieldset',
		'#title' => t('créer un pack site web')
	);

	// ajout du champ titre dans le formulaire
	// 
	$form['pack']['titre'] = array(
		'#type' => 'textfield',
		'#title' => t('titre'),
		'#description' => t('le titre du pack site')
	);

	// ajout du champ délai de livraison dans le formulaire
	// 
	$form['pack']['delai_livraison'] = array(
		'#type' => 'textfield',
		'#title' => t('délai livraison')
	);

	// ajout du champ titre dans le formulaire
	// 
	$form['pack']['prix_hors_taxes'] = array(
		'#type' => 'textfield',
		'#title' => t('prix hors taxes')
	);

	// ajout des boutons de contrôle
	// 
	$form['buttons']['submit'] = array(
		'#type' => 'submit',
		'#value' => t('enregistrer')
	);

	// on renvoie le formulaire sous forme de tableau associatif
	return $form;
}

/**
 * Implements hook_validate() pour le formulaire_creer_pack.
 * 
 */
function formulaire_creer_pack_validate(&$form, &$form_state) 
{

	// on renseigne ici les noms des champs de type texte qui sont requis
	$champs = array('titre', 'delai_livraison', 'prix_hors_taxes');

	foreach ($champs as $champ)  // pour chaque champ on vérifie sa présence
	{
		if (empty($form_state['values'][$champ])) // le champ titre doit être rempli
		{
			form_set_error($champ, t("Le $champ est un champ requis."));
		}
	}

}

/**
 * Gestion des services
 *
 * @return void
 * 
 */
function agence_web_gestion_services()
{
	$content =  array(
		'raw_markup' => array(
			'#type' => 'markup', 
			'#content' => '<h3>Gestion des clients</h3>'
		)
	);
	return $content;
}