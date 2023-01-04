<?php

class DataRepository {
	public function __construct($pdo) {
		$this->pdo = $pdo;
	}

	public function get_artist_by_id($id) {
		$query = $this->pdo->prepare("SELECT * FROM artist where id = :id");
		$query->bindValue("id", $id);
		$query->execute();
		return $query->fetch();
	}

	public function get_albums_by_artist_id($artist_id) {
		$query = $this->pdo->prepare("SELECT * FROM album where artist_id = :artist_id");
		$query->bindValue("artist_id", $artist_id);
		$query->execute();
		return $query->fetchAll();
	}

	public function get_album_by_id($album_id) {
		$query = $this->pdo->prepare("SELECT * FROM album where id = :album_id");
		$query->bindValue("album_id", $album_id);
		$query->execute();
		return $query->fetch();
	}

	public function add_names($albums) {
		for ($i = 0; $i < count($albums); $i++) {
			$name = $this->get_artist_by_id($albums[$i]["artist_id"])["name"] . " | " . $albums[$i]["name"];
			$albums[$i]["name"] = $name;
		}

		return $albums;
	}

	public function get_artists() {
		return $this->pdo->query("SELECT * FROM artist")->fetchAll();
	}

	public function get_albums() {
		return $this->add_names($this->pdo->query("SELECT * FROM album")->fetchAll());
	}

	public function add_album($artist_id, $name, $year, $rating) {
		$this->auth_or_exit();

		$query = $this->pdo->prepare("INSERT INTO album(artist_id, name, year, rating) VALUES(:artist_id, :name, :year, :rating)");
		$query->bindValue("artist_id", $artist_id);
		$query->bindValue("name", $name);
		$query->bindValue("year", $year);
		$query->bindValue("rating", $rating);
		$query->execute();
		return $this->pdo->lastInsertId();
	}

	public function add_artist($name) {
		$this->auth_or_exit();

		$query = $this->pdo->prepare("INSERT INTO artist(name) VALUES(:name)");
		$query->bindValue("name", $name);
		$query->execute();
		return $this->pdo->lastInsertId();
	}

	public function change_album($id, $artist_id, $name, $year, $rating) {
		$this->auth_or_exit();

		$query = $this->pdo->prepare("UPDATE album SET artist_id = :artist_id, name = :name, year = :year, rating = :rating WHERE id = :id");
		$query->bindValue("artist_id", $artist_id);
		$query->bindValue("name", $name);
		$query->bindValue("year", $year);
		$query->bindValue("rating", $rating);
		$query->bindValue("id", $id);
		$query->execute();
	}

	public function change_artist($id, $name) {
		$this->auth_or_exit();

		$query = $this->pdo->prepare("UPDATE artist SET name = :name WHERE id = :id");
		$query->bindValue("name", $name);
		$query->bindValue("id", $id);
		$query->execute();
	}

	public function delete_artist_by_id($id) {
		$this->auth_or_exit();

		$albums = $this->get_albums_by_artist_id($id);
		foreach($albums as $album) {
			unlink("../public/pics/album/{$album["id"]}.jpg");
		}
		unlink("../public/pics/artist/$id.jpg");

		$query = $this->pdo->prepare("DELETE FROM artist where id = :id");
		$query->bindValue("id", $id);
		$query->execute();
	}

	public function delete_album_by_id($id) {
		$this->auth_or_exit();

		unlink("../public/pics/album/$id.jpg");

		$query = $this->pdo->prepare("DELETE FROM album where id = :id");
		$query->bindValue("id", $id);
		$query->execute();
	}

	public function search_albums($artist_id, $name, $min_rating) {
		$query = $this->pdo->prepare(
<<<EOL
SELECT * FROM album
WHERE (:name = '' OR name like CONCAT('%', :name, '%'))
	AND (:artist_id = 'all' OR artist_id = :artist_id)
	AND (:min_rating <= rating)
EOL);
		$query->bindValue("artist_id", $artist_id);
		$query->bindValue("name", $name);
		$query->bindValue("min_rating", $min_rating);
		$query->execute();
		return $this->add_names($query->fetchAll());
	}

	public function is_last_album($artist_id) {
		$query = $this->pdo->prepare("SELECT * FROM album where artist_id = :artist_id");
		$query->bindValue("artist_id", $artist_id);
		$query->execute();
		return count($query->fetchAll()) == 1;
	}

	public function is_authenticated() {
	}

	public function auth_or_exit() {
		$query = $this->pdo->prepare("SELECT * FROM user");
		$query->execute();
		$users = $query->fetchAll();

		// берем значения которые введет пользователь
		$user = isset($_SERVER['PHP_AUTH_USER']) ? $_SERVER['PHP_AUTH_USER'] : '';
		$password = isset($_SERVER['PHP_AUTH_PW']) ? $_SERVER['PHP_AUTH_PW'] : '';

		foreach($users as $userr) {
			if (!($userr["login"] != $user || $userr["password"] != $password)) {
				return;
			}
		}

		// если не совпали, надо указать такой заголовок
		// именно по нему браузер поймет что надо показать окно для ввода юзера/пароля
		header('WWW-Authenticate: Basic realm="Space objects"');
		http_response_code(401); // ну и статус 401 -- Unauthorized, то есть неавторизован
		exit; // прерываем выполнение скрипта
	}
}

?>