#include <stdio.h>

#include "cube.h"

const char cube_solved[] = "\x00nnbyon\x01nnnyon\x02nnnyog\x03nnbynn" \
                           "\x04nnnyng\x05nrbynn\x06nrnynn\x07nrnyng" \
                           "\x08nnbnon\x09nnnnog\x0Anrbnnn\x0Bnrnnng" \
                           "\x0Cwnbnon\x0Dwnnnon\x0Ewnnnog\x0Fwnbnnn" \
                           "\x10wnnnng\x11wrbnnn\x12wrnnnn\x13wrnnng";

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
 * This table is used by cube_turn to determine the cubie numbers after a turn
 * It is two dimensional:
 * The first dimension goes from 0 to 19 representing the old position id
 * The second dimension ranges from 0 to 17 for the turn being performed
 * The value in the table is the new position.
 * XXX Values in table are off by 1 (should subtract 1) OOPS
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
 * Turns the cube.  See comments by declaration in cube.h for more details
 */
char *cube_turn(char *to_twist, int dir)
{
    int turndir;
    char temp;
    int i;
    int swap[4];
    char *cubie;

    if (dir >= 12) {
        /* half turn */
        turndir = 2;
        dir -= 12;
    } else if (dir >= 6) {
        /* CC-wise turn */
        turndir = -1;
        dir -= 6;
    } else {
        /* C-wise turn */
        turndir = 1;
    }
    switch (dir) {
        case 0:
            swap[0] = TOP;
            swap[1] = RIGHT;
            swap[2] = DOWN;
            swap[3] = LEFT;
            break;
        case 1:
            swap[0] = BACK;
            swap[1] = RIGHT;
            swap[2] = FRONT;
            swap[3] = LEFT;
            break;
        case 2:
            swap[0] = TOP;
            swap[1] = FRONT;
            swap[2] = DOWN;
            swap[3] = BACK;
            break;
        case 3:
            swap[0] = TOP;
            swap[1] = LEFT;
            swap[2] = DOWN;
            swap[3] = RIGHT;
            break;
        case 4:
            swap[0] = FRONT;
            swap[1] = RIGHT;
            swap[2] = BACK;
            swap[3] = LEFT;
            break;
        case 5:
            swap[0] = TOP;
            swap[1] = BACK;
            swap[2] = DOWN;
            swap[3] = FRONT;
            break;
        default:
            /* To squelch warnings: */
            swap[0] = 0;
            swap[1] = 0;
            swap[2] = 0;
            swap[3] = 0;
            fprintf(stderr, "WARNING: CUBE TURN SWITCH CASE RAN OFF DEFAULT\n");
            break;

    }
    if (turndir == 1) {
        for (i=0; i<20; i++) {
            cubie = CUBIE(to_twist, i);
            if (cubie[dir] != 'n') {
                /* Turn this cubie clockwise */
                temp = cubie[swap[0]];
                cubie[swap[0]] = cubie[swap[3]];
                cubie[swap[3]] = cubie[swap[2]];
                cubie[swap[2]] = cubie[swap[1]];
                cubie[swap[1]] = temp;
                /* update the cubie position byte */
                cubie[-1] = turn_position_lookup[(int)cubie[-1]][dir];
            }
        }
    } else if (turndir == -1) {
        for (i=0; i<20; i++) {
            cubie = CUBIE(to_twist, i);
            if (cubie[dir] != 'n') {
                /* Turn this cubie counter-clockwise */
                temp = cubie[swap[0]];
                cubie[swap[0]] = cubie[swap[1]];
                cubie[swap[1]] = cubie[swap[2]];
                cubie[swap[2]] = cubie[swap[3]];
                cubie[swap[3]] = temp;
                /* update the cubie position byte */
                cubie[-1] = turn_position_lookup[(int)cubie[-1]][dir+6];
            }
        }
    } else if (turndir == 2) {
        for (i=0; i<20; i++) {
            cubie = CUBIE(to_twist, i);
            if (cubie[dir] != 'n') {
                /* Turn this cubie twice */
                temp = cubie[swap[0]];
                cubie[swap[0]] = cubie[swap[1]];
                cubie[swap[1]] = cubie[swap[2]];
                cubie[swap[2]] = cubie[swap[3]];
                cubie[swap[3]] = temp;
                temp = cubie[swap[0]];
                cubie[swap[0]] = cubie[swap[1]];
                cubie[swap[1]] = cubie[swap[2]];
                cubie[swap[2]] = cubie[swap[3]];
                cubie[swap[3]] = temp;
                /* update the cubie position byte */
                cubie[-1] = turn_position_lookup[(int)cubie[-1]][dir+12];
            }
        }
    }

    return to_twist;
}
    
