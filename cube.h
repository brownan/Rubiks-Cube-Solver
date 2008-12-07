#ifndef CUBE_H
#define CUBE_H

/**
 * The cube type.  This is made up of a 120 length string, with
 * each character being a color:
 * (w)hite
 * (r)ed
 * (b)lue
 * (y)ellow
 * (o)range
 * (g)reen)
 * and n for (n)one
 *
 * Cubies are numbered from 0 to 19 (each cubie gets 6 characters for the
 * front, top, left, back, bottom, right sides respectively)
 * Cube numbers are as follows:
    6----7----8
    |         |\
    4    Y    5 \
    |         |  \
    1----2----3   \
     \             \
      \   11---R---12
       \  |         |\
        \ B    X    G \
         \|         |  \
          9----O---10   \
           \             \
            \   18--19---20
             \  |         |
              \ 16   W   17
               \|         |
                13--14---15

 */
typedef char cube_type[6*20];

/*
 * Default "solved" cube definition
 */
const char cube_solved[] = "nnbyonnnnyonnnnyognnbynnnnnyngnrbynnnrnynnnrnyngnnbnonnnnnognrbnnnnrnnngwnbnonwnnnonwnnnogwnbnnnwnnnngwrbnnnwrnnnnwrnnng";

/*
 * This macro is used to return a pointer into a cube type for the given
 * cubie (sub-cube)
 */
#define CUBIE(cube, n) ((char *)cube + (n*6))

/*
 * Defines cube sides, they go in this order
 */
#define FRONT 0
#define TOP 1
#define LEFT 2
#define BACK 3
#define DOWN 4
#define RIGHT 5

/*
 * Cube Twists:
 * Twists are numbered from 0 to 17, there are 18 possible twists.
 * 0 through 5 are 90 degree clockwise twists of each of the faces (in order)
 * Twists 6 through 11 are counterclockwise 90 degree twists of the
 * corresponding face (subtract 6 from the twist to get the face it operates
 * on)
 * And twists 12 through 17 are 180 degree twists of the corresponding face
 * (twist minus 12)
 *
 * Edits the cube string in-place
 *
 * Returns a pointer to the cube to_twist on success
 * null on failure
 */
cube_type *cube_turn(cube_type *to_twist, int direction);

/*
 * These tables are used in the heuristics.  For each twist, there are certian
 * twists that should be subsequently avoided.  This helps avoid the situation of
 * turning opposite faces in a different order.
 * In short, when a turn is performed, this table should be consulted for turns
 * that should NOT be performed right after.
 *
 * The table works like this: 
 * cubeavoid is an array, the Mth bit of the Nth item indicates that turn M
 * should not be performed after turning the cube in the N direction.
 * 
 * So to determine if you should turn a direction i after having turned a
 * direction t, do:
 * if (cubeavoid[t] & (1L << i)) {...}
 * in fact, I think I'll make that a macro:
 */
#define SHOULDIAVOID(i, t) (cube_turn_avoid[t] & (1L<<i))
/*
 * Now just do:
 * if (SHOULDIAVOID(i, t)) {...}
 * the macro returns TRUE if you should NOT take that path
 *
 * Side note: these numbers should be at least 18 bits.  An integer is usually
 * 32 bits, but not guaranteed to be so, so I use long here instead.
 */
const long cube_turn_avoid[] = {
                             /* |17<----------->0| */
                    0010101, /* 000001000001000001 */
                    0020202, /* 000010000010000010 */
                    0040404, /* 000100000100000100 */
                    0111111, /* 001001001001001001 */
                    0222222, /* 010010010010010010 */
                    0444444, /* 100100100100100100 */
                    0010101, /* 000001000001000001 */
                    0020202, /* 000010000010000010 */
                    0040404, /* 000100000100000100 */
                    0111111, /* 001001001001001001 */
                    0222222, /* 010010010010010010 */
                    0444444, /* 100100100100100100 */
                    0010101, /* 000001000001000001 */
                    0020202, /* 000010000010000010 */
                    0040404, /* 000100000100000100 */
                    0111111, /* 001001001001001001 */
                    0222222, /* 010010010010010010 */
                    0444444, /* 100100100100100100 */
                    };

#endif
