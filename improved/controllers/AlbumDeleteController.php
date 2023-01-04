<?php

class AlbumDeleteController extends BaseController
{
	public function post(array $context)
	{
		$id = $_POST['id'];

		unlink("../public/pics/album/$id.jpg");
		$query = $this->pdo->prepare("DELETE FROM album WHERE id = :id");
		$query->bindValue(":id", $id);
		$query->execute();

		header("Location: /");
		exit;
	}
}
