#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <random>

#include "vector3.h"
#include "vector2.h"

using namespace std;

//global variables OpenGL
int WINDOW, WIDTH, HEIGHT;

//terrain size
const double h = 100;
const double w = 100;

//number of iterations
const int N = 25;
//matrix size
const int matSize = 2*N+1;
//controls irregularity
const double roughness = 1.0f;

vector<vector<Vector2>> terrain(matSize, vector<Vector2>(matSize));

//diamondSquare algorithm
void genMap(vector<vector<Vector2>>& terrain, const int size, const double roughness) {
	//init corners
	Vector2 v1(	h/2, -w/2);	terrain[0][0] = v1;
	Vector2 v2(	h/2, w/2);	terrain[0][size-1] = v2;
	Vector2 v3(-h/2, -w/2);	terrain[size-1][0] = v3;
	Vector2 v4(-h/2, w/2);	terrain[size-1][size-1] = v4;

	int i = size;
	while(i > 0) {
		int step = i / 2;

		for(int x = 0; x < size; x+step) {
			for(int y = 0; y < size; y+step) {
				Vector2 avg = average(
					terrain[x + step][y + step], 
					terrain[x - step][y - step],
					terrain[x - step][y + step],
					terrain[x + step][y - step]
				);
				random_device rd;  
    			mt19937 gen(rd()); 
    			uniform_real_distribution<> dis(-roughness, roughness);
				avg = avg + dis(gen);
				terrain[x][y] = avg;
			}
		}

		
	}
}

void traceLine(Vector3& v1, Vector3& v2) {
	glBegin(GL_LINES);
    	glVertex3d(v1.x, v1.y, v1.z);
    	glVertex3d(v2.x, v2.y, v2.z);
    glEnd();
}

void main_reshape(int newWidth, int newHeight) {
    WIDTH = newWidth;
    HEIGHT = newHeight;

    GLint viewport[4];
	glViewport(0, 0, WIDTH, HEIGHT);
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, viewport[2], 0.0, viewport[3], -50.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main_display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutPostRedisplay();
    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
    
}

void motion(int x, int y) {

}

int main(int argc, char** argv) {
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);

	WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);

	genMap(terrain, matSize, roughness);

	WINDOW = glutCreateWindow("Projet montagne fractales");
	glutReshapeFunc(main_reshape);
	glutDisplayFunc(main_display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutPostRedisplay();
	glutMainLoop();

	return 0;
}
