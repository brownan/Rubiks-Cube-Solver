#include "common.h"

/*
 * See more comments in common.h
 */
int whichpos(char *cubie)
{
    /* Takes a 6 character cubie string and returns which cubie it is,
     * one through twenty.  cubie is NOT null terminated (this func only
     * touches the first 6 chars)
     * */
    int ns[4] = {-1,-1,-1,-1};
    int i, j=0;
    for (i=0; i<6; i++) {
        if (cubie[i] == 'n')
            ns[j++] = i;
    }
    /* Just a big if statement will suffice: */
    if (ns[0]==0 && ns[1]==1 && ns[2]==5 && ns[3]==-1)
        return 1;
    else if (ns[0]==0 && ns[1]==1 && ns[2]==2 && ns[3]==5)
        return 2;
    else if (ns[0]==0 && ns[1]==1 && ns[2]==2 && ns[3]==-1)
        return 3;
    else if (ns[0]==0 && ns[1]==1 && ns[2]==4 && ns[3]==5)
        return 4;
    else if (ns[0]==0 && ns[1]==1 && ns[2]==2 && ns[3]==4)
        return 5;
    else if (ns[0]==0 && ns[1]==4 && ns[2]==5 && ns[3]==-1)
        return 6;
    else if (ns[0]==0 && ns[1]==2 && ns[2]==4 && ns[3]==5)
        return 7;
    else if (ns[0]==0 && ns[1]==2 && ns[2]==4 && ns[3]==-1)
        return 8;
    else if (ns[0]==0 && ns[1]==1 && ns[2]==3 && ns[3]==5)
        return 9;
    else if (ns[0]==0 && ns[1]==1 && ns[2]==2 && ns[3]==3)
        return 10;
    else if (ns[0]==0 && ns[1]==3 && ns[2]==4 && ns[3]==5)
        return 11;
    else if (ns[0]==0 && ns[1]==2 && ns[2]==3 && ns[3]==4)
        return 12;
    else if (ns[0]==1 && ns[1]==3 && ns[2]==5 && ns[3]==-1)
        return 13;
    else if (ns[0]==1 && ns[1]==2 && ns[2]==3 && ns[3]==5)
        return 14;
    else if (ns[0]==1 && ns[1]==2 && ns[2]==3 && ns[3]==-1)
        return 15;
    else if (ns[0]==1 && ns[1]==3 && ns[2]==4 && ns[3]==5)
        return 16;
    else if (ns[0]==1 && ns[1]==2 && ns[2]==3 && ns[3]==4)
        return 17;
    else if (ns[0]==3 && ns[1]==4 && ns[2]==5 && ns[3]==-1)
        return 18;
    else if (ns[0]==2 && ns[1]==3 && ns[2]==4 && ns[3]==5)
        return 19;
    else if (ns[0]==2 && ns[1]==3 && ns[2]==4 && ns[3]==-1)
        return 20;
    
    return -1;
}
