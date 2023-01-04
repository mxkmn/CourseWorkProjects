#include "adddialog.h"
#include "ui_adddialog.h"

#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

AddDialog::AddDialog(QWidget *parent) : QDialog(parent), ui(new Ui::addDialog) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); // отключение возможности изменять размер окна
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // отключение ?

    ui->yearInput1->setValue(QDate::currentDate().year()); // установка текущего года
}
AddDialog::~AddDialog() { delete ui; }

void AddDialog::setData(QString nowFile, QString defFile) { // установка сведений об открытом файле
    nowDataFile = nowFile;
    defaultDataFile = defFile;

    ui->manufacturerInput->setFocus();
}

void AddDialog::setItAsEditor(QList<QTableWidgetItem *> row) { // сделать окно изменяющим запись
    manufacturer = row[0]->text();
    model = row[1]->text();
    year = row[2]->text().toUInt();
    color = row[3]->text();
    mileage = row[4]->text().toUInt();
    price = row[5]->text().toUInt();

    ui->manufacturerInput->setText(manufacturer);
    ui->modelInput->setText(model);
    ui->colorInput->setText(color);
    ui->yearInput1->setValue(year);
    ui->mileageInput1->setValue(mileage);
    ui->priceInput1->setValue(price);

    addMode = false;
    this->setWindowTitle("Изменение записи");
    ui->changeButton->setText("Изменить");
}

void AddDialog::on_changeButton_clicked() { // обработка данных при нажатии на кнопку
    QString line = ui->manufacturerInput->text() + '\t' + ui->modelInput->text() + '\t' + ui->yearInput1->text() + '\t' + ui->colorInput->text() + '\t' + ui->mileageInput1->text() + '\t' + ui->priceInput1->text();
    QFile file(nowDataFile); // открытие файла
    if (line.count('\n') == 0 and line.count('\t') == 5 and ui->manufacturerInput->text() != "" and ui->modelInput->text() != "" and  ui->colorInput->text() != "") {
        if (file.exists() and file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // получение исходных данных
            QString fileData = file.readAll();
            file.close();

            // запись новых данных
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                if (nowDataFile == defaultDataFile) QMessageBox::warning(this, "Проблема изменения файла", "Возникла проблема при обращении к БД. Запись не была изменена. Повторите попытку.");
                else QMessageBox::warning(this, "Проблема изменения файла", "Возникла проблема при обращении к файлу " + nowDataFile + ". Запись не была изменена. Повторите попытку");
                return;
            }
            QTextStream writeStream(&file);
            writeStream.setCodec("UTF-8");

            if (addMode) {
                writeStream << fileData << line << '\n';
            }
            else {
                QString prevLine = manufacturer + '\t' + model + '\t' + QString::number(year) + '\t' + color + '\t' + QString::number(mileage) + '\t' + QString::number(price);
                writeStream << fileData.replace(fileData.indexOf(prevLine), prevLine.size(), line);
            }
            file.close();

            // выход из окна
            successful = true;
            close();
        }
        else {
            if (nowDataFile == defaultDataFile) QMessageBox::warning(this, "Проблема открытия файла", "Возникла проблема при обращении к БД. Записи не были получены, изменение невозможно. Повторите попытку.");
            else QMessageBox::warning(this, "Проблема открытия файла", "Возникла проблема при обращении к файлу " + nowDataFile + ". Записи не были получены, изменение невозможно. Повторите попытку.");
        }
    }
    else QMessageBox::warning(this, "Ошибка введения данных", "Вы не ввели данные в поле, вставили знак перехода на следующую строку или знак табуляции. Это могло произойти при копировании из другого источника. Добавьте данные или удалите эти знаки и повторите попытку.");
}
