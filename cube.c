#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define WIDTH 100
#define HEIGHT 40

// Size of cube
const float cWidth = 40;

// Buffers for output points
char buffer[WIDTH * HEIGHT];
// Coordinates of projection z-buffering
float zbuffer[WIDTH * HEIGHT];
// Buffer for color
char *colorbuffer[WIDTH * HEIGHT];

int background = ' ';
int xproj, yproj; // projection position
int cam = 110;    // distance from camera to object
float zdist = 40; // distance from camera to screen
int screenpos;    // position of a point on the screen
float ooz;        // z-buffer 1/z

float x, y, z; // Point coords
float A, B, C; // Angle

// Calculate coordinates of a point
void calculatePoint(int i, int j, int k, int sym, char *color);

int main()
{
    float i, j;
    int k;
    char *lastcolor, *newcolor;

    system("cls");

    while (1)
    {
        memset(buffer, background, sizeof(buffer));
        memset(zbuffer, 0, sizeof(zbuffer));

        for (i = -cWidth / 2; i < cWidth / 2; i += 0.34)
        {
            for (j = -cWidth / 2; j < cWidth / 2; j += 0.34)
            {
                calculatePoint(i, j, cWidth / 2, '@', "\033[31m");  // Z fixed
                calculatePoint(i, j, -cWidth / 2, '#', "\033[32m"); // -Z fixed
                calculatePoint(cWidth / 2, i, j, '%', "\033[33m");  // X fixed
                calculatePoint(-cWidth / 2, i, j, '&', "\033[0m");  // -X fixed
                calculatePoint(i, cWidth / 2, j, 'O', "\033[34m");  // Y fixed
                calculatePoint(i, -cWidth / 2, j, 'H', "\033[35m"); // -Y fixed
            }
        }

        // Move cursor to home position
        printf("%c[%d;%df", 0x1B, 0, 0);

        // Coordinates of X Y Z
        printf("%0.2f %0.2f %0.2f\n", x, y, z);

        // Print all points
        for (k = 0; k < WIDTH * HEIGHT; k++)
        {
            // Color swap
            newcolor = (buffer[k] != ' ') ? colorbuffer[k] : "\033[0m";
            if (strcmp(lastcolor, newcolor) != 0)
            {
                lastcolor = newcolor;
                printf("%s", newcolor);
            }

            // Print Char
            putchar(k % WIDTH ? buffer[k] : '\n');
        }

        C += 0.05; // X rotation (roll)
        B += 0.05; // Y rotation (pitch)
        A += 0.05; // Z rotation (yaw)
        Sleep(10);
    }
}

// TO DO
// REWORK THIS PART AS ONE STRUCT WITH ONE MATRIX

void calculatePoint(int i, int j, int k, int sym, char *color)
{

    // Sines and Cosines
    float sinA = sin(A), cosA = cos(A); // X-axis rotation
    float sinB = sin(B), cosB = cos(B); // Y-axis rotation
    float sinC = sin(C), cosC = cos(C); // Z-axis rotation

    // Rotate around Z
    float x1 = i;
    float y1 = j * cosA - k * sinA;
    float z1 = j * sinA + k * cosA;

    // Rotate around Y
    float x2 = x1 * cosB + z1 * sinB;
    float y2 = y1;
    float z2 = -x1 * sinB + z1 * cosB;

    // Rotate around X
    float x3 = x2 * cosC - y2 * sinC;
    float y3 = x2 * sinC + y2 * cosC;
    float z3 = z2;

    x = x3;
    y = y3;
    z = z3 + cam;
    ooz = 1 / z;
    
    xproj = (int)(WIDTH / 2 + x * zdist * ooz * 2);
    yproj = (int)(HEIGHT / 2 + y * zdist * ooz);
    screenpos = xproj + yproj * WIDTH;

    if (screenpos >= 0 && screenpos < WIDTH * HEIGHT)
    {
        if (ooz > zbuffer[screenpos])
        {
            zbuffer[screenpos] = ooz;
            buffer[screenpos] = sym;
            colorbuffer[screenpos] = color;
        }
    }
}