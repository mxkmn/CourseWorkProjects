#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        QString manufacturerSearch, modelSearch, colorSearch;
        uint16_t year1Search = 1886, year2Search = 2050;
        uint32_t mileage1Search = 0, mileage2Search = 999999,
                 price1Search = 0, price2Search = 99999999;

        const QString defaultDataFile = "cars.data";
        QString nowDataFile = defaultDataFile;
        const QString lastFilesFile = "lastfiles.data";
        const QString guideDataFile = "readme.txt";

    private:
        Ui::MainWindow *ui;
        uint32_t row = 0;

        bool lineFits(QString line);
        void getDataFromFile();
        void setSearch();
        void addLine(QString line);
        void tryEditLine();
        void paintSearchButton();
        void setRecentDataFile(QString file);
        void updateLastFilesMenu();
        void addNewLastFile(QString f);

    private slots:
        void on_manufacturerInput_textChanged();
        void on_modelInput_textChanged();
        void on_colorInput_textChanged();
        void on_yearInput1_valueChanged(int);
        void on_yearInput2_valueChanged(int);
        void on_mileageInput1_valueChanged(int);
        void on_mileageInput2_valueChanged(int);
        void on_priceInput1_valueChanged(int);
        void on_priceInput2_valueChanged(int);

        void on_searchButton_clicked();
        void on_disableSearchButton_clicked();

        void on_chartButton_clicked();

        void on_saveFile_triggered();
        void on_getFile_triggered();
        void on_defaultFile_triggered();

        void on_addElem_triggered();
        void on_editElem_triggered();
        void on_delElem_triggered();
        void on_cancelElem_triggered();

        void on_guide_triggered();
        void on_task_triggered();
        void on_about_triggered();

        void on_tableWidget_cellDoubleClicked();
};
