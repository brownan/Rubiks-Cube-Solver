#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "edgetable.h"
#include "cube.h"
#include "stack.h"

/*
 * This file holds two edge hashing functions.  For any six of the twelve edge
 * cubies, there are 42577920 possible permutations.  There are two hash
 * functions, one for the first 6 edge cubies, and another for the second.
 *
 * There will be two tables, both are admissible heuristics for solving the
 * cube.
 *
 * I'd like to try and find a generalized way of hashing an arbitrary subset of
 * cubies, but I don't think it's feasable. So I just need to write a new
 * function for every subset.
 */

/*
 * This hashes the edge cubies
 * 1, 3, 4, 6, 8, and 9
 */
int edge_hash1(const char *cubestr)
{
    char positions[] = {1, 3, 4, 6, 8, 9, 10, 11, 13, 15, 16, 18};
    int index = 0;
    int pos, rot, num, i;
    
    const char *cubie;

    /*
     * First split hash space (42577920) into 24 slices:
     * 1774080
     * then into 22 slices:
     * 80640
     * and so forth (each cubie eliminates 2 positions):
     * 4032
     * 224
     * 14
     * 1
     */
    
    /* first */
    cubie = CUBIE(cubestr, 1);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<11; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 12;
    /* 
     * pos is range 0-11
     * rot is 0 or 12
     * so pos+rot has range 0-23 as expected
     */
    index += (pos+rot) * 1774080;
#ifdef DEBUG_ASSERTS
    if (pos>11 || (rot != 0 && rot != 12)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        fprintf(stderr, "Pos: %d, Rot: %d\n", pos, rot);
        fprintf(stderr, "Cubie[] = [%d, %d]\n", (int)cubie[0], (int)cubie[1]);
        cube_print(stderr, cubestr);
        fprintf(stderr, "Positions:\n");
        for (i=0; i<12; ++i)
            fprintf(stderr, "%d ", positions[i]);
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* second */
    cubie = CUBIE(cubestr, 3);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<10; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 11;
    /* 
     * pos is range 0-10
     * rot is 0 or 11
     * so pos+rot has range 0-21
     */
    index += (pos+rot) * 80640;
#ifdef DEBUG_ASSERTS
    if (pos>10 || (rot != 0 && rot != 11)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* third */
    cubie = CUBIE(cubestr, 4);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<9; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 10;
    /* 
     * pos is range 0-9
     * rot is 0 or 10
     * so pos+rot has range 0-19
     */
    index += (pos+rot) * 4032;
#ifdef DEBUG_ASSERTS
    if (pos>9 || (rot != 0 && rot != 10)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* fourth */
    cubie = CUBIE(cubestr, 6);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<8; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 9;
    /* 
     * pos is range 0-8
     * rot is 0 or 9
     * so pos+rot has range 0-17
     */
    index += (pos+rot) * 224;
#ifdef DEBUG_ASSERTS
    if (pos>8 || (rot != 0 && rot != 9)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* fifth */
    cubie = CUBIE(cubestr, 8);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<7; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 8;
    /* 
     * pos is range 0-7
     * rot is 0 or 8
     * so pos+rot has range 0-15
     */
    index += (pos+rot) * 14;
#ifdef DEBUG_ASSERTS
    if (pos>7 || (rot != 0 && rot != 8)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* sixth */
    cubie = CUBIE(cubestr, 9);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<6; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 7;
    /* 
     * pos is range 0-6
     * rot is 0 or 7
     * so pos+rot has range 0-13
     */
    index += (pos+rot);
#ifdef DEBUG_ASSERTS
    if (pos>6 || (rot != 0 && rot != 7)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

#ifdef DEBUG_ASSERTS
    if (index >= 42577920) {
        fprintf(stderr, "\nWARNING: HASH RETURNED %d\n", index);
        index = *((int *)0x0); /* sigsev */
    }
#endif

    return index;
}


/*
 * This hashes the edge cubies
 * 10, 11, 13, 15, 16, and 18
 */
int edge_hash2(const char *cubestr)
{
    char positions[] = {1, 3, 4, 6, 8, 9, 10, 11, 13, 15, 16, 18};
    int index = 0;
    int pos, rot, num, i;
    
    const char *cubie;

    /*
     * First split hash space (42577920) into 24 slices:
     * 1774080
     * then into 22 slices:
     * 80640
     * and so forth (each cubie eliminates 2 positions):
     * 4032
     * 224
     * 14
     * 1
     */
    
    /* first */
    cubie = CUBIE(cubestr, 10);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<11; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 12;
    /* 
     * pos is range 0-11
     * rot is 0 or 12
     * so pos+rot has range 0-23 as expected
     */
    index += (pos+rot) * 1774080;
#ifdef DEBUG_ASSERTS
    if (pos>11 || (rot != 0 && rot != 12)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        fprintf(stderr, "Pos: %d, Rot: %d\n", pos, rot);
        fprintf(stderr, "Cubie[] = [%d, %d]\n", (int)cubie[0], (int)cubie[1]);
        cube_print(stderr, cubestr);
        fprintf(stderr, "Positions:\n");
        for (i=0; i<12; ++i)
            fprintf(stderr, "%d ", positions[i]);
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* second */
    cubie = CUBIE(cubestr, 11);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<10; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 11;
    /* 
     * pos is range 0-10
     * rot is 0 or 11
     * so pos+rot has range 0-21
     */
    index += (pos+rot) * 80640;
#ifdef DEBUG_ASSERTS
    if (pos>10 || (rot != 0 && rot != 11)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* third */
    cubie = CUBIE(cubestr, 13);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<9; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 10;
    /* 
     * pos is range 0-9
     * rot is 0 or 10
     * so pos+rot has range 0-19
     */
    index += (pos+rot) * 4032;
#ifdef DEBUG_ASSERTS
    if (pos>9 || (rot != 0 && rot != 10)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* fourth */
    cubie = CUBIE(cubestr, 15);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<8; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 9;
    /* 
     * pos is range 0-8
     * rot is 0 or 9
     * so pos+rot has range 0-17
     */
    index += (pos+rot) * 224;
#ifdef DEBUG_ASSERTS
    if (pos>8 || (rot != 0 && rot != 9)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* fifth */
    cubie = CUBIE(cubestr, 16);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<7; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 8;
    /* 
     * pos is range 0-7
     * rot is 0 or 8
     * so pos+rot has range 0-15
     */
    index += (pos+rot) * 14;
#ifdef DEBUG_ASSERTS
    if (pos>7 || (rot != 0 && rot != 8)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

    /* sixth */
    cubie = CUBIE(cubestr, 18);
    num = cubie[0]; /* position id on the cube */
    /* find its index in the positions array */
    pos = 0;
    while (num != positions[pos])
        ++pos;
    /* remove that pos from the array */
    for (i=pos; i<6; ++i)
        positions[i] = positions[i+1];
    rot = cubie[1] * 7;
    /* 
     * pos is range 0-6
     * rot is 0 or 7
     * so pos+rot has range 0-13
     */
    index += (pos+rot);
#ifdef DEBUG_ASSERTS
    if (pos>6 || (rot != 0 && rot != 7)) {
        fprintf(stderr, "\nWARNING: BAD POS/ROT\n");
        index = *((int *)0x0); /* sigsev */
    }
#endif

#ifdef DEBUG_ASSERTS
    if (index >= 42577920) {
        fprintf(stderr, "\nWARNING: HASH RETURNED %d\n", index);
        index = *((int *)0x0); /* sigsev */
    }
#endif

    return index;
}


/*
 * Similar to corner_generate, this generates the hash table for the edge
 * cubies.  There are two edge cubie tables to choose from.  The tablenum
 * variable should specify which table is being generated.  Either 1 or 2.
 *
 * Mostly copied from corner_generate
 */
int edge_generate(unsigned char *table, const char *solution, int tablenum)
{
    stacktype *stack;
    /*
     * qdata is a struct, holding cube string 'cube_data',
     * an int representing the turn that was made, 'turn',
     * and an int representing the distance, 'distance'
     */
    qdata current;
    int count = 0; /* total hashed */
    int popcount = 0; /* total traversed */
    int hash;
    int i;
    unsigned char *instack;
    int depth;
    cube_type turned;

    /* Which edge hashing function to use */
    int (*hashfunc)(const char *);

    switch (tablenum) {
        case 1:
            hashfunc = edge_hash1;
            break;
        case 2:
            hashfunc = edge_hash2;
            break;
        default:
            return 0;
    }

    /* Create a stack */
    stack = STACK_NEW;

    /*
     * create a temporary table to keep track of the stack This holds the value
     * of each item that has been added to the stack, and its distance.  This
     * way, we know if we encounter the same node but at a further distance we
     * can throw it away.  This heuristic cuts down on processing time by quite
     * a bit, and is only as time intensive as the hashing algorithm, at the
     * expense of using more memory.
     */
    instack = EDGE_TABLE_NEW;

    depth = -1;
    while (count < 42577920)
    {
        /* if stack is empty, go up a level */
        if (stack->length == 0)
        {
            stack_push(stack, solution, -1, 0);
            depth++;
            /* clear out instack table */
            EDGE_TABLE_CLEAR(instack);
        }

        /* Pop the first item off, put it in current */
        stack_peek_cube(stack, &(current.cube_data));
        current.turn = stack_peek_turn(stack);
        current.distance = stack_peek_distance(stack);
        stack_pop(stack);
        popcount++;

        /* Print out status every 2^18 pops  (approx every 200k)*/
        if ((popcount & 0777777) == 0777777) {
            fprintf(stderr, "\r%d/42577920 hashed, on level:%d/10, total traversed:%d %d%% ", count, depth, popcount, popcount/2752511);
        }
#ifdef PROFILE_MODE
        /* For profiling, so I don't have to wait an hour to gather data */
        if (count == 10000000) {
            return 0;
        }
#endif

        /*
         * if item is at our current target depth, add it to hash table
         */
        if (current.distance == depth) {
            hash = hashfunc(current.cube_data);
            if (hash & 1) {
                if (!(table[(hash-1)/2] >> 4)) {
                    table[(hash-1)/2] |= current.distance << 4;
                    count++;
                } else {
                    /* A duplicate, skip */
                }
            } else {
                if (!(table[hash/2]&15)) {
                    table[hash/2] |= current.distance;
                    count++;
                } else {
                    /* a duplicate */
                }
            }
        } else {
            /* Not at the current depth, put all turns onto the stack */
            for (i=0; i<18; i++) {
                /* Determine if we should skip this turn */
                if (current.turn != -1 && SHOULDIAVOID(i, current.turn)) {
                    continue;
                }

                memcpy(turned, current.cube_data, CUBELEN);
                cube_turn(turned, i);

                /*
                 * Check if turned is in instack and is greater than
                 * or equal to the depth.  If so, skip
                 */
                hash = hashfunc(turned);
                if (hash&1 ? \
                        ((instack[(hash-1)/2] >> 4) <= (current.distance+1)) : \
                        ((instack[hash/2] & 15) <= (current.distance+1))) {
                    continue;
                }
                /* add to instack */
                if (hash&1) {
                    instack[(hash-1)/2] &= 15;
                    instack[(hash-1)/2] |= (current.distance+1) << 4;
                } else {
                    instack[hash/2] &= 15<<4;
                    instack[hash/2] |= (current.distance+1);
                }

                /* Add to real stack */
                stack_push(stack, turned, i, current.distance+1);
            }
        }

    }

    /*
     * This sets the value for the solved cube to 0, as it should be.  This
     * exceptional case comes about because there are moves which don't change
     * the hash (this table only involves 6 of the 12 edge cubies.)  Therefore,
     * an edge is turned, and while the edge cubies are still "solved", an
     * entry is inserted in the table with distance 1, which ruins the
     * admissibility of the heuristic.
     *
     * This wasn't an issue with the corner table because every turn from the
     * solved state brought it to an unsolved state (unlike this table).  So
     * the 18 cubes that are 1 position away from solved go into the 'instack'
     * table, which prevents the search from coming back to the solved state.
     * The 18 moves around the solved state form a kind of barrier.
     */
    hash = hashfunc(cube_solved);
    if (hash & 1) {
        /* zero out upper bits */
        table[(hash-1)/2] &= 15;
    } else {
        /* zero out lower bits */
        table[hash/2] &= (15<<4);
    }


    free(instack);
    while(stack->length) {
        stack_pop(stack);
    }
    free(stack);
    fprintf(stderr, "\n");
    return 1;
}

int edge_write(unsigned char *edgetable, FILE *output)
{
    int written;
    written = fwrite(edgetable, 1, 21288960, output);
    if (written < 21288960)
        return 0;
    return 1;
}
int edge_read(unsigned char *edgetable, FILE *input)
{
    int read;
    read = fread(edgetable, 1, 21288960, input);
    if (read < 21288960) {
        return 0;
    }
    return 1;
}
