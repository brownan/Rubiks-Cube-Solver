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
