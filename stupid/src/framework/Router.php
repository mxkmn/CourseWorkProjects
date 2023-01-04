<?php

require_once '../src/middlewares/LoginRequiredMiddeware.php';

class Router {
	private $routes = [];

	public function __construct($twig_instance, $data_instance, $default_controller) {
		$this->twig_instance = $twig_instance;
		$this->data_instance = $data_instance;
		$this->default_controller = $default_controller;
	}

	public function get_controller() {
		$url = rawurldecode(parse_url($_SERVER['REQUEST_URI'], PHP_URL_PATH));

		$controller = $this->default_controller;
		$params = [];
		foreach($this->routes as $route_regexp => $route_controller) {
			if (preg_match($route_regexp, $url, $params)) {
				$controller = $route_controller;
				break;
			}
		}

		return new $controller($this->twig_instance, $this->data_instance, $params);
	}

	public function add_route($regexp, $controller) {
		$this->routes[$regexp] = $controller;
	}
}

?>