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
 * Input: a 120 byte cube string
 * Output: An integer in the range 0 to 88179840-1
 */
int corner_hash(const char *cubetohash);

/*
 * This gets the stored value from the table
 */
char corner_lookup(const unsigned char *cornertable, const char *cubetohash);

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
