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
#ifndef EDGETABLE_H
#define EDGETABLE_H


int edge_hash1(const char *cubestr);
int edge_hash2(const char *cubestr);

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
