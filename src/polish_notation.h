#pragma once

#include "s21_smart_calc.h"

/**
 * @{ @name Error code constants.
 */
#define OK 0                 ///< Error code 0
#define ERROR_POLISH 1       ///< Error code 1
#define ARGUMENT_REQUIRED 2  ///< Error code 2
/**
 * @}
 */
#define S21_EPS 1e-16   ///< Epsilon for calculation
#define SIZE_ARRAY 255  ///< Max size of lexeme(expression) in calculator

/**
 * s21_lexeme
 * @brief enum for lexeme tokens
 */
typedef enum s21_lexeme {
  PLUS = 1,
  MINUS,
  UNMINUS,
  MUL,
  DIV,
  MOD,
  POW,
  SIN,
  COS,
  TAN,
  ASIN,
  ACOS,
  ATAN,
  SQRT,
  LN,
  LOG,
  OPEN_BRACKET,
  CLOSE_BRACKET
} s21_lexeme;

/**
 * s21_type
 * @brief enum for lexeme token types
 */
typedef enum s21_type {
  BRACKET = 1,
  OPERATOR,
  FUNCTION,
  UNARY_MINUS,
  NUMBER,
  VARIABLE
} s21_type;

/**
 * s21_priority
 * @brief enum for lexeme token's priority
 */
typedef enum s21_priority {
  FIRST_PRIORITY = 1,  ///> basic + and -
  SECOND_PRIORITY,     ///> * and / and mod
  THIRD_PRIORITY,      ///> unary -
  FOURTH_PRIORITY,     ///> ^
  FIFTH_PRIORITY,      ///> functions
  SIXTH_PRIORITY       ///> brackets
} s21_priority;

/**
 * s21_token
 * @brief Lexeme token structure
 */
typedef struct s21_token {
  char type;      ///> type of token
  char lexeme;    ///> symbols of lexeme
  char priority;  ///> operand/operator priority
  double value;   ///> value if number
} s21_token;

/**
 * s21_stack
 * @brief Struct for stack, based on static array
 */
typedef struct s21_stack {
  s21_token array[SIZE_ARRAY];  ///> array for stack
  unsigned quantity;  ///> variable, that remember index of upper element
} s21_stack;

/**
 * @brief Fucntion for pushing element to stack
 *
 * @param static_stack --- pointer to stack
 * @param value --- value of struct s21_token to push
 *
 * @return Error code from constant definitions
 */
int push_stack(s21_stack* static_stack, s21_token value);

/**
 * @brief Fucntion for pop element from stack
 *
 * @param static_stack --- pointer to stack
 * @param value --- value of struct s21_token to pop
 *
 * @return Error code from constant definitions
 */
int pop_stack(s21_stack* static_stack, s21_token* value);

/**
 * @brief Fucntion for parsing char* lexeme by tokens
 *
 * @param lexeme --- pointer to string
 * @param result --- pointer to array of tokens
 * @param counter --- variable to count tokens
 *
 * @return Error code from constant definitions
 */
int parse_string(char* lexeme, s21_token* result, unsigned* counter);

/**
 * @brief Function for checking is number
 *
 * @param lexeme - poiner to string
 * @param token - pointer to token to write
 *
 * @return Error code from constant definitions
 */
int get_number(char* lexeme, s21_token* token);

/**
 * @brief Function for checking is brackets
 *
 * @param lexeme - poiner to string
 * @param token - pointer to token to write
 *
 * @return Error code from constant definitions
 */
int get_brackets(char* lexeme, s21_token* token);

/**
 * @brief Function for checking is operator
 *
 * @param lexeme - poiner to string
 * @param token - pointer to token to write
 *
 * @return Error code from constant definitions
 */
int get_operator(char* lexeme, s21_token* token);

/**
 * @brief Function for checking is function
 *
 * @param lexeme - poiner to string
 * @param token - pointer to token to write
 *
 * @return Error code from constant definitions
 */
int get_function(char* lexeme, s21_token* token);

/**
 * @brief Function for checking is variable
 *
 * @param lexeme - poiner to string
 * @param token - pointer to token to write
 *
 * @return Error code from constant definitions
 */
int get_variable(char* lexeme, s21_token* token);

/**
 * @brief Convert string(lexeme) to postfix notation
 *
 * @param lexeme --- pointer to array of tokens
 * @param counter --- number of tokens
 * @param result --- pointer to array of result tokens
 * @param counter_result --- variable to write quantity of tokens
 *
 * @return Error code from constant definitions
 */
int postfix_notation(s21_token* lexeme, unsigned counter, s21_token* result,
                     unsigned* counter_result);

/**
 * @brief Convert string(lexeme) to postfix notation
 *
 * @param lexeme --- pointer to array of tokens
 * @param counter --- number of tokens
 * @param argument --- value of 'x' argument
 * @param is_arg --- boolean of availability of argument
 * @param result --- variable to write result of calculation
 *
 * @return Error code from constant definitions
 */
int count_notation(s21_token* lexeme, unsigned counter, double argument,
                   bool is_arg, double* result);
