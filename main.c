
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

int make_edge(int t)
{
    FILE *output;
    unsigned char *edgetable = CORNER_TABLE_NEW;
    edge_generate(edgetable, cube_solved, t);
    if (t==1)
        output = fopen("table_edge1.rht", "w");
    else
        output = fopen("table_edge2.rht", "w");
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
    if (!corner_read(corner_table, input)) {
        printf("Warning: could not open corner table\n");
        free(corner_table);
        corner_table = NULL;
    }
    fclose(input);

    edge_table = EDGE_TABLE_NEW;
    input = fopen("table_edge1.rht", "r");
    if (!edge_read(edge_table, input)) {
        printf("Warning: could not open edge table\n");
        free(edge_table);
        edge_table = NULL;
    }
    fclose(input);

    cube_120convert(tosolve, tosolve_converted);
    goal_solve(tosolve_converted, cube_solved, corner_table, edge_table); 
    return 1;
}

int main(int argc, char **argv)
{
    int number;
    char cube_raw[121];
    if (argc == 2) {
        solve(argv[1]);
        exit(0);
    }
    printf("What would you like to do?\n");
    printf("1) Solve a cube\n");
    printf("2) Generate the corner table\n");
    printf("3) Generate the first edge table\n");
    printf("4) Generate the second edge table\n");
    printf("Choose: ");
    while (scanf("%d", &number) != 1 || number > 4 || number < 1) {
        printf("\n");
        printf("Please enter a valid choice.\n");
        printf("Choose: ");
        number = getchar();
    }
    printf("\n");
    switch (number) {
        case 1:
            printf("\nPlease enter a raw cube string.\n" \
                    "Raw cube strings are 120 characters long and designate\n"\
                    "the colors on each side of all 20 cubies.\n"\
                    "You can generate this with the cube_convert.py script\n");
            scanf("%120s", cube_raw);
            printf("\n");
            solve(cube_raw);
            break;
        case 2:
            printf("Generating corner cube table\n");
            printf("This can take upwards of 40 minutes.  Ctrl-C to cancel\n");
            printf("\n");
            make_corner();
            break;
        case 3:
            printf("Generating edge cube table\n");
            printf("This can take upwards of 20 minutes.  Ctrl-C to cancel\n");
            printf("\n");
            make_edge(1);
            break;
        case 4:
            printf("Generating edge cube table\n");
            printf("This can take upwards of 20 minutes.  Ctrl-C to cancel\n");
            printf("\n");
            make_edge(2);
            break;
    }

    return 0;
}
