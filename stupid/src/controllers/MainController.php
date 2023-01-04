<?php

require_once '../src/controllers/BaseController.php';

class MainController extends BaseController {
    protected $template = "gallery.twig";
    protected $page_title = "Главная";

    protected function get_context() {
        $context = parent::get_context();

        $context["content"] = $this->data_instance->get_albums();
        $context["caption"] = "Добавленные альбомы";

        return $context;
    }
}

?>