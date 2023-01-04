<?php

abstract class FrameworkController {
	protected $twig_instance, $template;

	public function __construct($twig_instance) {
		$this->twig_instance = $twig_instance;
	}

	protected function get_context() {
		$context = [];
		if ($_SERVER['REQUEST_METHOD'] == 'POST') {
			$context = $this->on_post($context);
		}
		return $context;
	}

	public function render() {
		echo $this->twig_instance->render($this->template, $this->get_context());
	}

	protected function on_post($context) { return $context; }
}

?>