<?php
require_once "../controllers/BaseLayoutController.php";

class SearchController extends BaseLayoutController
{
	public $template = "search.twig";

	public function getContext(): array
	{
		$context = parent::getContext();

		$context['get_artist'] = isset($_GET['artist']) ? $_GET['artist'] : 'all';
		$context['get_name'] = isset($_GET['name']) ? $_GET['name'] : '';
		$context['get_min_rating'] = isset($_GET['min_rating']) ? $_GET['min_rating'] : '0.1';


		$query = $this->pdo->prepare(<<<EOL
			SELECT * FROM album
			WHERE (:name = '' OR name like CONCAT('%', :name, '%'))
				AND (:artist_id = 'all' OR artist_id = :artist_id)
				AND (:min_rating <= rating)
			EOL);
		$query->bindValue("artist_id", $context['get_artist']);
		$query->bindValue("name", $context['get_name']);
		$query->bindValue("min_rating", $context['get_min_rating']);
		$query->execute();
		$context['search_result'] = $query->fetchAll();


		return $context;
	}
}
