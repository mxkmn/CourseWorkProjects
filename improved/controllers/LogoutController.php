<?php

class LogoutController extends TwigBaseController
{

	public function getContext(): array
	{
		$context = parent::getContext();
		return $context;
	}

	public function get(array $context)
	{
		parent::get($context);
	}

	public function post(array $context)
	{
		$_SESSION['is_logged'] = false;
		header("Location: /login");
	}
}
