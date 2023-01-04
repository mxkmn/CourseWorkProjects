<?php

class LoginController extends TwigBaseController
{
	public $template = "login.twig";

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
		$user = $_POST['username'];
		$password = $_POST['password'];
		$sql = "SELECT * FROM user WHERE login = :username AND password = :password";

		$query = $this->pdo->prepare($sql);
		$query->bindValue("username", $user);
		$query->bindValue("password", $password);
		$query->execute();
		$data = $query->fetch();

		$is_correct = empty($data) ? false : true;

		$_SESSION['is_logged'] = $is_correct;
		$context['with_route'] = false;
		if (!$is_correct) {
			header("Location: /login");
			exit;
		} else {
			header("Location: /");
			exit;
		}
	}
}
