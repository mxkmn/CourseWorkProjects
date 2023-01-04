#include "chartdialog.h"
#include "ui_chartdialog.h"

#include <QPainter>
#include <QDebug>

ChartDialog::ChartDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ChartDialog) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint); // отключение возможности изменять размер окна
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint); // отключение ?
}

ChartDialog::~ChartDialog() { delete ui; }

void ChartDialog::addData(QList<uint16_t> y, QList<uint32_t> p) { // добавление переданных данных, установка необходимых значений
    years = y;
    prices = p;

    l = y.length();

    // установка подписей цены слева и справа
    maxPrice = *std::max_element(p.begin(), p.end());
    ui->L4->setText(QString::number(maxPrice)); ui->R4->setText(QString::number(maxPrice));
    ui->L3->setText(QString::number(maxPrice/4*3)); ui->R3->setText(QString::number(maxPrice/4*3));
    ui->L2->setText(QString::number(maxPrice/2)); ui->R2->setText(QString::number(maxPrice/2));
    ui->L1->setText(QString::number(maxPrice/4)); ui->R1->setText(QString::number(maxPrice/4));

    if (l < 11) { // отключение слайдера, если все данные входят
        ui->horizontalSlider->setVisible(false);
        ui->label_2->setVisible(false);
    } // изменение максимума слайдера, если не все данные входят
    else ui->horizontalSlider->setMaximum(l-10);
}

void ChartDialog::paintEvent(QPaintEvent *e) { // отрисовка диаграммы после открытия окна или после движения слайдера
    Q_UNUSED(e); QPainter qp(this);

    // строим коробку
    QPen boxLine(Qt::black, 2, Qt::SolidLine); qp.setPen(boxLine);

    qp.drawLine(100, 20, 700, 20);
    qp.drawLine(100, 20, 100, 450);
    qp.drawLine(700, 20, 700, 450);
    qp.drawLine(100, 450, 700, 450);

    // строим дополнительные линии
    QPen additionalLine(Qt::gray, 1, Qt::SolidLine); qp.setPen(additionalLine);

    qp.drawLine(100, 50, 700, 50);
    qp.drawLine(100, 150, 700, 150);
    qp.drawLine(100, 250, 700, 250);
    qp.drawLine(100, 350, 700, 350);

    // строим полоски диаграммы и подписи
    QPainter myText(this); myText.setFont(QFont("Helvetica", 8));

    if (l < 11) {
        uint16_t step = 295-59/2*l;
        for (uint8_t i = 0; i < l; i++) {
            qp.fillRect(110+step+59*i, 450, 49, -400*(1.0*prices[i]/maxPrice), Qt::blue);
            qp.drawText(120+step+59*i, 470, QString::number(years[i]));
        }
    }
    else {
        for (uint8_t i = 0; i < 10; i++) {
            qp.fillRect(110+59*i, 450, 49, -400*(1.0*prices[i+slider]/maxPrice), Qt::blue);
            qp.drawText(120+59*i, 470, QString::number(years[i+slider]));
        }
    }
}

void ChartDialog::on_horizontalSlider_sliderMoved(int position) { // обработка движения слайдера снизу
    slider = position;
    update();
}
