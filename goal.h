#ifndef GOAL_H
#define GOAL_H
int goal_solve(const char *scrambled, const char *solved,
        const unsigned char *cornertable,
        const unsigned char *edgetable1,
        const unsigned char *edgetable2);

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
