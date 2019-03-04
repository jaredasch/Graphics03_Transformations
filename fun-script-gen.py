# Python code used to generate a script file

from math import sin, cos, pi, sqrt

f = open("fun-script" , "w")

def line(x1, y1, z1, x2, y2, z2):
    f.write("line\n%d %d %d %d %d %d\n" % (x1, y1, z1, x2, y2, z2))

def circleX(n, r, x, y, z):
    for a in range(0, n):
        line(x, y + r * cos(2 * pi * a / n), z + r * sin(2 * pi * a / n), x, y + r * cos(2 * pi * (a+1) / n), z + r * sin(2 * pi * (a+1) / n))

def circleY(n, r, x, y, z):
    for a in range(0, n):
        line(x + r * cos(2 * pi * a / n), y, z + r * sin(2 * pi * a / n), x + r * cos(2 * pi * (a+1) / n), y, z + r * sin(2 * pi * (a+1) / n))

def circleZ(n, r, x, y, z):
    for a in range(0, n):
        line(x + r * cos(2 * pi * a / n), y + r * sin(2 * pi * a / n), z, x + r * cos(2 * pi * (a+1) / n), y + r * sin(2 * pi * (a+1) / n), z)

circleX(100, 100, 100, 100, 0)
circleX(100, 50 * sqrt(3), 50, 100, 0)
circleX(100, 50 * sqrt(3), 150, 100, 0)

circleY(100, 50 * sqrt(3), 100, 50, 0)
circleY(100, 50 * sqrt(3), 100, 150, 0)
circleY(100, 100, 100, 100, 0)

circleZ(100, 50 * sqrt(3), 100, 100, -50)
circleZ(100, 50 * sqrt(3), 100, 100, 50)
circleZ(100, 100, 100, 100, 0)

f.write("ident\nrotate\nx 10\nrotate\ny 10\nrotate\nz 10\napply\n")
f.write("move\n200 200 0\napply\n")
f.write("save\npic.png\n")
