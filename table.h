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

    Copyright © 2010 Andrew Brown <brownan@gmail.com>
*/

#ifndef TABLE_H
#define TABLE_H

#include "cube.h"

/*
 * This struct defines a heuristic that can be used in the solver
 *
 * A heuristic is defined by:
 * * Its mapping function, which takes a cube configuration string and returns
 *   an int
 *
 * * The number of entries stored in the table. Each entry is stored in 4 bits.
 *
 * * The maximum depth of this table, used only for the output
 *
 * * And the filename that should store this table
 */
typedef struct {
    int (*map_function)(const char *);
    int num_entries;
    int max_level;
    char *filename;
} heuristic;

/**
 * Method prototypes, see table.c for details
 */
unsigned char *table_generate(heuristic *heuristic_info);
void table_save(heuristic *heuristic_info);
unsigned char *table_load(heuristic *heruistic_info);

#endif
