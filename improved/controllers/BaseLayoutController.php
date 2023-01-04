<?php

class BaseLayoutController extends TwigBaseController
{
	public function getContext(): array
	{
		$context = parent::getContext();

		$query = $this->pdo->query("SELECT * FROM artist");
		$artists = $query->fetchAll();
		$context['artists'] = $artists;

		if (isset($_SERVER['HTTP_REFERER'])) {
			if (!isset($_SESSION['route'])) {
				$_SESSION['route'] = [];
			}

			$path = substr($_SERVER['HTTP_REFERER'], 0);
			if (end($_SESSION['route']) != $path) {
				if (count($_SESSION['route']) == 9) {
					$_SESSION['route'] = array_slice($_SESSION['route'], 1);
				}
				array_push($_SESSION['route'], $path);
			}
		}

		$context['route'] = isset($_SESSION['route']) ? array_reverse($_SESSION['route']) : "";
		$context['with_route'] = true;

		return $context;
	}
}
