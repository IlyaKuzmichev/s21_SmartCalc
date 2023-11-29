#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "graphwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  GraphWindow *graph_window;
  void SetValidators();
  void SetSlots();
  bool CheckInputDeposit();
  unsigned SetPeriodicity(int index);

 private slots:
  void press_calc();
  void press_func();
  void on_button_ac_clicked();
  void on_button_eq_clicked();
  void on_button_graph_clicked();
  void on_button_backspace_clicked();
  void on_button_calculate_clicked();
  void on_combo_deposit_currentIndexChanged(int index);
  void on_combo_withdrawal_currentIndexChanged(int index);
  void on_button_calculate_depo_clicked();
};

#endif  // MAINWINDOW_H
