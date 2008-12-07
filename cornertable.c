#include <string.h>

#include "cornertable.h"

int corner_hash(const cube_type *cubetohash)
{

    return 0;
}

/*
 * This implementation is pretty simple.
 * I'm thinking it may be better to just copy+paste this to wherever you
 * need it, maybe it'll save a stack call or something, or maybe it won't
 * matter
 */
char corner_lookup(const char *cornertable, const cube_type *c)
{
    int hash = corner_hash(c);
    return hash&1 ? cornertable[(hash-1)/2] >> 4 : \
        cornertable[hash/2] & 15;
}
