#include <iostream>
#include <GL/glut.h>
#include <vector>
#include <random>

#include "vector3.h"
#include "vector2.h"

using namespace std;

//global variables OpenGL
int WINDOW, WIDTH, HEIGHT;

//size of the map
const double w = 10;
const double h = 5;

//number of iterations
const int N = 5;
//matrix size
const int matSize = 2*N+1;
//controls irregularity
double roughness = 1.0f;
//controls fractal dimension of the mountain
const double h = 10;

vector<vector<double>> height_map(matSize, vector<double>(matSize));
vector<vector<Vector3>> points(matSize, vector<Vector3>(matSize));

void genPoints(vector<vector<Vector3>>& points, const double h, const double w) {
	const double xStep = w / 2*N-1;
	const double yStep = h / 2*N-1;
	
	
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
				) / 4 + rand;
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

	genMap(height_map, matSize, roughness);

	WINDOW = glutCreateWindow("Projet montagne fractales");
	glutReshapeFunc(main_reshape);
	glutDisplayFunc(main_display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutPostRedisplay();
	glutMainLoop();

	return 0;
}
