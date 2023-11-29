#include "mainwindow.h"

#include <QAction>
#include <QDebug>
#include <QIntValidator>
#include <QMessageBox>
#include <iostream>

#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  auto eq_shortcut = new QAction(this);
  eq_shortcut->setShortcuts({Qt::Key_Equal, Qt::Key_Return, Qt::Key_Enter});
  this->addAction(eq_shortcut);
  auto f = [&]() { ui->button_eq->animateClick(); };
  connect(eq_shortcut, &QAction::triggered, f);
  SetValidators();
  SetSlots();
  ui->line_sumdepo->hide();
  ui->line_sumwithdrawal->hide();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::SetSlots() {
  connect(ui->button_openbracket, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_closebracket, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_point, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_0, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_1, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_2, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_3, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_4, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_5, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_6, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_7, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_8, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_9, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_div, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_sum, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_sub, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_mul, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_mod, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_x, SIGNAL(clicked()), this, SLOT(press_calc()));
  connect(ui->button_sqrt, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_log, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_ln, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_sin, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_cos, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_tan, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_asin, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_acos, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_atan, SIGNAL(clicked()), this, SLOT(press_func()));
  connect(ui->button_pow, SIGNAL(clicked()), this, SLOT(press_func()));
}

void MainWindow::SetValidators() {
  ui->line_x->setValidator(VALIDATE_DOUBLE);
  ui->line_amount->setValidator(VALIDATE_POSITIVE_DOUBLE);
  ui->line_rate->setValidator(VALIDATE_POSITIVE_DOUBLE);
  ui->line_depamount->setValidator(VALIDATE_POSITIVE_DOUBLE);
  ui->line_deprate->setValidator(VALIDATE_POSITIVE_DOUBLE);
  ui->line_deptax->setValidator(VALIDATE_POSITIVE_DOUBLE);
  ui->line_sumdepo->setValidator(VALIDATE_POSITIVE_DOUBLE);
  ui->line_sumwithdrawal->setValidator(VALIDATE_POSITIVE_DOUBLE);
  ui->line_duration->setValidator(VALIDATE_INT);
  ui->line_depperiod->setValidator(VALIDATE_INT);
}

void MainWindow::press_calc() {
  QPushButton *button = (QPushButton *)sender();
  QString line;

  line = (ui->line_lexeme->text() + button->text());
  ui->line_lexeme->setText(line);
}

void MainWindow::press_func() {
  QPushButton *button = (QPushButton *)sender();
  QString line;

  line = (ui->line_lexeme->text() + button->text() + '(');
  ui->line_lexeme->setText(line);
}

void MainWindow::on_button_ac_clicked() { ui->line_lexeme->clear(); }

void MainWindow::on_button_eq_clicked() {
  QString x_line = (ui->line_x->text());
  double x_arg = 0.;
  bool is_x_arg = false;

  x_arg = x_line.toDouble(&is_x_arg);
  x_line = (ui->line_lexeme->text());
  if (!x_line.isEmpty()) {
    int error_code = 0;
    double result = 0.;
    char str[SIZE_ARRAY] = {0};
    strncpy(str, qPrintable(x_line), SIZE_ARRAY);
    error_code = count_lexeme(str, x_arg, is_x_arg, &result);
    if (ERROR_POLISH == error_code) {
      QMessageBox::warning(this, "Error", "Incorrect expression");
    } else if (ARGUMENT_REQUIRED == error_code) {
      QMessageBox::warning(this, "Error", "Argument required");
    } else {
      x_line = QString::number(result, 'g', 16);
      ui->line_lexeme->setText(x_line);
    }
  } else {
    QMessageBox::information(this, "Empty input", "Input smth, ABOBA!");
  }
}

void MainWindow::on_button_graph_clicked() {
  QString lexeme = ui->line_lexeme->text();
  if (lexeme.isEmpty()) {
    QMessageBox::information(this, "Empty input", "Input smth, ABOBA!");
  } else {
    graph_window = new GraphWindow(lexeme, nullptr);
    graph_window->show();
  }
}

void MainWindow::on_button_backspace_clicked() {
  QString str = ui->line_lexeme->text();
  if (!str.isEmpty()) {
    str.chop(1);
    ui->line_lexeme->setText(str);
  }
}

void MainWindow::on_button_calculate_clicked() {
  credit count_credit = {0};

  if (ui->line_amount->text().isEmpty() ||
      ui->line_duration->text().isEmpty() || ui->line_rate->text().isEmpty()) {
    QMessageBox::information(
        this, "Empty lines",
        "Not enougth parameters!\nPlease, input amount, duration and rate");
  } else {
    count_credit.amount = ui->line_amount->text().toDouble();
    count_credit.duration = ui->line_duration->text().toInt();
    count_credit.rate = ui->line_rate->text().toDouble();
    count_credit.annuity = ui->radio_annuity->isChecked();
    credit_calc(&count_credit);
    QString to_print;
    if (count_credit.annuity) {
      to_print = QString::number(count_credit.monthly_payment, 'f', 2);
      ui->line_mpayment->setText(to_print);
    } else {
      to_print =
          (QString::number(count_credit.payment_diff_first, 'f', 2) + " ... " +
           QString::number(count_credit.payment_diff_last, 'f', 2));
      ui->line_mpayment->setText(to_print);
    }
    to_print = QString::number(count_credit.interest_charges, 'f', 2);
    ui->line_charges->setText(to_print);
    to_print = QString::number(count_credit.total_payment, 'f', 2);
    ui->line_tpayment->setText(to_print);
  }
}

void MainWindow::on_combo_deposit_currentIndexChanged(int index) {
  ui->line_sumdepo->show();
  ui->line_sumdepo->setFocus();
}

void MainWindow::on_combo_withdrawal_currentIndexChanged(int index) {
  ui->line_sumwithdrawal->show();
  ui->line_sumwithdrawal->setFocus();
}

void MainWindow::on_button_calculate_depo_clicked() {
  deposit count_deposit = {0};
  if (CheckInputDeposit()) {
    count_deposit.amount = ui->line_depamount->text().toDouble();
    count_deposit.duration = ui->line_depperiod->text().toInt();
    count_deposit.rate = ui->line_deprate->text().toDouble();
    count_deposit.tax_rate = ui->line_deptax->text().toDouble();
    count_deposit.replenishment =
        SetPeriodicity(ui->combo_deposit->currentIndex());
    count_deposit.replenishment_amount = ui->line_sumdepo->text().toDouble();
    count_deposit.withdrawal =
        SetPeriodicity(ui->combo_withdrawal->currentIndex());
    count_deposit.withdrawal_amount = ui->line_sumwithdrawal->text().toDouble();
    count_deposit.capitalization = ui->checkBox_capital->isChecked();
    count_deposit.payment_period =
        SetPeriodicity(ui->combo_period->currentIndex());
    deposit_calc(&count_deposit);

    QString to_print;
    to_print = QString::number(count_deposit.result_charges, 'f', 2);
    ui->line_depcharges->setText(to_print);
    to_print = QString::number(count_deposit.result_tax, 'f', 2);
    ui->line_deptaxamount->setText(to_print);
    to_print = QString::number(count_deposit.reuslt_amount, 'f', 2);
    ui->line_depamountsum->setText(to_print);
  }
}

bool MainWindow::CheckInputDeposit() {
  bool result = true;
  bool rate = true;

  if (ui->line_depamount->text().isEmpty()) {
    result = false;
  }
  if (ui->line_depperiod->text().isEmpty()) {
    result = false;
  }
  if (ui->line_deprate->text().isEmpty()) {
    result = false;
  } else if (ui->line_deprate->text().toDouble() >= 100) {
    rate = false;
  }
  if (ui->line_deptax->text().isEmpty()) {
    result = false;
  } else if (ui->line_deptax->text().toDouble() >= 100) {
    rate = false;
  }

  if (!result) {
    QMessageBox::information(this, "Empty lines",
                             "Not enougth parameters!\nPlease, input amount, "
                             "period, interest and tax rate");
  } else if (!rate) {
    QMessageBox::information(this, "Empty lines",
                             "Incorrect amount of interest or tax rate.\nInput "
                             "number from 0 to 100");
  }
  return result;
}

unsigned MainWindow::SetPeriodicity(int index) {
  unsigned value = 0;
  switch (index) {
    case 2:
      value = QUARTER;
      break;
    case 3:
      value = HALF;
      break;
    case 4:
      value = YEAR;
      break;
    default:
      value = index;
      break;
  }
  return value;
}
