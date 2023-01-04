<?php

require_once '../src/controllers/BaseController.php';

class Page404Controller extends BaseController {
    protected $template = "page404.twig";
    protected $page_title = "Страница не найдена";

    public function render() {
        http_response_code(404);
        parent::render();
    }
}

?>