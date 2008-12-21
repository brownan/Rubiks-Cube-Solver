#ifndef CORNERTABLE_H
#define CORNERTABLE_H
#include "cube.h"

/*
 * The actual table.
 * I couldn't think of a better way to do this, so run this macro
 * to malloc the space for the table
 * This should be put into an UNSIGNED CHAR POINTER
 */
#define CORNER_TABLE_NEW calloc(44089920, 1)
#define CORNER_TABLE_CLEAR(table) memset(table, 255, 44089920)

int corner_hash(const char *cubetohash);

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
