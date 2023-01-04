#pragma once
#include <QDialog>

namespace Ui { class DeleteQuestion; }

class DeleteQuestion : public QDialog {
    Q_OBJECT

    public:
        explicit DeleteQuestion(QWidget *parent = nullptr);
        ~DeleteQuestion();

        bool successful = false;
        void setSize(uint32_t s);

    private slots:
        void on_okButton_clicked();
        void on_cancelButton_clicked();

    private:
        Ui::DeleteQuestion *ui;
};
