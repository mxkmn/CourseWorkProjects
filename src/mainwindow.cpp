#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "adddialog.h"
#include "info.h"
#include "deletequestion.h"
#include "chartdialog.h"

#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QFileDialog>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {    
    ui->setupUi(this);
    this->statusBar()->setSizeGripEnabled(false); // отключение возможности изменять размер окна

    year2Search = QDate::currentDate().year();
    ui->yearInput2->setValue(year2Search);

    QFile file(defaultDataFile); // проверка открытия стандартной БД
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "Проблема открытия файла", "Файл с базой данных " + defaultDataFile + " не найден в каталоге программы.\n\nЭто нормально при первом запуске, но если вы уже пользовались программой - удостоверьтесь, что случайно не удалили его.\n\nФайл был создан заново. Вы можете приступить к добавлению новых записей.");
            file.close();
        }
        else QMessageBox::warning(this, "Проблема открытия файла", "Файл с базой данных " + defaultDataFile + " не найден в каталоге программы.\n\nЭто нормально при первом запуске, но если вы уже пользовались программой - удостоверьтесь, что случайно не удалили его.\n\nФайл не получилось создать заново. Воспользуйтесь пунктом \"Файл\" в меню.");
    }
    else getDataFromFile();

    updateLastFilesMenu();
}
MainWindow::~MainWindow() { delete ui; }

void MainWindow::setRecentDataFile(QString file) { // установка нового файла
    nowDataFile = file;
    getDataFromFile();
}
void MainWindow::updateLastFilesMenu() { // обновление последних файлов в меню
    // удаление всего, что имеется сейчас
    QList<QAction *> actions = ui->chooseFile->actions();
    for (int i = 0; i < actions.length(); i++) { ui->chooseFile->removeAction(actions[i]); }

    // отключение пункта меню на случай, если файлов нет
    ui->chooseFile->setDisabled(true);

    // получение последних файлов из файла
    QFile file(lastFilesFile); // открытие файла
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    while (!file.atEnd()) {
        QString f = file.readLine();
        f.remove('\n');
        ui->chooseFile->addAction(f, this, [this, f]{ setRecentDataFile(f); });

        ui->chooseFile->setDisabled(false); // включение пункта меню
    }
}
void MainWindow::addNewLastFile(QString f) { // добавление последнего файла вверх
    if (f == defaultDataFile) return; // прерываем выполнение функции в случае обработки стандартной БД

    QList<QString> files; // получение последних файлов из файла

    QFile file(lastFilesFile); // открытие файла
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.close();

        file.open(QIODevice::WriteOnly); file.close(); // создание файла если файла не существует
    }
    else {
        while (!file.atEnd()) {
            QString ftemp = file.readLine();
            ftemp.remove('\n');
            if (ftemp.toLower() != f.toLower() and ftemp!="") files.append(ftemp);
        }
        file.close();
    }

    // запись новых данных
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream writeStream(&file);
    writeStream.setCodec("UTF-8");
    writeStream << f << '\n';
    for (uint8_t i = 0; i < qMin(files.length(), 9); i++) { writeStream << files[i] << '\n'; } // добавление до 9 последних файлов после нового (суммарно до 10)
    file.close();

    updateLastFilesMenu(); // добавляем файлы в меню
}

bool MainWindow::lineFits(QString line) { // проверка строки на "правильность"
    QStringList ss = line.split('\t');
    return ( ss.length() == 6 and
         ss[0].contains(manufacturerSearch, Qt::CaseInsensitive) and
         ss[1].contains(modelSearch, Qt::CaseInsensitive) and
         year1Search <= ss[2].toUInt() and ss[2].toUInt() <= year2Search and
         ss[3].contains(colorSearch, Qt::CaseInsensitive) and
         mileage1Search <= ss[4].toUInt() and ss[4].toUInt() <= mileage2Search and
         price1Search <= ss[5].toUInt() and ss[5].toUInt() <= price2Search );
}

void MainWindow::addLine(QString line) { // добавление строки в таблицу
    QStringList ss = line.split('\t');

    ui->tableWidget->setRowCount(++row);

    for(uint8_t column = 0; column < 6; column++) {
        QString sss = ss.at(column).split('\n')[0]; // получение строки без \n в последнем слове
        QTableWidgetItem *newItem = new QTableWidgetItem;
        if (column == 2 or column == 4 or column == 5)
            newItem->setData(Qt::EditRole, sss.toInt());
        else
            newItem->setText(sss);

        ui->tableWidget->setItem(row-1, column, newItem);
    }
}
void MainWindow::getDataFromFile() { // получение данных из файла и помещение в таблицу
    row = 0; ui->tableWidget->setRowCount(row); // удаление всего старого

    QFile file(nowDataFile); // открытие файла
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (nowDataFile == defaultDataFile) QMessageBox::warning(this, "Проблема открытия файла", "Возникла проблема при обращении к БД. Записи не были получены. Повторите попытку.");
        else QMessageBox::warning(this, "Проблема открытия файла", "Возникла проблема при обращении к файлу " + nowDataFile + ". Записи не были получены. Повторите попытку.");
        ui->statusbar->showMessage("Файл " + nowDataFile + " недоступен");
        return;
    }

    while (!file.atEnd()) {
        ui->tableWidget->setSortingEnabled(false);

        QString line = file.readLine();
        if (lineFits(line)) addLine(line);

        ui->tableWidget->setSortingEnabled(true);
    }

    if (nowDataFile == defaultDataFile) ui->statusbar->showMessage("Работа со стандартной БД");
    else {
        ui->statusbar->showMessage("Работа с файлом " + nowDataFile);
        addNewLastFile(nowDataFile);
    }
}

void MainWindow::setSearch() { // применение поисковых фильтров
    manufacturerSearch = ui->manufacturerInput->text();
    modelSearch = ui->modelInput->text();
    colorSearch = ui->colorInput->text();
    year1Search = ui->yearInput1->value();
    year2Search = ui->yearInput2->value();
    mileage1Search = ui->mileageInput1->value();
    mileage2Search = ui->mileageInput2->value();
    price1Search = ui->priceInput1->value();
    price2Search = ui->priceInput2->value();

    getDataFromFile();

    QFont font("Century Gothic", 16, -1, false);
    ui->searchButton->setFont(font);
}



void MainWindow::on_chartButton_clicked() { // вывод диаграммы при нажатии на кнопку
    // формирование списка данных
    QList<uint16_t> years; QList<QList<uint32_t>> pricesByYear;
    for (uint32_t i = 0; i < row; i++) {
        uint16_t yearTemp = ui->tableWidget->item(i, 2)->text().toUInt();
        uint32_t priceTemp = ui->tableWidget->item(i, 5)->text().toUInt();
        if (years.indexOf(yearTemp) == -1) { // год появился впервые
            years.append(yearTemp);
            QList<uint32_t> tempList;
            tempList.append(priceTemp);
            pricesByYear.append(tempList);
        }
        else pricesByYear[years.indexOf(yearTemp)].append(priceTemp);
    }

    // выходим из программы в случае, если данных нет
    if (years.length() == 0) {
        QMessageBox::warning(this, "Отсутствуют данные", "Невозможно построение диаграммы без данных в таблице!");
        return;
    }

    // сортируем по годам
    QList<uint16_t> yearsSorted;
    for (uint16_t i = 0; i < years.length(); i++) {
        yearsSorted.append(years[i]);
    }
    std::sort(yearsSorted.begin(), yearsSorted.end());

    // составляем конечный массив
    QList<uint32_t> pricesSorted;
    for (uint16_t i = 0; i < years.length(); i++) {
        QList<uint32_t> prices = pricesByYear[years.indexOf(yearsSorted[i])];
        uint32_t sum = 0;
        for (uint16_t j = 0; j < prices.length(); j++) {
            sum += prices[j];
        }
        pricesSorted.append(sum/prices.length());
    }

    ChartDialog chart;
    chart.addData(yearsSorted, pricesSorted);
    chart.exec();
}



// обработка нажатий по кнопкам, изменений полей ввода
void MainWindow::on_searchButton_clicked() { setSearch(); }
void MainWindow::on_disableSearchButton_clicked() {
    ui->manufacturerInput->setText("");
    ui->modelInput->setText("");
    ui->colorInput->setText("");
    ui->yearInput1->setValue(1886); ui->yearInput2->setValue(QDate::currentDate().year());
    ui->mileageInput1->setValue(0); ui->mileageInput2->setValue(999999);
    ui->priceInput1->setValue(0); ui->priceInput2->setValue(99999999);

   setSearch();
}

void MainWindow::paintSearchButton() {
    if (  ui->manufacturerInput->text() == manufacturerSearch and
          ui->modelInput->text() == modelSearch and
          ui->colorInput->text() == colorSearch and
          ui->yearInput1->text().toUInt() == year1Search and ui->yearInput2->text().toUInt() == year2Search and
          ui->mileageInput1->text().toUInt() == mileage1Search and ui->mileageInput2->text().toUInt() == mileage2Search and
          ui->priceInput1->text().toUInt() == price1Search and ui->priceInput2->text().toUInt() == price2Search  ) {
        QFont font("Century Gothic", 16, -1, false);
        ui->searchButton->setFont(font);
    }
    else {
        QFont font("Century Gothic", 16, -1, true);
        ui->searchButton->setFont(font);
    }
}
void MainWindow::on_manufacturerInput_textChanged() { paintSearchButton(); }
void MainWindow::on_modelInput_textChanged() { paintSearchButton(); }
void MainWindow::on_colorInput_textChanged() { paintSearchButton(); }
void MainWindow::on_yearInput1_valueChanged(int) { paintSearchButton(); }
void MainWindow::on_yearInput2_valueChanged(int) { paintSearchButton(); }
void MainWindow::on_mileageInput1_valueChanged(int) { paintSearchButton(); }
void MainWindow::on_mileageInput2_valueChanged(int) { paintSearchButton(); }
void MainWindow::on_priceInput1_valueChanged(int) { paintSearchButton(); }
void MainWindow::on_priceInput2_valueChanged(int) { paintSearchButton(); }


void MainWindow::on_saveFile_triggered() {
    QString path = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("Текстовые файлы (*.txt *.data)"));

    if (path == "") return;

    QFile file(path); // открытие файла

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream writeStream(&file);
        writeStream.setCodec("UTF-8");

        for (uint32_t i = 0; i < row; i++) {
            writeStream << ui->tableWidget->item(i, 0)->text() + '\t' + ui->tableWidget->item(i, 1)->text() + '\t' + ui->tableWidget->item(i, 2)->text() + '\t' + ui->tableWidget->item(i, 3)->text() + '\t' + ui->tableWidget->item(i, 4)->text() + '\t' + ui->tableWidget->item(i, 5)->text() + '\n';
        }
        file.close();

        addNewLastFile(path);
    }
    else {
        QMessageBox::warning(this, "Проблема создания файла", "Файл не получилось создать. Повторите попытку.");
        return;
    }
}
void MainWindow::on_getFile_triggered() {
    QString path = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("Текстовые файлы (*.txt *.data)"));
    if (path == "") return;

    setRecentDataFile(path);
}

void MainWindow::on_defaultFile_triggered() { setRecentDataFile(defaultDataFile); }


void MainWindow::on_addElem_triggered() {
    if (nowDataFile != defaultDataFile) {
        QMessageBox::warning(this, "Обратите внимание", "Сейчас вы работаете с файлом " + nowDataFile + ". Если вы хотите работать со стандартной БД - закройте окно добавления и выберите соответствующий пункт в меню \"Файл\".");
    }

    AddDialog addDialogWindow;
    addDialogWindow.setData(nowDataFile, defaultDataFile);
    addDialogWindow.exec();
    if (addDialogWindow.successful) getDataFromFile(); // если данные успешно добавлены - обновляем таблицу
}
void MainWindow::on_editElem_triggered() {
    QList<QTableWidgetItem *> items = ui->tableWidget->selectedItems();
    switch (items.size()/6) {
        case 1: { // выбрана одна строка
            if (nowDataFile != defaultDataFile) QMessageBox::warning(this, "Обратите внимание", "Сейчас вы работаете с файлом " + nowDataFile + ". Если вы хотите работать со стандартной БД - закройте окно изменения и выберите соответствующий пункт в меню \"Файл\".");

            AddDialog editDialogWindow;
            editDialogWindow.setItAsEditor(items);
            editDialogWindow.setData(nowDataFile, defaultDataFile);
            editDialogWindow.exec();
            if (editDialogWindow.successful) getDataFromFile(); // если данные успешно добавлены - обновляем таблицу
            break;
        }
        default: { // выбрано несколько строк или ни одной
            QApplication::beep();
            ui->statusbar->showMessage("Выберите одну строку для изменения");
            break;
        }
    }
}
void MainWindow::on_delElem_triggered() {
    QList<QTableWidgetItem *> items = ui->tableWidget->selectedItems();
    if (items.size()/6 == 0) {
        QApplication::beep();
        ui->statusbar->showMessage("Выберите одну или несколько строк для удаления");
        return;
    }
    else {
        if (nowDataFile != defaultDataFile) QMessageBox::warning(this, "Обратите внимание", "Сейчас вы работаете с файлом " + nowDataFile + ". Если вы хотите работать со стандартной БД - закройте окно удаления и выберите соответствующий пункт в меню \"Файл\".");

        DeleteQuestion del;
        del.setSize(items.size()/6);
        del.exec();
        if (!del.successful) return;
    }

    QString fileData;
    QFile file(nowDataFile); // открытие файла
    if (file.exists() and file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // получение исходных данных
        fileData = file.readAll();
        file.close();
    }
    else {
        if (nowDataFile == defaultDataFile) QMessageBox::warning(this, "Проблема открытия файла", "Возникла проблема при обращении к БД. Записи не были получены, удаление невозможно. Повторите попытку.");
        else QMessageBox::warning(this, "Проблема открытия файла", "Возникла проблема при обращении к файлу " + nowDataFile + ". Записи не были получены, удаление невозможно. Повторите попытку.");
        ui->statusbar->showMessage("Файл " + nowDataFile + " недоступен");
        return;
    }

    for (int i = 0; i < items.length()/6; i++) {
        QString line = items[i*6+0]->text() + '\t' + items[i*6+1]->text() + '\t' + items[i*6+2]->text() + '\t' + items[i*6+3]->text() + '\t' + items[i*6+4]->text() + '\t' + items[i*6+5]->text() + '\n';
        fileData.remove(fileData.indexOf(line), line.size());
    }


    if (file.exists() and file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream writeStream(&file);
        writeStream.setCodec("UTF-8");
        //qDebug() << fileData;
        writeStream << fileData;
        file.close();

        getDataFromFile();
    }
    else {
        if (nowDataFile == defaultDataFile) QMessageBox::warning(this, "Проблема изменения файла", "Возникла проблема при удалении данных БД. Повторите попытку.");
        else QMessageBox::warning(this, "Проблема изменения файла", "Возникла проблема при удалении данных из файла " + nowDataFile + ". Повторите попытку.");
        ui->statusbar->showMessage("Файл " + nowDataFile + " недоступен");
        return;
    }
}

void MainWindow::on_cancelElem_triggered() { ui->tableWidget->clearSelection(); }


void MainWindow::on_guide_triggered() {
    QFile file(guideDataFile); // открытие файла
    if (file.exists() and file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Info guideWindow;
        guideWindow.setData("Руководство пользователя", file.readAll());
        file.close();
        guideWindow.exec();
    }
    else QMessageBox::warning(this, "Проблема открытия файла", "Возникла проблема нахождения файла " + guideDataFile + " с руководством пользователя. Повторите попытку.");
}
void MainWindow::on_task_triggered() {
    Info taskWindow;
    taskWindow.setData("Задание", "50. Программа «Автоаукцион» должна обрабатывать файл, содержащий сведения:\nпроизводитель а/м, марка, год выпуска, цвет, пробег, начальная цена.\n\nНаписать программу, позволяющую сформировать список автомобилей одной марки,\nотсортированный по цене. Построить диаграмму средней стоимости а/м одной марки\nв зависимости от года выпуска. Вывести список автомашин заданного производителя.");
    taskWindow.exec();
}
void MainWindow::on_about_triggered() {
    Info aboutWindow;
    aboutWindow.setData("О программе", "Разработчик: censored\nГруппа: censored\nВерсия: 1.0.0, дата релиза: 20 мая 2021\n\nСвязь с разработчиком:\nTelegram: censored\nEmail: censored");
    aboutWindow.exec();
}

void MainWindow::on_tableWidget_cellDoubleClicked() { ui->editElem->trigger(); } // двойное нажатие по таблице
