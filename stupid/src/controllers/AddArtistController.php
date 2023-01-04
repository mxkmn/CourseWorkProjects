<?php

require_once '../src/controllers/BaseController.php';

class AddArtistController extends BaseController {
	protected $template = "add_artist.twig";
	protected $page_title = "Добавление исполнителя";

	public function __construct($twig_instance, $data_instance, $params) {
		parent::__construct($twig_instance, $data_instance, $params);

		$this->data_instance->auth_or_exit();
	}

	protected function on_post($context) {
		$id = $this->data_instance->add_artist($_POST['name']);

		$tmp_name = $_FILES['cover']['tmp_name'];
		move_uploaded_file($tmp_name, "../public/pics/artist/$id.jpg");

		$context['artist_id'] = $id;
		return $context;
	}
}

?>