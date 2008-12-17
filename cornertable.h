#ifndef CORNERTABLE_H
#define CORNERTABLE_H
#include "cube.h"
/*
 * cornertable
 * this has functions to generate the 88179840 element corner heuristics table,
 * as well as methods to use it
 */

/*
 * The actual table.
 * I couldn't think of a better way to do this, so run this macro
 * to malloc the space for the table
 * This should be put into an UNSIGNED CHAR POINTER
 */
#define CORNER_TABLE_NEW calloc(44089920, 1)
#define CORNER_TABLE_CLEAR(table) memset(table, 255, 44089920)

/*
 * corner_hash
 * Input: a cube_type string
 * Output: An integer in the range 0 to 88179840-1
 *
 * This method is hopefully now less expensive.  Calling is now encouraged!
 * (just kidding)
 *
 * The general method is to process the first 7 of the 8 corner cubies.
 * For the first corner cube, since it has 24 possible positions, the
 * hash space is split into 24 even sections (3674160 elements long)
 * A number is calculated 0 through 23 that represents which position
 * the first corner cubie is in, and multiplied by 3674160, which becomes
 * the index.
 *
 * Then the second corner cube is similar, except the remaining space (which
 * is 3674160 elements long) is split into 21 equal parts, since the second
 * corner cubie has 21 possible positions.  That works out to 174960.  So,
 * similarly, a number 0-20 is calculated and multiplied by 174960 and then
 * added to the index.  This is repeated for each of the first 7 corner
 * cubies. The eighth cubie has only 1 possible position, so it is not
 * included in any calculations. (It cannot be rotated but one way since
 * the cube would not be solvable)
 *
 * This method takes about 37 mins on my 2GHz Intel T7200
 */
int corner_hash(const char *cubetohash);

/*
 * Generates the corner heuristics table about the given solution, which
 * could be any valid cube
 */
int corner_generate(unsigned char *cornertable, const char *solution);

/*
 * Dumps the table out to the given file object, open for writing
 */
int corner_write(unsigned char *cornertable, FILE *output);
/*
 * Reads a table in from a file obj open for reading
 */
int corner_read(unsigned char *cornertable, FILE *input);
#endif
