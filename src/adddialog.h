#pragma once
#include <QDialog>
#include <QTableWidget>

namespace Ui { class addDialog; }

class AddDialog : public QDialog {
    Q_OBJECT

    public:
        explicit AddDialog(QWidget *parent = nullptr);
        ~AddDialog();
        void setData(QString nowFile, QString defFile);
        void setItAsEditor(QList<QTableWidgetItem *> row);

        bool successful = false;
        QString nowDataFile, defaultDataFile;

    private slots:
        void on_changeButton_clicked();

    private:
        Ui::addDialog *ui;

        bool addMode = true;
        QString manufacturer, model, color;
        uint16_t year;
        uint32_t mileage, price;
};
