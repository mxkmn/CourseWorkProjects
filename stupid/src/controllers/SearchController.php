<?php

require_once '../src/controllers/BaseController.php';

class SearchController extends BaseController {
    protected $template = "search.twig";
    protected $page_title = "Поиск";

    public function __construct($twig_instance, $data_instance, $params) {
        parent::__construct($twig_instance, $data_instance, $params);
    }

    protected function get_context() {
        $context = parent::get_context();

        $context['get_artist'] = isset($_GET['artist']) ? $_GET['artist'] : 'all';
        $context['get_name'] = isset($_GET['name']) ? $_GET['name'] : '';
        $context['get_min_rating'] = isset($_GET['min_rating']) ? $_GET['min_rating'] : '0.1';
        $context['search_result'] = $this->data_instance->search_albums($context['get_artist'], $context['get_name'], $context['get_min_rating']);

        return $context;
    }
}

?>