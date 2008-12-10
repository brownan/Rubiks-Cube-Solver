#ifndef GOAL_H
#define GOAL_H
/*
 * goal.c contains routines for actually solving the cube.  Actually only one,
 * the function below takes:
 * 1) the scrambled cube type
 * 2) the solved cube type
 * 3) a pointer to the corner heurisitcs table
 * To come: parameters for pointers to more heuristics tables!  Excitement!
 *
 * The solution is printed to standard out
 */
int goal_solve(const char *scrambled, const char *solved,
        const unsigned char *cornertable);

/*
 * Maximum depth to search for solutions
 * currently proven maximum for the 3x3x3 cube is 25 moves
 */
#define GOAL_MAXDEPTH 25

/*
 * TODO:
 * Make a function that takes a pointer to the path array,
 * and prints to stdout a nice formatted sequence of turns, with
 * letters and all
 */

#endif
