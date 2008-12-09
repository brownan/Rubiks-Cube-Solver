#ifndef CUBE_H
#define CUBE_H

/**
 * The cube type.  This is made up of a 140 length string, broken up into
 * substrings of length 7, one substring for each of the 20 cubies as shown in
 * the diagram below.
 * Each substring consists of:
 * 1) an integer between 0 and 19 indicating its position (the chart below is
 * off by 1) 
 * 2) 6 characters indicating the colors on the front, top, left, back, bottom,
 * and right sides respectively
 *
 * each color character is one of:
 * (w)hite
 * (r)ed
 * (b)lue
 * (y)ellow
 * (o)range
 * (g)reen)
 * and n for (n)one, indicating that side faces into the cube
 *
 * Cubies are numbered from 0 to 19 throughout the code, but are
 * labeled here as 1 through 20 (I'm not re-doing this ascii diagram) 
 * (the old whichpos function used 1-20 numbers though)
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
#define CUBELEN 140
typedef char cube_type[CUBELEN];

/*
 * Default "solved" cube definition
 */
extern const char cube_solved[];

/*
 * This macro is used to return a pointer into a cube type for the given cubie
 * (sub-cube). Does not include the cubie id byte
 */
#define CUBIE(cube, n) ((char *)cube + (n*7) +1)

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
char *cube_turn(char *to_twist, int direction);

/*
 * This table is used in the heuristics.  For each twist, there are certian
 * twists that should be subsequently avoided.  This helps avoid the situation
 * of turning opposite faces in a different order.  In short, when a turn is
 * performed, this table should be consulted for turns that should NOT be
 * performed right after.
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
 *
 * This is defined in cube.c, delcared here.
 */
extern const long cube_turn_avoid[];

#endif
