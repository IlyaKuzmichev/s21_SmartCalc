#include "polish_notation.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

int push_stack(s21_stack* static_stack, s21_token value) {
  int error = OK;

  if (static_stack && static_stack->quantity < 256) {
    static_stack->array[static_stack->quantity] = value;
    ++(static_stack->quantity);
  } else {
    error = ERROR_POLISH;
  }
  return error;
}

int pop_stack(s21_stack* static_stack, s21_token* value) {
  int error = OK;

  if (static_stack && static_stack->quantity > 0) {
    *value = static_stack->array[static_stack->quantity - 1];
    static_stack->array[static_stack->quantity - 1].type = 0;
    static_stack->array[static_stack->quantity - 1].lexeme = 0;
    static_stack->array[static_stack->quantity - 1].priority = 0;
    static_stack->array[static_stack->quantity - 1].value = 0;
    --(static_stack->quantity);
  } else {
    error = ERROR_POLISH;
  }
  return error;
}

int parse_string(char* lexeme, s21_token* result, unsigned* counter) {
  int error = OK;
  int flag_operator = 1;
  int flag_bracket = 0;

  *counter = 0;
  while (*lexeme && !error) {
    int counters_index[] = {get_number(lexeme, &result[*counter]),
                            get_brackets(lexeme, &result[*counter]),
                            get_operator(lexeme, &result[*counter]),
                            get_function(lexeme, &result[*counter]),
                            get_variable(lexeme, &result[*counter])};
    if ((MINUS == result[*counter].lexeme) && (1 == flag_operator)) {
      result[*counter].lexeme = UNMINUS;
      result[*counter].type = UNARY_MINUS;
      result[*counter].priority = THIRD_PRIORITY;
    }
    if (OPERATOR == result[*counter].type) {
      ++flag_operator;
    } else if ((NUMBER == result[*counter].type) ||
               (VARIABLE == result[*counter].type)) {
      --flag_operator;
    }
    if (BRACKET == result[*counter].type) {
      if (OPEN_BRACKET == result[*counter].lexeme) {
        ++flag_bracket;
        if (*counter && CLOSE_BRACKET == result[*counter - 1].lexeme) {
          flag_bracket = -1;
        }
      } else {
        --flag_bracket;
      }
    }
    int length = 0;
    for (int i = 0; i < 5; ++i) {
      if (counters_index[i]) {
        length = counters_index[i];
      }
    }
    if ((PLUS == result[*counter].lexeme) && (2 == flag_operator)) {
      --flag_operator;
      ++lexeme;
    } else if (length) {
      ++(*counter);
      lexeme += length;
    } else {
      error = ERROR_POLISH;
    }
    if ((flag_operator > 1) || (flag_operator < 0) || (flag_bracket < 0)) {
      error = ERROR_POLISH;
    }
  }
  if (flag_bracket) {
    error = ERROR_POLISH;
  }
  return error;
}

int get_number(char* lexeme, s21_token* token) {
  int symbol_counter = 0, valid_number = 1;
  double number = 0., multiplier = 1.0;
  char* temp = lexeme;

  while (*temp && ((*temp >= '0' && *temp <= '9') || ('.' == *temp)) &&
         valid_number) {
    if ('.' == *temp && fabs(multiplier - 1.0) < S21_EPS) {
      multiplier = 0.1;
    } else if ('.' == *temp) {
      valid_number = 0;
    } else {
      if (fabs(multiplier - 1.0) < S21_EPS) {
        number *= 10.;
        number += (double)*temp - '0';
      } else {
        number += (double)(*temp - '0') * multiplier;
        multiplier /= 10.;
      }
    }
    ++temp;
    ++symbol_counter;
  }
  if (symbol_counter) {
    token->type = NUMBER;
    token->value = (double)number;
  }
  if (!valid_number) {
    symbol_counter = 0;
  }
  return symbol_counter;
}

int get_brackets(char* lexeme, s21_token* token) {
  int symbol_counter = 0;
  if ('(' == *lexeme) {
    token->type = BRACKET;
    token->lexeme = OPEN_BRACKET;
    ++symbol_counter;
  } else if (')' == *lexeme) {
    token->type = BRACKET;
    token->lexeme = CLOSE_BRACKET;
    ++symbol_counter;
  }
  return symbol_counter;
}

int get_operator(char* lexeme, s21_token* token) {
  int symbol_counter = 0;
  switch (*lexeme) {
    case '+':
      token->lexeme = PLUS;
      token->priority = FIRST_PRIORITY;
      symbol_counter++;
      break;
    case '-':
      token->lexeme = MINUS;
      token->priority = FIRST_PRIORITY;
      symbol_counter++;
      break;
    case '*':
      token->lexeme = MUL;
      token->priority = SECOND_PRIORITY;
      symbol_counter++;
      break;
    case '/':
      token->lexeme = DIV;
      token->priority = SECOND_PRIORITY;
      symbol_counter++;
      break;
    case '^':
      token->lexeme = POW;
      token->priority = FOURTH_PRIORITY;
      symbol_counter++;
      break;
    default:
      break;
  }
  if (!symbol_counter && strlen(lexeme) >= 3 &&
      0 == strncmp(lexeme, "mod", 3)) {
    token->lexeme = MOD;
    token->priority = SECOND_PRIORITY;
    symbol_counter += 3;
  }
  if (symbol_counter) {
    token->type = OPERATOR;
  }
  return symbol_counter;
}

int get_function(char* lexeme, s21_token* token) {
  int symbol_counter = 0;

  if (strlen(lexeme) >= 4) {
    if (0 == strncmp(lexeme, "sin", 3)) {
      token->lexeme = SIN;
      symbol_counter += 3;
    } else if (0 == strncmp(lexeme, "cos", 3)) {
      token->lexeme = COS;
      symbol_counter += 3;
    } else if (0 == strncmp(lexeme, "tan", 3)) {
      token->lexeme = TAN;
      symbol_counter += 3;
    } else if (0 == strncmp(lexeme, "asin", 4)) {
      token->lexeme = ASIN;
      symbol_counter += 4;
    } else if (0 == strncmp(lexeme, "acos", 4)) {
      token->lexeme = ACOS;
      symbol_counter += 4;
    } else if (0 == strncmp(lexeme, "atan", 4)) {
      token->lexeme = ATAN;
      symbol_counter += 4;
    } else if (0 == strncmp(lexeme, "sqrt", 4)) {
      token->lexeme = SQRT;
      symbol_counter += 4;
    } else if (0 == strncmp(lexeme, "ln", 2)) {
      token->lexeme = LN;
      symbol_counter += 2;
    } else if (0 == strncmp(lexeme, "log", 3)) {
      token->lexeme = LOG;
      symbol_counter += 3;
    }
  }
  if (symbol_counter) {
    token->type = FUNCTION;
    token->priority = FIFTH_PRIORITY;
  }
  return symbol_counter;
}

int get_variable(char* lexeme, s21_token* token) {
  int symbol_counter = 0;

  if ('x' == *lexeme) {
    token->type = VARIABLE;
    ++symbol_counter;
  }
  return symbol_counter;
}

int postfix_notation(s21_token* lexeme, unsigned counter, s21_token* result,
                     unsigned* counter_result) {
  s21_stack reverse_stack = {0};
  int counter_lexeme = 0, error = OK;

  while (counter && !error) {
    if (NUMBER == lexeme[counter_lexeme].type ||
        VARIABLE == lexeme[counter_lexeme].type) {
      result[*counter_result] = lexeme[counter_lexeme];
      ++(*counter_result);
    } else if (FUNCTION == lexeme[counter_lexeme].type) {
      push_stack(&reverse_stack, lexeme[counter_lexeme]);
    } else if (BRACKET == lexeme[counter_lexeme].type) {
      if (OPEN_BRACKET == lexeme[counter_lexeme].lexeme) {
        push_stack(&reverse_stack, lexeme[counter_lexeme]);
      } else {
        s21_token value = {0};
        while (reverse_stack.array[reverse_stack.quantity - 1].lexeme !=
               OPEN_BRACKET) {
          error = pop_stack(&reverse_stack, &value);
          if (!error) {
            result[*counter_result] = value;
            ++(*counter_result);
          }
        }
        error = pop_stack(&reverse_stack, &value);
        if (!error && reverse_stack.quantity &&
            FUNCTION == reverse_stack.array[reverse_stack.quantity - 1].type) {
          error = pop_stack(&reverse_stack, &value);
          if (!error) {
            result[*counter_result] = value;
            ++(*counter_result);
          }
        }
      }
    } else if (OPERATOR == lexeme[counter_lexeme].type ||
               UNARY_MINUS == lexeme[counter_lexeme].type) {
      while (reverse_stack.quantity &&
             reverse_stack.array[reverse_stack.quantity - 1].priority >=
                 lexeme[counter_lexeme].priority) {
        s21_token value = {0};
        error = pop_stack(&reverse_stack, &value);
        if (!error) {
          result[*counter_result] = value;
          ++(*counter_result);
        }
      }
      error = push_stack(&reverse_stack, lexeme[counter_lexeme]);
    }
    --counter;
    ++counter_lexeme;
  }
  while (reverse_stack.quantity) {
    s21_token value = {0};
    error = pop_stack(&reverse_stack, &value);
    if (!error) {
      result[*counter_result] = value;
      ++(*counter_result);
    }
  }
  return error;
}

int count_notation(s21_token* lexeme, unsigned counter, double argument,
                   bool is_arg, double* result) {
  int error = OK;
  s21_stack operand_stack = {0};

  for (unsigned i = 0; i < counter && !error; ++i) {
    if (VARIABLE == lexeme[i].type) {
      if (true == is_arg) {
        lexeme[i].value = argument;
        error = push_stack(&operand_stack, lexeme[i]);
      } else {
        error = ARGUMENT_REQUIRED;
      }
    } else if (NUMBER == lexeme[i].type) {
      error = push_stack(&operand_stack, lexeme[i]);
    } else if (OPERATOR == lexeme[i].type) {
      s21_token upper = {0}, lower = {0};
      error = pop_stack(&operand_stack, &upper);
      if (!error) error = pop_stack(&operand_stack, &lower);
      if (PLUS == lexeme[i].lexeme) {
        lower.value += upper.value;
      } else if (MINUS == lexeme[i].lexeme) {
        lower.value -= upper.value;
      } else if (MUL == lexeme[i].lexeme) {
        lower.value *= upper.value;
      } else if (DIV == lexeme[i].lexeme) {
        lower.value /= upper.value;
      } else if (MOD == lexeme[i].lexeme) {
        lower.value = fmod(lower.value, upper.value);
      } else if (POW == lexeme[i].lexeme) {
        lower.value = pow(lower.value, upper.value);
      }
      if (!error) error = push_stack(&operand_stack, lower);
    } else if (FUNCTION == lexeme[i].type || UNARY_MINUS == lexeme[i].type) {
      s21_token upper = {0};
      if (!error) error = pop_stack(&operand_stack, &upper);
      if (SIN == lexeme[i].lexeme) {
        upper.value = sin(upper.value);
      } else if (COS == lexeme[i].lexeme) {
        upper.value = cos(upper.value);
      } else if (TAN == lexeme[i].lexeme) {
        upper.value = tan(upper.value);
      } else if (ASIN == lexeme[i].lexeme) {
        upper.value = asin(upper.value);
      } else if (ACOS == lexeme[i].lexeme) {
        upper.value = acos(upper.value);
      } else if (ATAN == lexeme[i].lexeme) {
        upper.value = atan(upper.value);
      } else if (SQRT == lexeme[i].lexeme) {
        upper.value = sqrt(upper.value);
      } else if (LN == lexeme[i].lexeme) {
        upper.value = log(upper.value);
      } else if (LOG == lexeme[i].lexeme) {
        upper.value = log10(upper.value);
      } else if (UNMINUS == lexeme[i].lexeme) {
        upper.value *= -1;
      }
      if (!error) error = push_stack(&operand_stack, upper);
    }
  }
  if (!error && operand_stack.quantity) {
    *result = operand_stack.array[operand_stack.quantity - 1].value;
  }
  return error;
}