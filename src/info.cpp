#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) : QDialog(parent), ui(new Ui::Info) {
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // отключение ?
}
Info::~Info() { delete ui; }

void Info::setData(QString titleText, QString labelText) { // установка текста
    this->setWindowTitle(titleText);
    ui->infoLabel->setText(labelText);
}
