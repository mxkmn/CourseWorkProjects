<?php
require_once "BaseLayoutController.php";

class AlbumUpdateController extends BaseLayoutController
{
	public $template = "album_update.twig";

	public function get(array $context)
	{
		$query = $this->pdo->prepare("SELECT * FROM album WHERE id = :id");
		$query->bindValue("id", $this->params['id']);
		$query->execute();
		$context['album'] = $query->fetch();

		parent::get($context);
	}

	public function post(array $context)
	{
		$id = $this->params["id"];
		$query = $this->pdo->prepare("UPDATE album SET artist_id = :artist_id, name = :name, year = :year, rating = :rating WHERE id = :id");
		$query->bindValue("artist_id", $_POST['artist']);
		$query->bindValue("name", $_POST['name']);
		$query->bindValue("year", $_POST['year']);
		$query->bindValue("rating", $_POST['rating']);
		$query->bindValue("id", $id);
		$query->execute();

		$tmp_name = $_FILES['cover']['tmp_name'];
		move_uploaded_file($tmp_name, "../public/pics/album/$id.jpg");

		header("Location: /album/$id");
		exit;
	}
}
