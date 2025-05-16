#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define WIDTH 100
#define HEIGHT 34

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
int cam = 100;    // distance from camera to object
float zdist = 45; // distance from camera to screen
int screenpos;    // position of a point on the screen
float ooz;        // z-buffer 1/z

float x, y, z; // Point coords
float A, B, C; // Angle

//  calculate XYZ for a point
float calcX(int i, int j, int k);
float calcY(int i, int j, int k);
float calcZ(int i, int j, int k);

// Calculate coordinates of a point
void calculatePoint(int i, int j, int k, int sym, char *color);

int main()
{
    float i, j;
    int k;
    system("cls");

    while (1)
    {
        memset(buffer, background, sizeof(buffer));
        memset(zbuffer, 0, sizeof(zbuffer));

        for (i = -cWidth / 2; i < cWidth / 2; i += 0.23)
        {
            for (j = -cWidth / 2; j < cWidth / 2; j += 0.23)
            {
                calculatePoint(i, j, cWidth / 2, '@', "\033[31m");  // Z fixed
                calculatePoint(i, j, -cWidth / 2, '#', "\033[32m"); // -Z fixed
                //calculatePoint(cWidth / 2, i, j, '%', "\033[33m");  // X fixed
                //calculatePoint(-cWidth / 2, i, j, '&', "\033[0m");  // -X fixed
                calculatePoint(i, cWidth / 2, j, 'O', "\033[34m");  // Y fixed
                calculatePoint(i, -cWidth / 2, j, 'H', "\033[35m"); // -Y fixed
            }
        }

        // Move cursor to home position
        printf("%c[%d;%df", 0x1B, 0, 0);

        for (k = 0; k < WIDTH * HEIGHT; k++)
        {
            // Color swap
            if (buffer[k] != ' ')
                printf("%s", colorbuffer[k]);
            else
                printf("\033[0m");

            putchar(k % WIDTH ? buffer[k] : '\n');
        }
        A = 0;     // X rotation
        B = 20;    // Y rotation
        C += 0.1; // Z rotation
        Sleep(10);
    }
}

// TO DO
// REWORK THIS PART AS ONE STRUCT WITH ONE MATRIX

// Calc coordinates of X
float calcX(int i, int j, int k)
{
    float X = j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) + j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);

    return X;
}

// Calc coordinates of Y
float calcY(int i, int j, int k)
{
    float Y = j * cos(A) * cos(C) + k * sin(A) * cos(C) - j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) - i * cos(B) * sin(C);

    return Y;
}

// Calc coordinates of Z
float calcZ(int i, int j, int k)
{
    float Z = k * cos(A) * sin(B) - j * sin(A) * cos(B) + i * sin(B);

    return Z;
}

void calculatePoint(int i, int j, int k, int sym, char *color)
{
    x = calcX(i, j, k);
    y = calcY(i, j, k);
    z = calcZ(i, j, k) + cam;
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