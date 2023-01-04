#include "deletequestion.h"
#include "ui_deletequestion.h"

DeleteQuestion::DeleteQuestion(QWidget *parent) : QDialog(parent), ui(new Ui::DeleteQuestion) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); // отключение возможности изменять размер окна
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // отключение ?
}
DeleteQuestion::~DeleteQuestion() { delete ui; }

void DeleteQuestion::setSize(uint32_t s) { // изменение текста
    if (s > 1) ui->label->setText("Вы подтверждаете удаление\n" + QString::number(s) + " записей?");
}

void DeleteQuestion::on_okButton_clicked() { successful = true; close(); } // выход из окна при нажатии "Да"
void DeleteQuestion::on_cancelButton_clicked() { close(); } // выход из окна при нажатии "Отмена"
