
#include <stdio.h>
#include <stdlib.h>

#include "cornertable.h"
#include "edgetable.h"
#include "goal.h"
#include "cube.h"

int make_corner()
{
    FILE *output;
    unsigned char *cornertable = CORNER_TABLE_NEW;
    corner_generate(cornertable, cube_solved);
    output = fopen("table_corner.rht", "w");
    corner_write(cornertable, output);
    fclose(output);
    return 0;
}

int make_edge()
{
    FILE *output;
    unsigned char *edgetable = CORNER_TABLE_NEW;
    edge_generate1(edgetable, cube_solved);
    output = fopen("table_edge1.rht", "w");
    edge_write(edgetable, output);
    fclose(output);
    return 0;
}

/* Reads in tables and starts the solver */
int solve(char *tosolve)
{
    FILE *input;
    unsigned char *corner_table, *edge_table;
    cube_type tosolve_converted;

    corner_table = CORNER_TABLE_NEW;
    input = fopen("table_corner.rht", "r");
    if (!corner_read(corner_table, input))
        return 0;
    fclose(input);

    edge_table = EDGE_TABLE_NEW;
    input = fopen("table_edge1.rht", "r");
    if (!edge_read(edge_table, input))
        return 0;
    fclose(input);

    cube_120convert(tosolve, tosolve_converted);
    printf("Converted cube:\n");
    cube_print(stdout,tosolve_converted);
    goal_solve(tosolve_converted, cube_solved, corner_table, edge_table); 
    return 1;
}

int main(int argc, char **argv)
{
    solve(argv[1]);
    /*
    make_corner();
    make_edge();
    */
    return 0;
}
