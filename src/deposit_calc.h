#pragma once

#include <stdbool.h>

typedef enum {
  NONE = 0,
  MONTH = 1,
  QUARTER = 3,
  HALF = 6,
  YEAR = 12
} periodicity;

typedef struct deposit_s {
  double amount;
  unsigned duration;
  double rate;
  double tax_rate;
  unsigned replenishment;
  double replenishment_amount;
  unsigned withdrawal;
  double withdrawal_amount;
  bool capitalization;
  unsigned payment_period;
  double result_charges;
  double result_tax;
  double reuslt_amount;
} deposit;

void deposit_init(deposit* deposit);
void deposit_calc(deposit* count_deposit);