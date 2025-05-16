#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define WIDTH 100
#define HEIGHT 34

// Size of cube
const float cWidth = 40;

// Buffers for image storage
char buffer[WIDTH * HEIGHT];
// Coordinates of projection z-buffering
float zbuffer[WIDTH * HEIGHT];
int background = ' ';
int xproj, yproj; // projection position
int cam = 100;
float zdist = 40; // distance from camera to screen
int screenpos;    // position of a point on the screen
float ooz;        // z-buffer 1/z

float x, y, z; // Point coords
float A, B, C; // Angle

//  calculate XYZ for a point
float calcX(int i, int j, int k);
float calcY(int i, int j, int k);
float calcZ(int i, int j, int k);

// Calculate coordinates of a point
void calculatePoint(int i, int j, int k, int sym);

int main()
{
    float i, j;
    int k;
    system("cls");

    while (1)
    {
        memset(buffer, background, WIDTH * HEIGHT);
        memset(zbuffer, 0, WIDTH * HEIGHT * 4);

        for (i = -cWidth / 2; i < cWidth / 2; i += 0.23)
        {
            for (j = -cWidth / 2; j < cWidth / 2; j += 0.23)
                calculatePoint(i, j, -cWidth / 2, '@');
        }

        // Move cursor to home position
        printf("%c[%d;%df", 0x1B, 0, 0);

        for (k = 0; k < WIDTH * HEIGHT; k++)
        {
            putchar(k % WIDTH ? buffer[k] : 10);
        }
        A += 0.03;
        B += 0.03;
        C += 0.05;
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

void calculatePoint(int i, int j, int k, int sym)
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
        }
    }
}