#include <stdio.h>
#include <string.h>

#include "cube.h"
#include "common.h"

const cube_type cube_solved =  "\x00\x00" "\x01\x00" "\x02\x00" "\x03\x00" \
                               "\x04\x00" "\x05\x00" "\x06\x00" "\x07\x00" \
                               "\x08\x00" "\x09\x00" "\x0A\x00" "\x0B\x00" \
                               "\x0C\x00" "\x0D\x00" "\x0E\x00" "\x0F\x00" \
                               "\x10\x00" "\x11\x00" "\x12\x00" "\x13\x00";

/*
 * Avoids: Turning the same face in any direction that was just turned.
 * Also: for turns on the back, right, or bottom, avoids turns on the opposite
 * face.
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

/*
 * This table is used by cube_turn to determine the cubie position id after a
 * turn
 * It is two dimensional:
 * The first dimension goes from 0 to 19 representing the old position id
 * The second dimension ranges from 0 to 17 for the turn being performed
 * The value in the table is the new position.
 */
static char turn_position_lookup[20][18] = {
            /* clockwise */  /* counter */  /* half-turn */
    /* 0 */ { 0,0,5,2,12,0,  0,0,12,5,2,0,  0,0,17,7,14,0},
            { 1,1,1,4,8,1,   1,1,1,3,9,1,  1,1,1,6,13,1},
            { 2,2,2,7,0,14,  2,2,2,0,14,7,  2,2,2,5,12,19},
    /* 3 */ { 3,3,10,1,3,3,  3,3,8,6,3,3,   3,3,15,4,3,3},
            { 4,4,4,6,4,9,  4,4,4,1,4,11,  4,4,4,3,4,16},
    /* 5 */ { 5,7,17,0,5,5,  5,17,0,7,5,5,  5,19,12,2,5,5},
            { 6,11,6,3,6,6,  6,10,6,4,6,6,  6,18,6,1,6,6},
            { 7,19,7,5,7,2,  7,5,7,2,7,19,  7,17,7,0,7,14},
    /* 8 */ { 8,8,3,8,13,8,  8,8,15,8,1,8,  8,8,10,8,9,8},
            { 9,9,9,9,1,16,  9,9,9,9,13,4,  9,9,9,9,8,11},
            { 10,6,15,10,10,10,  10,18,3,10,10,10,  10,11,8,10,10,10},
            { 11,18,11,11,11,4,  11,6,11,11,11,16,  11,10,11,11,11,9},
    /* 12 */{ 17,12,0,12,14,12,  14,12,17,12,0,12,  19,12,5,12,2,12},
            { 15,13,13,13,9,13, 16,13,13,13,8,13,  18,13,13,13,1,13},
            { 12,14,14,14,2,19,  19,14,14,14,12,2,  17,14,14,14,0,7},
    /* 15 */{ 18,15,8,15,15,15,  13,15,10,15,15,15, 16,15,3,15,15,15},
            { 13,16,16,16,16,11, 18,16,16,16,16,9, 15,16,16,16,16,4},
    /* 17 */{ 19,5,12,17,17,17,  12,19,5,17,17,17,  14,7,0,17,17,17},
            { 16,10,18,18,18,18, 15,11,18,18,18,18, 13,6,18,18,18,18},
            { 14,17,19,19,19,7,  17,7,19,19,19,14,  12,5,19,19,19,2},
};

/*
 * I love tables.
 * This table is for determining which cubies should be turned for any
 * particular face turn.  The first dimension is the cubie you're considering,
 * and the second dimenstion is the face of the cube being turned (0-5).
 * The result is either a yes (1, turn this face), or a no (0, don't turn)
 */
static char turn_this_cubie[20][6] = {
    /* 0 */ { 0,0,1,1,1,0 },
            { 0,0,0,1,1,0 },
            { 0,0,0,1,1,1 },
            { 0,0,1,1,0,0 },
            { 0,0,0,1,0,1 },
    /* 5 */ { 0,1,1,1,0,0 },
            { 0,1,0,1,0,0 },
            { 0,1,0,1,0,1 },
    /* 8 */ { 0,0,1,0,1,0 },
            { 0,0,0,0,1,1 },
            { 0,1,1,0,0,0 },
            { 0,1,0,0,0,1 },
    /* 12 */{ 1,0,1,0,1,0 },
            { 1,0,0,0,1,0 },
            { 1,0,0,0,1,1 },
            { 1,0,1,0,0,0 },
            { 1,0,0,0,0,1 },
            { 1,1,1,0,0,0 },
            { 1,1,0,0,0,0 },
            { 1,1,0,0,0,1 }
};

/*
 * I REALLY love tables (I have to think it beats a big if statement for things
 * like this)
 * This one maps cubie IDs to if it's a corner cubie or not
 * 1 for yes, 0 for no
 */
static char corner_cubies[] = {1,0,1,0,0,1,0,1,0,0,0,0,1,0,1,0,0,1,0,1};

int cube_120convert(const char *input, char *output)
{
    int i;
    input++;
    for (i=0; i<20; ++i) {
        /* put in the position byte */
        CUBIE(output, i)[0] = (char) whichpos(input+i*6);
        /* put in the rotation byte */
        CUBIE(output, i)[1] = (char) whichrot(input+i*6);
    }
    return 1;
}

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
char *cube_turn(char *to_twist, int turn)
{
    int rotamt;
    int i, c;
    int face;
    char *cubie;

    if (turn >= 12) {
        /* half turn */
        rotamt = 0; /* half turns don't change corner rotations */
        face = turn - 12;
    } else if (turn >= 6) {
        /* CC-wise turn */
        rotamt = 1;
        face = turn - 6;
    } else {
        /* C-wise turn */
        rotamt = 1;
        face = turn;
    }
    for (i=0,c=0; i<20 && c<8; ++i) {
        /* turn each cubie */
        cubie = CUBIE(to_twist,i);
        if (turn_this_cubie[(int)cubie[0]][face]) {
            /* Yes, we're turning this one */
            ++c;
            if (corner_cubies[i]) {
                /* And it's a corner cubie */
                /* update rotation */
                switch (face) {
                    case FRONT:
                    case BACK:
                        /* 
                         * If rot is 0, no change 
                         * else, toggle between 1 and 2
                         */
                        cubie[1] += rotamt;
                        if (cubie[1] > 2) {
                            cubie[1] = 1;
                        }
                        break;
                    case TOP:
                    case DOWN:
                        /*
                         * if rot is 2, no change,
                         * else toggle between 0 and 1
                         */
                        cubie[1] -= rotamt;
                        if (cubie[1] < 0) {
                            cubie[1] = 1;
                        }
                        break;
                    case LEFT:
                    case RIGHT:
                        /*
                         * if rot is 1, no change,
                         * else toggle between 0 and 2
                         */
                        cubie[1] += rotamt << 1;
                        if (cubie[1] > 2) {
                            cubie[1] = 0;
                        }
                        break;
                }
                /* now update position */
                cubie[0] = turn_position_lookup[(int)cubie[0]][turn];
            } else {
                /* it's an edge cubie */
                switch (face) {
                    case LEFT:
                    case RIGHT:
                        cubie[1] = (!cubie[1]);
                }
                cubie[0] = turn_position_lookup[(int)cubie[0]][turn];
            }
        }
    }

    return to_twist;
}
    
