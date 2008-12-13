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
int whichpos(char *cubie);

#endif
