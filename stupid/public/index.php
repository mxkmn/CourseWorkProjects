<?php

require_once '../vendor/autoload.php';
require_once '../src/framework/Router.php';
require_once '../src/framework/DataRepository.php';
require_once '../src/controllers/MainController.php';
require_once '../src/controllers/ArtistController.php';
require_once '../src/controllers/AlbumController.php';
require_once '../src/controllers/SearchController.php';
require_once '../src/controllers/AddAlbumController.php';
require_once '../src/controllers/AddArtistController.php';
require_once '../src/controllers/Page404Controller.php';

$loader = new \Twig\Loader\FilesystemLoader('../src/refs/');
$twig = new \Twig\Environment($loader);

$pdo = new PDO("mysql:host=localhost;dbname=outer_space;charset=utf8", "root", "");
$data = new DataRepository($pdo);

$router = new Router($twig, $data, Page404Controller::class);
$router->add_route("#^/$#", MainController::class);
$router->add_route("#^/search$#", SearchController::class);
$router->add_route("#^/add_album$#", AddAlbumController::class);
$router->add_route("#^/add_artist$#", AddArtistController::class);
$router->add_route("#^/artist/(?P<artist_id>\d+)$#", ArtistController::class);
$router->add_route("#^/album/(?P<album_id>\d+)$#", AlbumController::class);

$controller = $router->get_controller();
$controller->render();

?>