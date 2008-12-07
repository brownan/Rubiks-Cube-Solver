#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "cornertable.h"
#include "common.h"
#include "stack.h"

int corner_hash(const char *cubestr)
{
    /* The seven relevant corner cubies.  These are NOT null terminated */
    char cb1[6];
    char cb2[6];
    char cb3[6];
    char cb4[6];
    char cb5[6];
    char cb6[6];
    char cb7[6];
    
    
    char positions[] = {1,3,6,8,13,15,18,20};
    int index = 0;
    int pos, rot,num, i;

    memcpy(cb1, cubestr+0, 6);
    memcpy(cb2, cubestr+12, 6);
    memcpy(cb3, cubestr+30, 6);
    memcpy(cb4, cubestr+42, 6);
    memcpy(cb5, cubestr+72, 6);
    memcpy(cb6, cubestr+84, 6);
    memcpy(cb7, cubestr+102, 6);
    
    /* First cubie */
    /* whichpos returns the position id of this cubie */
    num = whichpos(cb1);
    pos=0;
    /* Find its index in positions array.  In this case, it's
     * a number 0-7, for the next it's 0-6, and so fourth */
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    /* Remove that element from the array (that position is occupied
     * so no other cubie can be in it) */
    for (i=pos; i< 8-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    /* Munge the cb1 cubie string such that the first and second index
     * are not 'n', they are the first and second respective colors
     * that appear in the string */
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb1[i] != 'n') {
            cb1[0] = cb1[i];
            num++;
        } else if (num == 1 && cb1[i] != 'n') {
            cb1[1] = cb1[i];
            break;
        }
    }
    /* Identify the rotation of the cubie by finding which face has one of
     * the colors.  Blue in this case I believe is arbitrary, it could be
     * any color that appears in this cubie (if I remember correctly). */
    if (cb1[0] == 'b') rot = 0;
    else if (cb1[1] == 'b') rot = 1;
    else rot = 2;
    /* 3674160 = 21*18*15*12*9*6 */
    /* max(pos+rot*8) = 24 */
    /* so index after this line will be one one of the boundaries if the
     * hash space is split in 24 (24 places this cubie could go)
     * remember hash space is
     * 88179840 = 24*21*18*15*12*9*6
     */
    index += (pos + rot*8) * 3674160;
    /* The next cubie will add to the index such that the remaining hash
     * space (3674160) is split into 21 parts
     * and so fourth
     */
    
    /* second */
    num = whichpos(cb2);
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 7-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb2[i] != 'n') {
            cb2[0] = cb2[i];
            num++;
        } else if (num == 1 && cb2[i] != 'n') {
            cb2[1] = cb2[i];
            break;
        }
    }
    if (cb2[0] == 'y') rot = 0;
    else if (cb2[1] == 'y') rot = 1;
    else rot = 2;
    index += (pos + rot*7) * 174960;
    
    /* third */
    num = whichpos(cb3);
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 6-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb3[i] != 'n') {
            cb3[0] = cb3[i];
            num++;
        } else if (num == 1 && cb3[i] != 'n') {
            cb3[1] = cb3[i];
            break;
        }
    }
    if (cb3[0] == 'r') rot = 0;
    else if (cb3[1] == 'r') rot = 1;
    else rot = 2;
    index += (pos + rot*6) * 9720;

    /* fourth */
    num = whichpos(cb4);
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 5-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb4[i] != 'n') {
            cb4[0] = cb4[i];
            num++;
        } else if (num == 1 && cb4[i] != 'n') {
            cb4[1] = cb4[i];
            break;
        }
    }
    if (cb4[0] == 'r') rot = 0;
    else if (cb4[1] == 'r') rot = 1;
    else rot = 2;
    index += (pos + rot*5) * 648;

    /* fifth */
    num = whichpos(cb5);
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 4-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb5[i] != 'n') {
            cb5[0] = cb5[i];
            num++;
        } else if (num == 1 && cb5[i] != 'n') {
            cb5[1] = cb5[i];
            break;
        }
    }
    if (cb5[0] == 'w') rot = 0;
    else if (cb5[1] == 'w') rot = 1;
    else rot = 2;
    index += (pos + rot*4) * 54;

    /* sixth */
    num = whichpos(cb6);
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    for (i=pos; i< 3-1; i++)
        positions[i] = positions[i+1];
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb6[i] != 'n') {
            cb6[0] = cb6[i];
            num++;
        } else if (num == 1 && cb6[i] != 'n') {
            cb6[1] = cb6[i];
            break;
        }
    }
    if (cb6[0] == 'w') rot = 0;
    else if (cb6[1] == 'w') rot = 1;
    else rot = 2;
    index += (pos + rot*3) * 6;

    /* seventh */
    num = whichpos(cb7);
    pos=0;
    while (num != positions[pos])
        pos++; /* This will segfault if it falls off the end of the array.
                that would be an error anyways. */
    num = 0;
    for (i=0; i<6; i++)
    {
        if (num == 0 && cb7[i] != 'n') {
            cb7[0] = cb7[i];
            num++;
        } else if (num == 1 && cb7[i] != 'n') {
            cb7[1] = cb7[i];
            break;
        }
    }
    if (cb7[0] == 'w') rot = 0;
    else if (cb7[1] == 'w') rot = 1;
    else rot = 2;
    index += (pos + rot*2);
    
    return index;

}

/*
 * This implementation is pretty simple.
 * I'm thinking it may be better to just copy+paste this to wherever you
 * need it, maybe it'll save a stack call or something, or maybe it won't
 * matter
 */
char corner_lookup(const char *cornertable, const char *cubetohash)
{
    int hash = corner_hash(cubetohash);
    return hash&1 ? cornertable[(hash-1)/2] >> 4 : \
        cornertable[hash/2] & 15;
}

/* Returns 1 on success */
int corner_generate(char *cornertable, const char *solution)
{
    stacktype *stack;
    /*
     * qdata is a struct, holding 120 byte cube string 'cube_data',
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
    int lastturned;
    cube_type turned;


    /* Create a stack */
    stack = malloc(sizeof(stacktype));
    stack->rightblock = NULL; /* so it knows it's empty */

    /* create a temporary table to keep track of the stack */
    instack = CORNER_TABLE_NEW;

    depth = -1;
    while (count < 88179840)
    {
        /* if stack is empty, go up a level */
        if (stack->length == 0)
        {
            lastturned = -1;
            stack_push(stack, solution, -1, 0);
            depth++;
            /* clear out instack table */
            CORNER_TABLE_CLEAR(instack);
        }

        /* Pop the first item off, put it in current */
        stack_peek_cube(stack, &(current.cube_data));
        current.turn = stack_peek_turn(stack);
        current.distance = stack_peek_distance(stack);
        stack_pop(stack);
        popcount++;

        if (popcount % 100000 == 0) {
            fprintf(stderr, "\r%d/88179840 hashed, on level:%d/11, total traversed:%d ", count, depth, popcount);
        }

        /*
         * if item is at our current target depth, add it to hash table
         */
        if (current.distance == depth) {
            hash = corner_hash(current.cube_data);
            if (hash & 1) {
                if (!(cornertable[(hash-1)/2] >> 4)) {
                    cornertable[(hash-1)/2] |= current.distance << 4;
                    count++;
                } else {
                    /* A duplicate, skip */
                }
            } else {
                if (!(cornertable[hash/2]&15)) {
                    cornertable[hash/2] |= current.distance;
                    count++;
                } else {
                    /* a duplicate */
                }
            }
        } else {
            /* Not at the current depth, put all turns onto the stack */
            for (i=0; i<18; i++) {
                /* Determine if we should skip this turn */
                /* XXX */

                memcpy(turned, current.cube_data, 120);
                cube_turn(turned, i);

                /*
                 * Check if turned is in instack and is greater than
                 * or equal to the depth.  If so, skip
                 */
                hash = corner_hash(turned);
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
    return 1;
}
