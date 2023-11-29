#pragma once

#include <stdbool.h>

typedef struct credit_s {
  double amount;
  unsigned duration;
  double rate;
  bool annuity;
  double monthly_payment;
  double payment_diff_first;
  double payment_diff_last;
  double interest_charges;
  double total_payment;
} credit;

void credit_calc(credit* count_credit);
void annuity_credit_calc(credit* count_credit);
void diff_credit_calc(credit* count_credit);
