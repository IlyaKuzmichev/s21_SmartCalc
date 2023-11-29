#include "deposit_calc.h"

void deposit_init(deposit* deposit) {
  deposit->result_charges = 0.;
  deposit->result_tax = 0.;
  deposit->reuslt_amount = 0.;
}

void deposit_calc(deposit* count_deposit) {
  deposit_init(count_deposit);
  double interest_sum = 0;
  count_deposit->reuslt_amount = count_deposit->amount;
  for (unsigned i = 0; i < count_deposit->duration; ++i) {
    if (count_deposit->replenishment &&
        !((i + 1) % count_deposit->replenishment)) {
      count_deposit->reuslt_amount += count_deposit->replenishment_amount;
    }
    if (count_deposit->withdrawal && !((i + 1) % count_deposit->withdrawal) &&
        count_deposit->reuslt_amount > count_deposit->withdrawal_amount) {
      count_deposit->reuslt_amount -= count_deposit->withdrawal_amount;
    }
    interest_sum += count_deposit->reuslt_amount * count_deposit->rate / 1200.;
    if ((count_deposit->payment_period &&
         !((i + 1) % count_deposit->payment_period)) ||
        i == count_deposit->duration - 1) {
      count_deposit->result_charges += interest_sum;
      count_deposit->result_tax +=
          interest_sum * count_deposit->tax_rate / 100.;
      if (count_deposit->capitalization) {
        count_deposit->reuslt_amount += interest_sum;
      }
      interest_sum = 0;
    }
  }
}
