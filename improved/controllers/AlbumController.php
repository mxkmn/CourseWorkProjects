<?php
require_once "../controllers/BaseLayoutController.php";

class AlbumController extends BaseLayoutController
{
	public $template = "album.twig";

	public function getContext(): array
	{
		if (isset($_GET['type'])) {
			if ($_GET['type'] == "cover") {
				$this->template = "album-cover.twig";
			} else {
				$this->template = "album-desc.twig";
			}
		} else {
			$this->template = "album-start.twig";
		}
		$context = parent::getContext();

		$query = $this->pdo->prepare("SELECT * FROM album WHERE id = :id;");
		$query->bindValue("id", $this->params['id']);
		$query->execute();

		$data = $query->fetch();

		$context["description"] = [
			[
				"icon" => "fa-calendar",
				"value" => $data["year"]
			],
			[
				"icon" => "fa-star",
				"value" => $data["rating"]
			]
		];
		$context["album"] = $data;
		$context["title"] = "Альбом " . $data["name"];

		// $context["my_session_message"] = isset($_SESSION['welcome_message']) ? $_SESSION['welcome_message'] : "";
		// $context["messages"] = isset($_SESSION['messages']) ? $_SESSION['messages'] : "";

		return $context;
	}
}
