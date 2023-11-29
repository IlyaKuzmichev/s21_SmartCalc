#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QDialog>

#ifdef __cplusplus
extern "C" {
#endif
#include "../credit_calc.h"
#include "../deposit_calc.h"
#include "../polish_notation.h"
#include "../s21_smart_calc.h"
#ifdef __cplusplus
}
#endif

#define UPPER_LIMIT 1000000.
#define LOWER_LIMIT -1000000.

#define VALIDATE_DOUBLE            \
  new QRegularExpressionValidator( \
      QRegularExpression("^[+-]?[0-9]+([.][0-9]+)?$"), this)
#define VALIDATE_POSITIVE_DOUBLE                                              \
  new QRegularExpressionValidator(QRegularExpression("^[0-9]+([.][0-9]+)?$"), \
                                  this)
#define VALIDATE_INT \
  new QRegularExpressionValidator(QRegularExpression("^[0-9]+$"), this)

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QDialog {
  Q_OBJECT

 public:
  explicit GraphWindow(QString lexeme, QWidget *parent = nullptr);
  ~GraphWindow();

 private slots:
  void on_line_xmin_textChanged(const QString &arg1);

  void on_line_xmax_textChanged(const QString &arg1);

  void on_button_count_clicked();

 private:
  Ui::GraphWindow *ui;
  double scale_x_min, scale_x_max;
  double scale_y_min, scale_y_max;
  char expression[255];
  void PrintGraph();
};

#endif  // GRAPHWINDOW_H
