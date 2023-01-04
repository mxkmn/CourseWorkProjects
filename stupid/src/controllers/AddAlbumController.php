<?php

require_once '../src/controllers/BaseController.php';

class AddAlbumController extends BaseController {
	protected $template = "add_album.twig";
	protected $page_title = "Добавление альбома";

	public function __construct($twig_instance, $data_instance, $params) {
		parent::__construct($twig_instance, $data_instance, $params);

		$this->data_instance->auth_or_exit();
	}

	protected function on_post($context) {
		$id = $this->data_instance->add_album($_POST['artist'], $_POST['name'], $_POST['year'], $_POST['rating']);

		$tmp_name = $_FILES['cover']['tmp_name'];
		move_uploaded_file($tmp_name, "../public/pics/album/$id.jpg");

		$context['album_id'] = $id;
		return $context;
	}
}

?>