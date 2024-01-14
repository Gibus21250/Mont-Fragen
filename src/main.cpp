#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <random>

#include "vector3.h"
#include "vector2.h"
#include "gen_map.h"

using namespace std;

//global variables OpenGL
int WINDOW, WIDTH, HEIGHT;

bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX, cameraAngleY;
float cameraDistance = 0;

void traceLine(Vector3& v1, Vector3& v2) {
	glBegin(GL_LINES);
    	glVertex3d(v1.x, v1.y, v1.z);
    	glVertex3d(v2.x, v2.y, v2.z);
    glEnd();
}

void drawPoints(vector<GLdouble>& points, const int size) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
            int index = 3 * (i * matSize + j);
			glColor3f(0.0, 1.0, 0.0);
			glPointSize(3.0);
			glBegin(GL_POINTS);
				glVertex3d(points[index], points[index+1], points[index+2]);
			glEnd();
		}
	}
}

void drawWiredScene(vector<vector<Vector3&>>& points, const int size) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			
		}
	}
}

void initOpenGL() {
	// light
    glClearColor(.5, .5, .5, 0.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat l_pos[] = {3., 3.5, 3.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, l_pos);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, l_pos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, l_pos);

    // glDepthFunc(GL_LESS);
    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)200 / (GLfloat)200, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // glScalef(.7,.7,.7);
    gluLookAt(0., 0., 4., 0., 0., 0., 0., 1., 0.);
    // glTranslatef(0.0,0.0,-5.0);

    //generate map geometry
    genPoints();
	genMap();

}

void display_basis() {
    //x axis
	glBegin(GL_LINES);
    glColor3d(1., 0., 0.);
    glVertex3d(0., 0., 0.);
    glVertex3d(1., 0., 0.);
    glEnd();

    //y axis
    glBegin(GL_LINES);
    glColor3d(0., 1., 0.);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., 1., 0.);
    glEnd();

    //z axis
    glBegin(GL_LINES);
    glColor3d(0., 0., 1.);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., 0., 1.);
    glEnd();
}

void main_display(void) {
    glMatrixMode(GL_MODELVIEW);
    /* clearing background */
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0, 0, cameraDistance);
    glRotatef(cameraAngleX, 1., 0., 0.);
    glRotatef(cameraAngleY, 0., 1., 0.);

	display_basis();

	drawPoints(points, matSize);

    glPopMatrix();
	glFlush();
}

void keyboard(unsigned char key, int x, int y) {

}

void mouse(int button, int state, int x, int y) {
	mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }

    else if(button == GLUT_MIDDLE_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseMiddleDown = true;
        }
        else if(state == GLUT_UP)
            mouseMiddleDown = false;
    }
}

void motion(int x, int y) {
	if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance += (y - mouseY) * 0.2f;
        mouseY = y;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
	 /* glut init and window creation */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutInitWindowSize(600, 600);

	WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);

    WINDOW = glutCreateWindow("Fragen Project");

    glutDisplayFunc(main_display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    initOpenGL();

    /* Enter glut's main loop */
    glutMainLoop();

	return 0;
}
