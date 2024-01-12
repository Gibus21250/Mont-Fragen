#include <iostream>
#include <GL/glut.h>
#include <vector>

#include "vector3.h"

using namespace std;

//global variables OpenGL
int WINDOW, WIDTH, HEIGHT;

//terrain size
const double h = 100;
const double w = 100;

//number of iterations
const int N = 100;
//matrix size
const int matSize = pow(2, N) + 1;
//controls irregularity
const double roughness = 1.0f;

vector<vector<Vector3>> terrain(matSize, vector<Vector3>(matSize));

//diamondSquare algorithm
void genMap(vector<vector<Vector3>>& terrain, const int size, const double roughness) {
	//init corners
	Vector3 v1(	h/2, -w/2, 	0.0);	terrain[0][0] = v1;
	Vector3 v2(	h/2, w/2, 	0.0);	terrain[0][size-1] = v2;
	Vector3 v3(-h/2, -w/2, 	0.0);	terrain[size-1][0] = v3;
	Vector3 v4(-h/2, w/2, 	0.0);	terrain[size-1][size-1] = v4;

	
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

	WINDOW = glutCreateWindow("Projet montagne fractales");
	glutReshapeFunc(main_reshape);
	glutDisplayFunc(main_display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutPostRedisplay();
	glutMainLoop();

	return 0;
}
