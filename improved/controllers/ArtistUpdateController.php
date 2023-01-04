<?php
require_once "BaseLayoutController.php";

class ArtistUpdateController extends BaseLayoutController
{
	public $template = "artist_update.twig";

	public function get(array $context)
	{
		$query = $this->pdo->prepare("SELECT * FROM artist WHERE id = :id");
		$query->bindValue("id", $this->params['id']);
		$query->execute();
		$context['artist_name'] = $query->fetch()["name"];

		parent::get($context);
	}

	public function post(array $context)
	{
		$id = $this->params["id"];
		$query = $this->pdo->prepare("UPDATE artist SET name = :name WHERE id = :id");
		$query->bindValue("name", $_POST['name']);
		$query->bindValue("id", $id);
		$query->execute();

		$tmp_name = $_FILES['cover']['tmp_name'];
		move_uploaded_file($tmp_name, "../public/pics/artist/$id.jpg");

		header("Location: /?type=$id");
		exit;
	}
}
