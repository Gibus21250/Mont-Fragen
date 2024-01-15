#ifndef GEN_MAP_H
#define GEN_MAP_H

#include <iostream>
#include <vector>
#include <random>
#include <GL/glut.h>

using namespace std;

//size of the map
extern double w;
extern double h;

//number of iterations
extern int N;
//matrix size
extern int matSize;
//controls fractal dimension of the mountain
extern double H;

extern vector<vector<double>> height_map;
extern vector<GLdouble> points;
extern vector<GLint> indexFaces;

double distance(double x1, double y1, double x2, double y2);

//getters to access x, y, z coordinates of the point in the i,j coordinates of the height_map
GLdouble getX(int i, int j);
GLdouble getY(int i, int j);
GLdouble getZ(int i, int j);

//returns the index of the x component, +1 to have y and +2 to have z
int getPointIndexFromHeightMap(int i, int j);

//Generate x and z coordinates of the points (y is altitude determined in genMap)
void genPoints();
void genFaces();

void genMap();

#endif  // GEN_MAP_H