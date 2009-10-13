/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Copyright © 2009 Andrew Brown <brownan@gmail.com>
*/
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

int corner_map(const char *cubetohash);

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
