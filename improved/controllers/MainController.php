<?php
require_once "BaseLayoutController.php";

class MainController extends BaseLayoutController
{
	public $template = "main.twig";
	public $title = "Главная";

	public function getContext(): array
	{
		$context = parent::getContext();

		if (isset($_GET['type'])) {
			if (isset($_GET['delete']) && $_GET['delete'] == true) {
				$id = $_GET['type'];

				$query = $this->pdo->prepare("SELECT * FROM album where artist_id = :artist_id");
				$query->bindValue("artist_id", $id);
				$query->execute();
				$albums = $query->fetchAll();

				foreach($albums as $album) {
					unlink("../public/pics/album/{$album["id"]}.jpg");
				}
				unlink("../public/pics/artist/$id.jpg");

				$query = $this->pdo->prepare("DELETE FROM artist where id = :id");
				$query->bindValue("id", $id);
				$query->execute();

				header("Location: /");
				exit;
			}
			$query = $this->pdo->prepare("SELECT * FROM artist WHERE id = :type");
			$query->bindValue("type", $_GET['type']);
			$query->execute();
			$artist = $query->fetch();

			$context["caption"] = "Альбомы " . $artist["name"];
			$context["id"] = $_GET['type'];

			$query = $this->pdo->prepare("SELECT * FROM album WHERE artist_id = :type");
			$query->bindValue("type", $_GET['type']);
			$query->execute();
		} else {
			$context["caption"] = "Все альбомы";

			$query = $this->pdo->query("SELECT * FROM album");
		}

		$context['content'] = $query->fetchAll();

		return $context;
	}
}
