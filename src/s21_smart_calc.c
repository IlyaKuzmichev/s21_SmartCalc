#include "s21_smart_calc.h"

#include "polish_notation.h"

int count_lexeme(char* lexeme, double argument, bool is_arg, double* result) {
  s21_token token_lexeme[SIZE_ARRAY] = {0}, notation_result[SIZE_ARRAY] = {0};
  int error = OK;
  unsigned counter_lexeme = 0, counter_notation = 0;

  error = parse_string(lexeme, token_lexeme, &counter_lexeme);
  if (!error) {
    error = postfix_notation(token_lexeme, counter_lexeme, notation_result,
                             &counter_notation);
  }
  if (!error) {
    error = count_notation(notation_result, counter_notation, argument, is_arg,
                           result);
  }
  return error;
}

int create_graph(char* lexeme, double scale_x_min, double scale_x_max,
                 double* result_vector) {
  s21_token token_lexeme[SIZE_ARRAY] = {0}, notation_result[SIZE_ARRAY] = {0};
  unsigned counter_lexeme = 0, counter_notation = 0;
  int error = OK;

  error = parse_string(lexeme, token_lexeme, &counter_lexeme);
  if (!error) {
    error = postfix_notation(token_lexeme, counter_lexeme, notation_result,
                             &counter_notation);
  }
  if (!error) {
    double step_x = (double)(scale_x_max - scale_x_min) / (POINTS_QUANTITY - 1);
    for (unsigned i = 0; i < POINTS_QUANTITY && !error; i++) {
      *(result_vector + 2 * i) = scale_x_min;
      error = count_notation(notation_result, counter_notation, scale_x_min,
                             true, result_vector + 2 * i + 1);
      scale_x_min += step_x;
    }
  }
  return error;
}