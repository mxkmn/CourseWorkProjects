<?php
require_once '../vendor/autoload.php';
require_once '../framework/autoload.php';
require_once '../controllers/MainController.php';
require_once "../controllers/AlbumController.php";
require_once '../controllers/Controller404.php';
require_once '../controllers/SearchController.php';
require_once '../controllers/CreateController.php';
require_once '../controllers/AlbumDeleteController.php';
require_once "../controllers/AlbumUpdateController.php";
require_once "../controllers/ArtistUpdateController.php";
require_once "../middlewares/LoginRequiredMiddleware.php";
require_once "../controllers/LoginController.php";
require_once "../controllers/LogoutController.php";

$loader = new \Twig\Loader\FilesystemLoader('../views');
$twig = new \Twig\Environment($loader, ["debug" => true]);
$twig->addExtension(new \Twig\Extension\DebugExtension());

session_set_cookie_params(60 * 60 * 12); // 10 hours session
session_start();

$pdo = new PDO("mysql:host=localhost;dbname=outer_space;charset=utf8", "root", "");

$router = new Router($twig, $pdo);
$router->add("/", MainController::class)->middleware(new LoginRequiredMiddleware());
$router->add("/album/(?P<id>\d+)", AlbumController::class)->middleware(new LoginRequiredMiddleware());
$router->add("/album/(?P<id>\d+)/update", AlbumUpdateController::class)->middleware(new LoginRequiredMiddleware());
$router->add("/search", SearchController::class)->middleware(new LoginRequiredMiddleware());
$router->add("/add", CreateController::class)->middleware(new LoginRequiredMiddleware());
$router->add("/modify_artist/(?P<id>\d+)", ArtistUpdateController::class)->middleware(new LoginRequiredMiddleware());
$router->add("/delete", AlbumDeleteController::class)->middleware(new LoginRequiredMiddleware());
$router->add("/login", LoginController::class);
$router->add("/logout", LogoutController::class);
// ещё историю бы
$router->get_or_default(Controller404::class);