Optimal Rubik's Cube Solver
Written and Copyright © 2008-2009 by Andrew Brown <brownan@gmail.com>

REQUIREMENTS
============
* A C compiler w/ standard libraries, I use gcc
* make
* python to run the cube_convert.py script

TO COMPILE
==========
1. Ensure you have a C compiler installed as well as standard library headers.
   You will also need the "make" program. No other fancy libraries are needed.

2. Run the "make" command in the source directory to compile all the files.
   This will produce a program called "main" which is used to generate the
   heuristics tables and to solve the cube

RUNNING THE PROGRAM
===================
First you will need to generate the heuristics tables. This could take a while.

1. Run the compiled program with "./solver"
2. Select one of the three table generation options.
3. Wait.
4. Repeat 1-3 for the other two tables 

Verify the table generation worked with these md5 signatures:

61db7e6c5f73a6e96abdb96c182c6da3
    table_corner.rht

28577b26564c1b12ec6bc2ce61fa2ef4
    table_edge1.rht

2164d32703941aee8a4218bae595d944
    table_edge2.rht

Next obtain a scrambled cube to solve. The solver expects a sort of cryptic
format, so I have provided a script where you just enter the colors on each
side and it spits out the "cube string". 

Run the cube_convert.py script. It will give you this template:

::

                  1   2   3

                  4   R   5

                  6   7   8

     9  10  11   12  13  14   15  16  17    18  19  20

    21   B  22   23   W  24   25   G  26    27   Y  28

    29  30  31   32  33  34   35  36  37    38  39  40

                 41  42  43

                 44   O  45

                 46  47  48

It will ask you to enter in the colors of each face in the shown order. Note
that you don't enter the center faces, those are fixed, so skip over those.
For example, a solved cube would be entered like this:
``rrrrrrrrbbbwwwgggyyybbwwggyybbbwwwgggyyyoooooooo`` Which gets translated to::

    nnbyonnnnyonnnnyognnbynnnnnyngnrbynnnrnynnnrnyngnnbnonnnnnog
    nrbnnnnrnnngwnbnonwnnnonwnnnogwnbnnnwnnnngwrbnnnwrnnnnwrnnng

(except all on one line) 

Finally, to solve a cube:
Run the compiled program "./solver" and choose the solve a cube option.
Paste in your translated 120 character cube string all on one line and press
enter.

Wait for solutions!

NOTES
=====
I was re-writing how the heuristics tables are computed when I last worked on
this. Specifically, I changed how a cube configuration was mapped onto an index
into the table. You can see the new version in corner_table.c, but I never got
around to changing it for the two edge tables.

Also, this was like a year ago, so I don't remember if my new version was
extensively tested. If you find it doesn't work, try checking out the version
tagged "v1.2", which I'm fairly sure works. It's been so long though I don't
remember what works and what doesn't.

OTHER
=====
I wrote this program completely from scratch, using ideas from Richard Korf's
papers and other various internet resources. Any ideas, interesting comments,
or suggestions for improvements please email me!
-Andrew Brown <brownan@gmail.com>
