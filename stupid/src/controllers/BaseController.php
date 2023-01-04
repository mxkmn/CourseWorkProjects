<?php

require_once '../src/framework/FrameworkController.php';

class BaseController extends FrameworkController {
    public function __construct($twig_instance, $data_instance, $params) {
        parent::__construct($twig_instance);

        $this->data_instance = $data_instance;
        $this->params = $params;
    }

    protected function get_context() {
        $context = parent::get_context();

        $context["page_title"] = $this->page_title;
        $context["artists"] = $this->data_instance->get_artists();

        return $context;
    }
}

?>