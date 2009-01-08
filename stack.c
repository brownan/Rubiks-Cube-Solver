/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Copyright © 2009 Andrew Brown <brownan@gmail.com>
*/
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "cube.h"

/*
 * This is a custom stack data structure that is meant to take as one structure the
 * following information:
 * a cube type (120 length string)
 * an integer indicating what turn formed this cube
 * an integer indicating the distance from the start
 */


/*
 * This pushes a cube to the stack,
 * pass in a ptr to the cube str, the turn used to produce this cube, and the
 * distance.  The information will be pushed onto the stack
 *
 * Copies the cube string given by cube_to_append
 *
 * Returns 1 on success, 0 on failure
 */
int stack_push(stacktype *stack, const char *cube_to_append, int turn, int distance)
{
    qdata *newq;
    if (stack->rightblock == NULL){
        /* initialize */
        stack->rightblock = malloc(sizeof(block));
        stack->rightblock->leftlink = NULL;
        stack->length = 0;
        stack->rightindex = -1;
    }
    else if (stack->rightindex == BLOCKLEN-1) {
        /* Needs a new block */
        block *newblock = malloc(sizeof(block));
        newblock->leftlink = stack->rightblock;
        stack->rightblock = newblock;
        stack->rightindex = -1;
    }
    stack->length++;
    stack->rightindex++;
    /* Now copy data in */
    newq = &(stack->rightblock->data[stack->rightindex]);
    memcpy(newq->cube_data, cube_to_append, CUBELEN);
    newq->turn = turn;
    newq->distance = distance;
    return 1;
}

/*
 * This stack works a bit differently.  There are operations to individually
 * peek at the different items at the top of the stack, but the stack remains
 * unchanged.  Then the pop operation doesn't return anything, but just
 * destroys the topmost element.  This is done so that I don't deal with
 * references and all that, most all data transfer is done right on the stack.
 *
 * These could even be macros, but -finline-functions probably takes care of it
 *
 * stack_pop returns 1 on success, 0 on failure
 */
int stack_pop(stacktype *stack)
{
    /* Just erase the last element */
    stack->length--;
    if (stack->rightindex == 0) {
        /* Freeing an entire block */
        block *oldblock = stack->rightblock;
        stack->rightblock = oldblock->leftlink;
        free(oldblock); /* goodbye */
        stack->rightindex = BLOCKLEN-1;
    } else {
        stack->rightindex--;
    }
    return 1;
}

int stack_peek_cube(stacktype *stack, cube_type *targetcube)
{
    memcpy(targetcube, stack->rightblock->data[stack->rightindex].cube_data, CUBELEN);
    return 1;
}
int stack_peek_turn(stacktype *stack)
{
    return stack->rightblock->data[stack->rightindex].turn;
}
    
int stack_peek_distance(stacktype *stack)
{
    return stack->rightblock->data[stack->rightindex].distance;
}
