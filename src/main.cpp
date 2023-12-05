#include <iostream>

#include <GL/glut.h>

int WINDOW, WIDTH, HEIGHT;

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