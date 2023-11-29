/* ==========================================================================*/
/* Copyright 2023 © Moscow, Russian Federaion
 *
 * This file contains Original Code created by
 * Ilya Kuzmichev aka wilmerno.
 *
 * The Original Code and all software developed in the process of
 * participation on learning by experimental programming educational method.
 * The whole methodology was developed and distributed by
 * Autonomous non-profit organization «School 21» (ANO «School 21»).
 *
 * Redistribution and use of this file, its parts, or entire project
 * are permitted by confirmation of its original creators.
 */
/* ==========================================================================*/

/**
 * @author Ilya Kuzmichev aka Wilmer Norma
 * @mainpage
 * Ultimate Smart Calculator v 1.0, backend realised in C language, gui written
 * on QT Creator with qcustomplot.h usage for graphics
 *
 * Calculating arbitrary bracketed arithmetic expressions in infix notation.
 *
 * Calculate arbitrary bracketed arithmetic expressions in infix notation with
 * substitution of the value of the variable x as a number.
 *
 * Plotting a graph of a function given by an expression in infix notation with
 * the variable x (with coordinate axes, mark of the used scale and an adaptive
 * grid)
 *
 * Credit and Deposit Calculator
 */

#pragma once

#include <stdbool.h>

#define POINTS_QUANTITY 100000

/**
 * @brief Fucntion for drawing graph (count points)
 *
 * @param lexeme --- text lexeme in infix notation
 * @param scale_x_min --- Lower scale on graph
 * @param scale_x_max --- Higher scale on graph
 * @param result_vector --- pointer on array[][2] with x and y coordinates
 * @return Error code:
 * 0 - OK
 * 1 - Error during parsing or calculating expression
 */
int create_graph(char* lexeme, double scale_x_min, double scale_x_max,
                 double* result_vector);

/**
 * @brief Fucntion for calculating lexeme with polish notation
 *
 * @param lexeme --- text lexeme in infix notation
 * @param argument --- variable value
 * @param is_arg --- bool flag to check input of argument
 * @param result --- poiner to write the result of calculation
 *
 * @return Error code:
 * 0 - OK
 * 1 - Error during parsing or calculating expression
 * 2 - Argument required, but not set
 */
int count_lexeme(char* lexeme, double argument, bool is_arg, double* result);
