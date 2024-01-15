#include <iostream>
#include <vector>
#include <random>
#include <GL/glut.h>

#include "gen_map.h"

double w = 50;
double h = 50;
int N = 4;
int matSize = (2*N)+1;
double H = 150;

vector<vector<double>> height_map(matSize, vector<double>(matSize, 0.0));
vector<GLdouble> points(matSize * matSize * 3, 0.0);
//formula to get 2 faces per square of the matrix, *3 because each face has 3 points
vector<GLint> indexFaces((matSize-1)*(matSize-1) * 2 * 3, 0); 

double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

GLdouble getX(int i, int j) {
    int index = 3 * (i * matSize + j);
    return points[index];
}

GLdouble getY(int i, int j) {
    int index = 3 * (i * matSize + j);
    return points[index+1];
}

GLdouble getZ(int i, int j) {
    int index = 3 * (i * matSize + j);
    return points[index+2];
}

int getPointIndexFromHeightMap(int i, int j) {
    return 3 * (i * matSize + j);
}

void genFaces() {
    for(int i = 0; i < matSize-2; i++) {
        for(int j = 0; j < matSize-2; j++) {
            int p1Index = i * matSize + j;
            int p2Index = p1Index + 1;
            int p3Index = (i + 1) * matSize + j;
            int p4Index = p3Index+1; 
        }
    }
}

void genPoints() {
	const double xStep = w / (matSize-1);
	const double yStep = h / (matSize-1);
	
	for(int i = 0; i < matSize; i++) {
		for(int j = 0; j < matSize; j++) {
			int index = 3 * (i * matSize + j);
			points[index] = -w/2 + i*xStep;         //x
            points[index+1] = 0.0;                  //y
            points[index+2] = h/2 - j*yStep;        //z
		}
	}

    genFaces();
}

//diamondSquare algorith
void genMap() {
	random_device rd;  
    mt19937 gen(rd());
	uniform_real_distribution<> disInit(0, H);
    uniform_real_distribution<> disHeightRand(-1, 1);

	//4 corners initialized
	height_map[0][0] = disInit(gen);                    points[1] = height_map[0][0];                                                   //top left
	height_map[0][matSize-1] = disInit(gen);            points[(3*matSize)-2] = height_map[0][matSize-1];                            //top right
	height_map[matSize-1][0] = disInit(gen);            points[(matSize*matSize*3) - (matSize*3) + 1] = height_map[matSize-1][0];    //bottom left
	height_map[matSize-1][matSize-1] = disInit(gen);    points[(matSize*matSize*3) - 2] = height_map[matSize-1][matSize-1];       //bottom right

    int chunkSize = matSize - 1;
    while(chunkSize > 1) {
        int half = chunkSize / 2;

        //square rule
        for(int x = half; x < matSize; x += chunkSize) {
            for(int y = half; y < matSize; y += chunkSize) {
                double avg = (
                    height_map[x - half][y - half] +
                    height_map[x + half][y + half] +
                    height_map[x - half][y + half] +
                    height_map[x + half][y - half]
                ) / 4;

                double dist = distance(getX(x, y), getZ(x, y), getX(x+half, y+half), getZ(x+half, y+half));
                height_map[x][y] = avg + (disHeightRand(gen) * dist * pow(2, -H));
                points[getPointIndexFromHeightMap(x, y)+1] = height_map[x][y]; 
            }
        }

        int shift = 0;
        //diamond rule
        for(int x = 0; x < matSize; x += half) {
            if(shift == 0) shift = half;
            else shift = 0;

            for(int y = shift; y < matSize; y += chunkSize) {
                double sum = 0;
                int n = 0;
                double dist = 0;

                if(x >= half) {
                    sum += height_map[x - half][y];
                    n++;
                    dist = distance(getX(x, y), getZ(x, y), getX(x-half, y), getZ(x-half, y));
                }
                if(x + half < matSize) {
                    sum += height_map[x + half][y];
                    n++;
                    dist = distance(getX(x, y), getZ(x, y), getX(x+half, y), getZ(x+half, y));
                }
                if(y >= half) {
                    sum += height_map[x][y - half];
                    n++;
                    dist = distance(getX(x, y), getZ(x, y), getX(x, y-half), getZ(x, y-half));
                }
                if(y + half > matSize) {
                    sum += height_map[x][y + half];
                    n++;
                    dist = distance(getX(x, y), getZ(x, y), getX(x, y+half), getZ(x, y+half));
                }

                double avg = sum / n;
                height_map[x][y] = avg + (disHeightRand(gen) * dist * pow(2, -H));
                points[getPointIndexFromHeightMap(x, y)+1] = height_map[x][y];
            }
        }
        chunkSize = half;
    }
}