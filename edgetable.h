#ifndef EDGETABLE_H
#define EDGETABLE_H

/*
 * New in this header, no comments!
 * Find relevant comments by the definitions now
 */

int edge_hash1(const char *cubestr);

/*
 * Use these macros to calloc a new edge table and to clear
 * out a new edge table, respectively
 */
#define EDGE_TABLE_NEW calloc(21288960, 1)
#define EDGE_TABLE_CLEAR(table) memset(table, 255, 21288960)

int edge_generate(unsigned char *edgetable, const char *solution, int tablenum);

/*
 * Dumps the table out to the given file object, open for writing
 */
int edge_write(unsigned char *edgetable, FILE *output);
/*
 * Reads a table in from a file obj open for reading
 */
int edge_read(unsigned char *edgetable, FILE *input);
#endif
