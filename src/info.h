#pragma once
#include <QDialog>

namespace Ui { class Info; }

class Info : public QDialog {
    Q_OBJECT

    public:
        explicit Info(QWidget *parent = nullptr);
        ~Info();
        void setData(QString titleText, QString labelText);

    private:
        Ui::Info *ui;
};
