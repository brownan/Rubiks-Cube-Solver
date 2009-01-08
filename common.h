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
#ifndef COMMON_H
#define COMMON_H

int whichpos(const char *cubie);

int whichrot(const char *cubie);

/*
 * Use this to look up a value in the table.  It works for all tables, since
 * the format is the same and the hash is just an index, so I stuck it here in
 * common.h
 */
#define TABLE_LOOKUP(table, hash) (hash&1 ? \
        table[(hash-1)>>1] >> 4 : \
        table[hash/2] & 15)

#endif
