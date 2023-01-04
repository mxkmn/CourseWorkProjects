<?php
require_once "BaseLayoutController.php";

class CreateController extends BaseLayoutController
{
	public $template = "create.twig";
	public $title = "Добавление";

	public function getContext(): array
	{
		$context = parent::getContext();
		$query = $this->pdo->prepare("SELECT * FROM artist");
		$query->execute();
		$artist = $query->fetchAll();
		$context['artist'] = $artist;

		return $context;
	}

	public function post(array $context)
	{
		if (isset($_POST['album'])) {
			$sql = <<<EOL
				INSERT INTO album(artist_id, name, year, rating)
				VALUES(:artist_id, :name, :year, :rating)
				EOL;

			$query = $this->pdo->prepare($sql);
			$query->bindValue("name", $_POST['name']);
			$query->bindValue("artist_id", $_POST['artist_id']);
			$query->bindValue("year", $_POST['year']);
			$query->bindValue("rating", $_POST['rating']);

			$query->execute();
			$id = $this->pdo->lastInsertId();

			$tmp_name = $_FILES['cover']['tmp_name'];
			move_uploaded_file($tmp_name, "../public/pics/album/$id.jpg");

			$context['name_album'] = $_POST['name'];
			$context['id'] = $id;
		} elseif (isset($_POST['artist'])) {
			$query = $this->pdo->prepare("INSERT INTO artist(name) VALUES(:name)");
			$query->bindValue("name", $_POST['name']);
			$query->execute();
			$id = $this->pdo->lastInsertId();

			$tmp_name = $_FILES['photo']['tmp_name'];
			move_uploaded_file($tmp_name, "../public/pics/artist/$id.jpg");

			$context['name_artist'] = $_POST['name'];
			$context['id'] = $id;
		}

		$this->get($context);
	}
}
