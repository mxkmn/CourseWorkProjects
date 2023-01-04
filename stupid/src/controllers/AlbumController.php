<?php

require_once '../src/controllers/BaseController.php';

enum InfoType {
	case Start;
	case Desc;
	case Cover;
	case Update;
	case DeleteAlbum;
	case DeleteAlbumAndArtist;
}

class AlbumController extends BaseController {
	private $is_last = false;

	public function __construct($twig_instance, $data_instance, $params) {
		parent::__construct($twig_instance, $data_instance, $params);

		$album = $this->data_instance->get_album_by_id($this->params["album_id"]);
		$this->album = $album;
		$artist = $this->data_instance->get_artist_by_id($album["artist_id"]);

		$this->is_last = $this->data_instance->is_last_album($album["artist_id"]);

		$this->page_title = $album["name"] . " от " . $artist["name"];
		switch ($this->get_info_type()) {
			case InfoType::Start:
				$this->template = "album-start.twig";
				break;
			case InfoType::Desc:
				$this->template = "album-desc.twig";
				$this->year = $album["year"];
				$this->rating = $album["rating"];
				break;
			case InfoType::Cover:
				$this->template = "album-cover.twig";
				$this->image = "/pics/album/{$album["id"]}.jpg";
			break;
			case InfoType::Update:
				$this->data_instance->auth_or_exit();
				$this->template = "album-update.twig";
			break;
			case InfoType::DeleteAlbumAndArtist:
				$this->data_instance->auth_or_exit();
				$this->data_instance->delete_artist_by_id($artist["id"]);
				// ↓
			case InfoType::DeleteAlbum:
				$this->data_instance->auth_or_exit();
				$this->data_instance->delete_album_by_id($this->params["album_id"]);
				echo "<script>location.href='/';</script>";
			break;
		}
	}

	private function get_info_type() {
		if (isset($_GET["type"])) {
			if ($_GET["type"] == "desc") {
				return InfoType::Desc;
			} elseif ($_GET["type"] == "cover") {
				return InfoType::Cover;
			} elseif ($_GET["type"] == "delete_album") {
				return InfoType::DeleteAlbum;
			} elseif ($_GET["type"] == "delete_album_and_artist") {
				return InfoType::DeleteAlbumAndArtist;
			} elseif ($_GET["type"] == "update") {
				return InfoType::Update;
			}
		}

		return InfoType::Start;
	}

	protected function get_context() {
		$context = parent::get_context();

		$context["name"] = $this->album["name"];

		switch ($this->get_info_type()) {
			case InfoType::Desc:
				$context["description"] = [
					[
						"icon" => "fa-calendar",
						"value" => $this->year
					],
					[
						"icon" => "fa-star",
						"value" => $this->rating
					]
				];
				break;
			case InfoType::Cover:
				$context["image"] = $this->image;
				break;
			case InfoType::Update:
				$context["album"] = $this->album;
				$context["name"] = "Изменение альбома " . $context["name"];
				break;
		}

		if ($this->is_last) {
			$context["is_last"] = true;
		}

		return $context;
	}

	protected function on_post($context) {
		$id = $this->params["album_id"];
		$this->data_instance->change_album($id, $_POST['artist'], $_POST['name'], $_POST['year'], $_POST['rating']);

		$tmp_name = $_FILES['cover']['tmp_name'];
		move_uploaded_file($tmp_name, "../public/pics/album/$id.jpg");

		echo "<script>location.href='/album/$id';</script>";

		return $context;
	}
}

?>