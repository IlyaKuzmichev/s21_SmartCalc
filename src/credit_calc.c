#include "credit_calc.h"

#include <math.h>

void credit_init(credit* credit) {
  credit->monthly_payment = 0.;
  credit->payment_diff_first = 0.;
  credit->payment_diff_last = 0.;
  credit->interest_charges = 0.;
  credit->total_payment = 0.;
}

void credit_calc(credit* count_credit) {
  credit_init(count_credit);
  if (count_credit->annuity) {
    annuity_credit_calc(count_credit);
  } else {
    diff_credit_calc(count_credit);
  }
}

void annuity_credit_calc(credit* count_credit) {
  double m_rate = count_credit->rate / 1200.;

  count_credit->monthly_payment =
      (m_rate * pow(1 + m_rate, count_credit->duration)) /
      (pow(1 + m_rate, count_credit->duration) - 1) * count_credit->amount;
  count_credit->total_payment =
      count_credit->monthly_payment * count_credit->duration;
  count_credit->interest_charges =
      count_credit->total_payment - count_credit->amount;
}

void diff_credit_calc(credit* count_credit) {
  double main_payment = count_credit->amount / count_credit->duration;

  count_credit->total_payment = 0;
  for (unsigned i = 0; i < count_credit->duration; i++) {
    double remainder = count_credit->amount - main_payment * i;
    double rate_payment = remainder * count_credit->rate / 1200.;
    count_credit->interest_charges += rate_payment;
    if (0 == i) {
      count_credit->payment_diff_first = main_payment + rate_payment;
    }
    if (count_credit->duration - 1 == i) {
      count_credit->payment_diff_last = main_payment + rate_payment;
    }
  }
  count_credit->total_payment =
      count_credit->amount + count_credit->interest_charges;
}
