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
const double w = 100;
const double h = 100;

//number of iterations
const int N = 20;
//matrix size
const int matSize = 2*N+1;
//controls fractal dimension of the mountain
const double H = 10;

vector<vector<double>> height_map(matSize, vector<double>(matSize));
vector<vector<Vector3>> points(matSize, vector<Vector3>(matSize));

void genPoints(vector<vector<Vector3>>& points, const double h, const double w, const int size) {
	const double xStep = w / (size-1);
	const double yStep = h / (size-1);
	
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			Vector3 v(-w/2 + i*xStep, 0.0,  h/2 - j*yStep);
			points[i][j] = v;
		}
	}
}

double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void printPoints(vector<vector<Vector3>>& points, const int size) {
    for(int i = 0; i<size; i++) {
        for(int j = 0; j<size; j++) {
            points[i][j].display();
        }
    }
}

//diamondSquare algorithm
void genMap(vector<vector<Vector3>>& points, vector<vector<double>>& height_map, const int size) {
	random_device rd;  
    mt19937 gen(rd());
	uniform_real_distribution<> disInit(0, H);
    uniform_real_distribution<> disHeightRand(-1, 1);

	//4 corners initialized
	height_map[0][0] = disInit(gen);                points[0][0].y = height_map[0][0];
	height_map[0][size-1] = disInit(gen);           points[0][size-1].y = height_map[0][size-1];
	height_map[size-1][0] = disInit(gen);           points[size-1][0].y = height_map[size-1][0];
	height_map[size-1][size-1] = disInit(gen);      points[size-1][size-1].y = height_map[size-1][size-1];

    int chunkSize = size - 1;
    while(chunkSize > 1) {
        int half = chunkSize / 2;

        //square rule
        for(int x = half; x < size; x += chunkSize) {
            for(int y = half; y < size; y += chunkSize) {
                double avg = (
                    height_map[x - half][y - half] +
                    height_map[x + half][y + half] +
                    height_map[x - half][y + half] +
                    height_map[x + half][y - half]
                ) / 4;

                height_map[x][y] = avg + (disHeightRand(gen) * distance(points[x][y].x, points[x][y].z, points[x+half][y+half].x, points[x+half][y+half].z) * pow(2, -H));
                points[x][y].y = height_map[x][y];
            }
        }

        int shift = 0;
        //diamond rule
        for(int x = 0; x < size; x+=chunkSize) {
            if(shift == 0) shift = chunkSize;
            else shift = 0;

            for(int y = shift; y < size; y+=chunkSize) {
                double sum = 0;
                int n = 0;
                double dist = 0;

                if(x >= half) {
                    sum += height_map[x - half][y];
                    n++;
                    dist = distance(points[x][y].x, points[x][y].z, points[x - half][y].x, points[x - half][y].z);
                }
                if(x + half < size) {
                    sum += height_map[x + half][y];
                    n++;
                    dist = distance(points[x][y].x, points[x][y].z, points[x + half][y].x, points[x + half][y].z);
                }
                if(y >= half) {
                    sum += height_map[x][y - half];
                    n++;
                    dist = distance(points[x][y].x, points[x][y].z, points[x][y - half].x, points[x][y - half].z);
                }
                if(y + half > size) {
                    sum += height_map[x][y + half];
                    n++;
                    dist = distance(points[x][y].x, points[x][y].z, points[x][y + half].x, points[x][y + half].z);
                }

                double avg = sum / n;
                height_map[x][y] = avg + (disHeightRand(gen) * dist * pow(2, -H));
                points[x][y].y = height_map[x][y];
            }
        }
        chunkSize = half;
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
			glPointSize(3.0);
			glBegin(GL_POINTS);
				glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
			glEnd();
		}
	}
}

void drawWiredScene(vector<vector<Vector3>>& points, const int size) {
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
    genPoints(points, h, w, matSize);
	genMap(points, height_map, matSize);
    printPoints(points, matSize);

}

void display_basis() {
	glBegin(GL_LINES);
    glColor3d(1., 0., 0.);
    glVertex3d(0., 0., 0.);
    glVertex3d(1., 0., 0.);
    glEnd();

    glBegin(GL_LINES);
    glColor3d(0., 1., 0.);
    glVertex3d(0., 0., 0.);
    glVertex3d(0., 1., 0.);
    glEnd();

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
