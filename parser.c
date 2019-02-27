#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

char ** parse_args( char * cmd ){
    char ** arg_list = calloc(sizeof(char*), 25);
    for(int i = 0; (arg_list[i] = strsep(&cmd, " ")); i++){
        // If the arg is an empty string (extra spaces), remove it
        if(*arg_list[i] == 0){
            i--;
        }
    }
    return arg_list;
}

/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every line is a single character that takes up a line
     Any line that requires arguments must have those arguments in the second line.
     The lines are as follows:
         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         translate: create a translation matrix,
                    then multiply the transform matrix by the translation matrix -
                    takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge matrix
         display: clear the screen, then
                  draw the lines of the edge matrix to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge matrix to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file(char * filename, struct matrix * transform, struct matrix * edges, screen s) {
    FILE * f;
    char line[256];
    char ** args;

    struct color c;
    c.red = 255;

    clear_screen(s);

    if (strcmp(filename, "stdin") == 0)
        f = stdin;
    else
        f = fopen(filename, "r");

    while (fgets(line, 255, f) != NULL) {
        line[strlen(line) - 1] = '\0';

        if (strcmp(line, "line") == 0) {
            fgets(line, 1024, f);
            args = parse_args(line);
            add_edge(edges, atof(args[0]), atof(args[1]), atof(args[2]), atof(args[3]), atof(args[4]), atof(args[5]));
        } else if (strcmp(line, "ident") == 0) {
            ident(transform);
        } else if (strcmp(line, "scale") == 0) {
            fgets(line, 1024, f);
            args = parse_args(line);
            struct matrix * m = new_matrix(4, 4);
            scale(m, atof(args[0]), atof(args[1]), atof(args[2]));
            matrix_mult(m, transform);
            free_matrix(m);
        } else if (strcmp(line, "move") == 0) {
            fgets(line, 1024, f);
            args = parse_args(line);
            struct matrix * m = new_matrix(4, 4);
            translate(m, atof(args[0]), atof(args[1]), atof(args[2]));
            matrix_mult(m, transform);
            free_matrix(m);
        } else if (strcmp(line, "apply") == 0) {
            matrix_mult(transform, edges);
        } else if (strcmp(line, "rotate") == 0) {
            fgets(line, 1024, f);
            args = parse_args(line);
            struct matrix * m = new_matrix(4, 4);

            if (args[0][0] == 'x') {
                rotateX(m, atof(args[1]));
            } else if (args[0][0] == 'y') {
                rotateY(m, atof(args[1]));
            } else if (args[0][0] == 'z') {
                rotateZ(m, atof(args[1]));
            } else {
                printf("s% is not a valid axis of rotation\n", args[0]);
                return 1;
            }

            matrix_mult(m, transform);
            free_matrix(m);
        } else if (strcmp(line, "save") == 0) {
            fgets(line, 1024, f);
            args = parse_args(line);

            clear_screen(s);
            draw_lines(edges, s, c);
            save_extension(s, args[0]);
            printf("Image saved to %s\n", args[0]);
        } else if(strcmp(line, "quit") == 0) {
            return;
        }
    }
}