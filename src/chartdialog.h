#pragma once
#include <QDialog>

namespace Ui { class ChartDialog; }

class ChartDialog : public QDialog {
    Q_OBJECT

    public:
        explicit ChartDialog(QWidget *parent = nullptr);
        ~ChartDialog();
        void addData(QList<uint16_t> years, QList<uint32_t> prices);

    private slots:
        void on_horizontalSlider_sliderMoved(int position);

    private:
        Ui::ChartDialog *ui;
        void paintEvent(QPaintEvent *e);
        QList<uint16_t> years;
        QList<uint32_t> prices;
        uint32_t maxPrice;
        uint16_t l, slider = 0;
};
