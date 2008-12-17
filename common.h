#ifndef COMMON_H
#define COMMON_H
/*
 * common.h has methods used by more than one function
 */

/*
 * Whichpos takes a 6 character string and determines which of the twenty
 * cubies it is by looking at the signature of 'n's it has
 *
 * This method is costly, the huge number of tiny branches probably kill the
 * processor pipeline.  To replace it, I have added a byte to each cubie in the
 * cube string its location, so this method is no longer needed.  The tradeoff
 * is: more memory to store each cube (20 more bytes per cube), and a slightly
 * more expensive turn function for rotating a face (must also update the
 * appropriate cubie bytes).
 *
 * This method should no longer be used in any main loops, but is still
 * useful for doing an initial translation
 *
 */
int whichpos(const char *cubie);

/*
 * Whichrot takes a 6 character string and determines what its rotation is.
 * See cube.h for information on how cubie rotations are defined
 */
int whichrot(const char *cubie);

/*
 * Use this to look up a value in the table.  It works for all tables, since
 * the format is the same and the hash is just an index, so I stuck it here in
 * common.h
 */
#define TABLE_LOOKUP(table, hash) (hash&1 ? \
        table[(hash-1)/2] >> 4 : \
        table[hash/2] & 15)

#endif
