
#include <stdio.h>
#include <stdlib.h>

#include "cornertable.h"

int main()
{
    unsigned char *cornertable = CORNER_TABLE_NEW;
    corner_generate(cornertable, cube_solved);
    return 0;
}
