#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <random>

#include "vector3.h"
#include "vector2.h"

using namespace std;

//global variables OpenGL
int WINDOW, WIDTH, HEIGHT;

bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX, cameraAngleY;
float cameraDistance = 0;

//size of the map
const double w = 5;
const double h = 5;

//number of iterations
const int N = 100;
//matrix size
const int matSize = 2*N+1;
//controls irregularity
double roughness = 1.0f;
//controls fractal dimension of the mountain
const double H = 10;

vector<vector<double>> height_map(matSize, vector<double>(matSize));
vector<vector<Vector3>> points(matSize, vector<Vector3>(matSize));

void genPoints(vector<vector<Vector3>>& points, const double h, const double w, const int size) {
	const double xStep = w / 2*N-1;
	const double yStep = h / 2*N-1;
	
	for(int i = 1; i <= size; i++) {
		for(int j = 1; j <= size; j++) {
			Vector3 v(-w/2 + i*xStep, h/2 - j*yStep, 0.0);
			points[i-1][j-1] = v;
		}
	}
}

//diamondSquare algorithm
void genMap(vector<vector<double>>& height_map, const int size, double roughness) {
	//init corners
	random_device rd;  
    mt19937 gen(rd());
	uniform_real_distribution<> dis(0, h);

	height_map[0][0] = dis(gen);
	height_map[0][size-1] = dis(gen);
	height_map[size-1][0] = dis(gen);
	height_map[size-1][size-1] = dis(gen);

	int chunkSize = size - 1;
	while(chunkSize > 1) {
		int half = chunkSize / 2;
		cout << half << endl;

		//square rule
		for(int x = 0; x < size; x+=chunkSize) {
			for(int y = 0; y < size; y+=chunkSize) {
				//rand

				height_map[x+half][y+half] = (	
					height_map[x][y] + 
					height_map[x][y+chunkSize] +
					height_map[x+chunkSize][y] +
					height_map[x+chunkSize][y+chunkSize]
				) / 4;
			}
		}
		
		//diamond rule
		/*for(int x = 0; x < size; x+=half) {
			for(int y = (x + half) % chunkSize; y < size; y+=chunkSize) {
				random_device rd;  
    			mt19937 gen(rd());
				uniform_real_distribution<> dis(-half*roughness, half*roughness);

				height_map[x][y] = (	
					height_map[x+half][y] + 
					height_map[x-half][y] +
					height_map[x][y+half] +
					height_map[x][y-half]
				) / 4 + dis(gen);
			}
		}*/
		
		chunkSize /= 2;
		roughness /= 2;
	}
}

void traceLine(Vector3& v1, Vector3& v2) {
	glBegin(GL_LINES);
    	glVertex3d(v1.x, v1.y, v1.z);
    	glVertex3d(v2.x, v2.y, v2.z);
    glEnd();
}

void drawPoints(vector<vector<Vector3>>& points, const int size) {
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			glColor3f(0.0, 1.0, 0.0);
			glPointSize(10.0);
			glBegin(GL_POINTS);
				glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
			glEnd();
		}
	}
}

void initOpenGL() {
	// light
    glClearColor(.5, .5, 0.5, 0.0);

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
}

void display_basis() {
	glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0., 0.);
    glVertex2f(1., 0.);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0., 0.);
    glVertex2f(0., 1.);
    glEnd();
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0., 0., 0.);
    glVertex3f(0., 0., 1.);
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
    glMatrixMode(GL_MODELVIEW);
    /* clearing background */
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef(0, 0, cameraDistance);
    glRotatef(cameraAngleX, 1., 0., 0.);
    glRotatef(cameraAngleY, 0., 1., 0.);

	display_basis();

	genPoints(points, h, w, matSize);
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
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);

	WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);

	WINDOW = glutCreateWindow("Fragen Project");

	glutReshapeFunc(main_reshape);
	glutDisplayFunc(main_display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	initOpenGL();

	glutMainLoop();

	return 0;
}
