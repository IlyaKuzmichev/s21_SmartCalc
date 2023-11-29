#include "graphwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <algorithm>

#include "ui_graphwindow.h"
GraphWindow::GraphWindow(QString lexeme, QWidget *parent)
    : QDialog(parent), ui(new Ui::GraphWindow) {
  ui->setupUi(this);
  scale_x_min = -10.;
  scale_x_max = 10.;
  scale_y_min = -10.;
  scale_y_max = 10.;
  ui->line_xmax->setValidator(VALIDATE_DOUBLE);
  ui->line_xmin->setValidator(VALIDATE_DOUBLE);
  ui->line_ymax->setValidator(VALIDATE_DOUBLE);
  ui->line_ymin->setValidator(VALIDATE_DOUBLE);
  strncpy(expression, qPrintable(lexeme), SIZE_ARRAY);
  PrintGraph();
}

GraphWindow::~GraphWindow() { delete ui; }

void GraphWindow::PrintGraph() {
  int error_code = 0;
  double static_array[2 * POINTS_QUANTITY] = {0};
  QVector<double> x(POINTS_QUANTITY), y(POINTS_QUANTITY);
  QPen pen(Qt::red, 0, Qt::DotLine);

  error_code =
      create_graph(expression, scale_x_min, scale_x_max, &static_array[0]);
  if (error_code) {
    QMessageBox::warning(
        this, "Error",
        "Incorrect expression for draw graph!\nPlease try again");
    QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
  } else {
    for (unsigned i = 0; i < POINTS_QUANTITY && !error_code; i++) {
      x[i] = *(static_array + 2 * i);
      y[i] = *(static_array + 2 * i + 1);
    }
    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y);
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(0)->setScatterStyle(QCPScatterStyle(
        QCPScatterStyle::ssCircle, QPen(Qt::darkCyan), QBrush(Qt::darkRed), 2));
    ui->widget->xAxis->setLabel("x axis");
    ui->widget->yAxis->setLabel("y axis");
    ui->widget->xAxis->setLabelColor(Qt::darkMagenta);
    ui->widget->yAxis->setLabelColor(Qt::darkMagenta);
    ui->widget->xAxis->grid()->setPen(pen);
    ui->widget->yAxis->grid()->setPen(pen);
    ui->widget->xAxis->setRange(scale_x_min, scale_x_max);
    ui->widget->yAxis->setRange(scale_y_min, scale_y_max);
    ui->widget->replot();
    ui->widget->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag |
                                QCP::iSelectPlottables);
    ui->widget->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    ui->widget->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
  }
}

void GraphWindow::on_line_xmin_textChanged(const QString &arg1) {
  ui->line_xmin->setValidator(new QRegularExpressionValidator(
      QRegularExpression("^[+-]?[0-9]+([.][0-9]+)?$"), this));
}

void GraphWindow::on_line_xmax_textChanged(const QString &arg1) {
  ui->line_xmax->setValidator(new QRegularExpressionValidator(
      QRegularExpression("^[+-]?[0-9]+([.][0-9]+)?$"), this));
}

void GraphWindow::on_button_count_clicked() {
  scale_x_min = ui->line_xmin->text().toDouble();
  scale_x_max = ui->line_xmax->text().toDouble();
  scale_y_min = ui->line_ymin->text().toDouble();
  scale_y_max = ui->line_ymax->text().toDouble();

  PrintGraph();
}
