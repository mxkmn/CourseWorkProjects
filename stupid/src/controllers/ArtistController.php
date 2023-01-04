<?php

require_once '../src/controllers/BaseController.php';

class ArtistController extends BaseController {
	protected $artist_id = null;

	public function __construct($twig_instance, $data_instance, $params) {
		parent::__construct($twig_instance, $data_instance, $params);

		$this->template = $this->get_template();

		if ($this->template == "delete_artist.twig") {
			$this->data_instance->delete_artist_by_id($this->params["artist_id"]);
			echo "<script>location.href='/';</script>";
		}

		$this->artist_id = $this->params["artist_id"];

		$this->artist_name = $this->data_instance->get_artist_by_id($this->artist_id)["name"];
		$this->page_title = "Альбомы " . $this->artist_name;
	}

	private function get_template() {
		if (isset($_GET["type"])) {
			if ($_GET["type"] == "update") {
				$this->data_instance->auth_or_exit();
				return "artist_update.twig";
			} elseif ($_GET["type"] == "delete_artist") {
				$this->data_instance->auth_or_exit();
				return "delete_artist.twig";
			}
		}

		return "gallery.twig";
	}

	protected function get_context() {
		$context = parent::get_context();

		$context["content"] = $this->data_instance->get_albums_by_artist_id($this->artist_id);
		$context["caption"] = $this->page_title;
		$context["is_editable"] = true;
		$context["artist_name"] = $this->artist_name;
		$context["picture"] = "/pics/artist/" . $this->artist_id . ".jpg";

		return $context;
	}

	protected function on_post($context) {
		$id = $this->params["artist_id"];
		$this->data_instance->change_artist($id, $_POST['name']);

		$tmp_name = $_FILES['cover']['tmp_name'];
		move_uploaded_file($tmp_name, "../public/pics/artist/$id.jpg");

		echo "<script>location.href='/artist/$id';</script>";

		return $context;
	}
}

?>