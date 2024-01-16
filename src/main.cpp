#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "shader.hpp"
#include <string.h>

#include "gen_map.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;
using namespace std;

const int N = 8; // Correspond à la résolutions

double w = 128; // largeur de la matrice dans le monde
double h = 128; // longueur de la matrice dans le monde
double maxHauteur = 25;

const int nbVertex = (pow(2, N) + 1) * (pow(2, N) + 1);
int nbTriangle = 0;

glm::vec3 *tSommets;
glm::vec3 *tNormales;
glm::uvec3 *tIndices;
glm::vec3 *tColors;

glm::vec3 tEauSommets[] = {
    {-w/2, 0, -h/2},
    {-w/2, 0, h/2},
    {w/2, 0, -h/2},
    {w/2, 0, h/2}};

glm::uvec3 tEauIndices[] = {
    {0, 1, 2},
    {2, 1, 3}};

glm::vec3 tEauNormales[] = {
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0}};

void initBuffers();
void clearBuffers();
void afficheInfoData();

// initialisations
void genereVBO();
void deleteVBO();
void traceMontagne();
void traceEau();

// fonctions de rappel de glut
void affichage();
void clavier(unsigned char, int, int);
void mouse(int, int, int, int);
void mouseMotion(int, int);
void reshape(int, int);

// variables globales pour OpenGL
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance = 0.;

// variables Handle d'opengl
//--------------------------
GLuint montagneProg;
GLuint MatrixIDMVP, MatrixIDView, MatrixIDModel, MatrixIDPerspective; // handle pour la matrice MVP
GLuint VBO_sommets, VBO_normales, VBO_Colors, VBO_indices, VAO_Montagne;

GLuint locCameraPosition;
GLuint locmaterialShininess;
GLuint locmaterialSpecularColor;

GLuint eauProg;
GLuint MatrixIDMVP_eau, MatrixIDModel_eau, Hauteur_eau_uniform;
GLuint VBO_sommets_eau, VBO_normales_eau, VBO_indices_eau, VAO_Eau;
GLfloat Hauteur_eau = 0;

struct LightInfoGPU
{
  GLuint locLightPosition;
  GLuint locLightIntensities;
  GLuint locLightAttenuation;
  GLuint locLightAmbientCoefficient;
} LightInfoGPU;

struct LightInfoCPU
{
  glm::vec3 position = glm::vec3(1., 1, 1);
  glm::vec3 intensity = vec3(1., 1., 1.);
  GLfloat attenuation = 1;
  GLfloat ambientCoeff = .5;
} LightInfoCPU;

// location des VBO
//------------------
GLuint indexVertex = 0, indexNormale = 1, indexColors = 2;
GLuint indexVertexEau = 0, indexNormaleEau = 1;

// variable pour paramétrage eclairage
//--------------------------------------
vec3 cameraPosition(1, 1, 10.);
// le matériau
//---------------
GLfloat materialShininess = 3.;
vec3 materialSpecularColor(1., .1, 1); // couleur du materiau

glm::mat4 MVP;                     // justement la voilà
glm::mat4 Model, View, Projection; // Matrices constituant MVP

int screenHeight = 720;
int screenWidth = 1280;

// pour la texcture
//-------------------
GLuint image;
GLuint bufTexture, bufNormalMap;
GLuint locationTexture, locationNormalMap;

void initBuffers()
{
  tSommets = (glm::vec3 *)malloc(nbVertex * sizeof(glm::vec3));
  tColors = (glm::vec3 *)malloc(nbVertex * sizeof(glm::vec3));
  tNormales = (glm::vec3 *)malloc(nbVertex * sizeof(glm::vec3));
}

void clearBuffers()
{
  free(tSommets);
  free(tNormales);
  free(tIndices);
  free(tColors);
}
//----------------------------------------
void initOpenGL(void)
//----------------------------------------
{
  glCullFace(GL_BACK);    // on spécifie queil faut éliminer les face arriere
  glEnable(GL_CULL_FACE); // on active l'élimination des faces qui par défaut n'est pas active
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // le shader
  montagneProg = LoadShaders("shaders/PhongShader.vert", "shaders/PhongShader.frag");

  // Get  handles for our matrix transformations "MVP" VIEW  MODELuniform
  MatrixIDMVP = glGetUniformLocation(montagneProg, "MVP");
  MatrixIDView = glGetUniformLocation(montagneProg, "VIEW");
  MatrixIDModel = glGetUniformLocation(montagneProg, "MODEL");
  // MatrixIDPerspective = glGetUniformLocation(programID, "PERSPECTIVE");

  // Projection matrix : 65 Field of View, 1:1 ratio, display range : 1 unit <-> 1000 units
  // ATTENTIOn l'angle est donné en radians si f GLM_FORCE_RADIANS est défini sinon en degré
  Projection = glm::perspective(glm::radians(90.f), 1.0f, 1.0f, 1000.0f);

  /* on recupere l'ID */
  locCameraPosition = glGetUniformLocation(montagneProg, "cameraPosition");

  LightInfoGPU.locLightAmbientCoefficient = glGetUniformLocation(montagneProg, "l_ambientCoefficient");
  LightInfoGPU.locLightPosition = glGetUniformLocation(montagneProg, "l_position");
  LightInfoGPU.locLightIntensities = glGetUniformLocation(montagneProg, "l_intensity"); // a.k.a the color of the light
  LightInfoGPU.locLightAttenuation = glGetUniformLocation(montagneProg, "l_attenuation");

  // EAU
  eauProg = LoadShaders("shaders/WaterShader.vert", "shaders/WaterShader.frag");

  MatrixIDMVP_eau = glGetUniformLocation(eauProg, "MVP");
  MatrixIDModel_eau = glGetUniformLocation(eauProg, "MODEL");
  Hauteur_eau_uniform = glGetUniformLocation(eauProg, "Hauteur_eau");
}
//----------------------------------------
int main(int argc, char **argv)
//----------------------------------------
{

  /* initialisation de glut et creation
     de la fenetre */

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(200, 200);
  glutInitWindowSize(screenWidth, screenHeight);
  glutCreateWindow("Mont Fragen");

  // Initialize GLEW
  if (glewInit() != GLEW_OK)
  {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  // info version GLSL
  std::cout << "***** Info GPU *****" << std::endl;
  std::cout << "Fabricant : " << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Carte graphique: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version : " << glGetString(GL_VERSION) << std::endl;
  std::cout << "Version GLSL : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

  initBuffers();

  initOpenGL();
  initPoints(tSommets, tColors, N, w, h);
  nbTriangle = initFaces(&tIndices, N);

  generateDiamondSquare(tSommets, N, maxHauteur);

  computeNormales(tNormales, tSommets, tIndices, N, nbTriangle);

  // construction des VBO a partir des tableaux du cube deja construit
  genereVBO();

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotion);

  /* Entree dans la boucle principale glut */
  glutMainLoop();

  clearBuffers();
  glDeleteProgram(montagneProg);
  glDeleteProgram(eauProg);
  deleteVBO();

  return 0;
}

void genereVBO()
{
  glGenVertexArrays(1, &VAO_Montagne);
  glBindVertexArray(VAO_Montagne);

  if (glIsBuffer(VBO_sommets) == GL_TRUE)
    glDeleteBuffers(1, &VBO_sommets);
  glGenBuffers(1, &VBO_sommets);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_sommets);
  cout << "taille sommets: " << nbVertex * sizeof(glm::vec3) << "\n";
  glBufferData(GL_ARRAY_BUFFER, nbVertex * sizeof(glm::vec3), tSommets, GL_STATIC_DRAW);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(pTest), pTest, GL_STATIC_DRAW);
  glVertexAttribPointer(indexVertex, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  if (glIsBuffer(VBO_normales) == GL_TRUE)
    glDeleteBuffers(1, &VBO_normales);
  glGenBuffers(1, &VBO_normales);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_normales);
  cout << "taille normales: " << nbVertex * sizeof(glm::vec3) << "\n";
  glBufferData(GL_ARRAY_BUFFER, nbVertex * sizeof(glm::vec3), tNormales, GL_STATIC_DRAW);
  glVertexAttribPointer(indexNormale, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  if (glIsBuffer(VBO_Colors) == GL_TRUE)
    glDeleteBuffers(1, &VBO_Colors);
  glGenBuffers(1, &VBO_Colors);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_Colors);
  glBufferData(GL_ARRAY_BUFFER, nbVertex * sizeof(glm::vec3), tColors, GL_STATIC_DRAW);
  glVertexAttribPointer(indexColors, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  if (glIsBuffer(VBO_indices) == GL_TRUE)
    glDeleteBuffers(1, &VBO_indices);
  glGenBuffers(1, &VBO_indices); // ATTENTIOn IBO doit etre un GL_ELEMENT_ARRAY_BUFFER
  cout << "taille indices: " << nbTriangle * sizeof(glm::uvec3) << "\n";
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbTriangle * sizeof(glm::uvec3), tIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(indexVertex);
  glEnableVertexAttribArray(indexNormale);
  glEnableVertexAttribArray(indexColors);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glGenVertexArrays(1, &VAO_Eau);
  glBindVertexArray(VAO_Eau);

  // GENERATION DEAU
  if (glIsBuffer(VBO_sommets_eau) == GL_TRUE)
    glDeleteBuffers(1, &VBO_sommets_eau);
  glGenBuffers(1, &VBO_sommets_eau);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_sommets_eau);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), tEauSommets, GL_STATIC_DRAW);
  glVertexAttribPointer(indexVertexEau, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

  if (glIsBuffer(VBO_normales_eau) == GL_TRUE)
    glDeleteBuffers(1, &VBO_normales_eau);
  glGenBuffers(1, &VBO_normales_eau);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_normales_eau);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), tEauNormales, GL_STATIC_DRAW);
  glVertexAttribPointer(indexNormaleEau, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  
  if (glIsBuffer(VBO_indices_eau) == GL_TRUE)
    glDeleteBuffers(1, &VBO_indices_eau);
  glGenBuffers(1, &VBO_indices_eau); // ATTENTIOn IBO doit etre un GL_ELEMENT_ARRAY_BUFFER
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indices_eau);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(glm::uvec3), tEauIndices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(indexVertexEau);
  glEnableVertexAttribArray(indexNormaleEau);
  // une fois la config terminée
  // on désactive le dernier VBO et le VAO pour qu'ils ne soit pas accidentellement modifié
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void deleteVBO()
{
  glDeleteBuffers(1, &VBO_sommets);
  glDeleteBuffers(1, &VBO_normales);
  glDeleteBuffers(1, &VBO_indices);
  glDeleteBuffers(1, &VBO_Colors);
  glDeleteBuffers(1, &VAO_Montagne);
  glDeleteBuffers(1, &VAO_Eau);
}

/* fonction d'affichage */
void affichage()
{

  /* effacement de l'image avec la couleur de fond */
  /* Initialisation d'OpenGL */
  glClearColor(119 / 255., 181 / 255., 254 / 255., 0.0);
  glClearDepth(10.0f); // 0 is near, >0 is far
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0);
  glPointSize(2.0);

  View = glm::lookAt(cameraPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  Model = glm::mat4(1.0f);
  Model = glm::translate(Model, glm::vec3(0, 0, cameraDistance));
  Model = glm::rotate(Model, glm::radians(cameraAngleX), glm::vec3(1, 0, 0));
  Model = glm::rotate(Model, glm::radians(cameraAngleY), glm::vec3(0, 1, 0));
  // Model = glm::scale(Model,glm::vec3(.8, .8, .8));
  MVP = Projection * View * Model;
  traceMontagne();
  traceEau();
  /* on force l'affichage du resultat */
  glutPostRedisplay();
  glutSwapBuffers();
}

//-------------------------------------
// Trace le tore 2 via le VAO
void traceMontagne()
{
  // Use  shader & MVP matrix   MVP = Projection * View * Model;
  glUseProgram(montagneProg);

  // on envoie les données necessaires aux shaders */
  glUniformMatrix4fv(MatrixIDMVP, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDView, 1, GL_FALSE, &View[0][0]);
  glUniformMatrix4fv(MatrixIDModel, 1, GL_FALSE, &Model[0][0]);

  glUniform3f(locCameraPosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);

  glUniform1f(LightInfoGPU.locLightAmbientCoefficient, LightInfoCPU.ambientCoeff);
  glUniform3f(LightInfoGPU.locLightPosition, LightInfoCPU.position.x, LightInfoCPU.position.y, LightInfoCPU.position.z);
  glUniform3f(LightInfoGPU.locLightIntensities, LightInfoCPU.intensity.x, LightInfoCPU.intensity.y, LightInfoCPU.intensity.z);
  glUniform1f(LightInfoGPU.locLightAttenuation, LightInfoCPU.attenuation);

  // pour l'affichage
  glBindVertexArray(VAO_Montagne);                                  // on active le VAO
  glDrawElements(GL_TRIANGLES, nbTriangle * 3, GL_UNSIGNED_INT, 0); // on appelle la fonction dessin
  glBindVertexArray(0);                                             // on desactive les VAO
  glUseProgram(0);                                                  // et le pg
}

void traceEau()
{
  // Use  shader & MVP matrix   MVP = Projection * View * Model;
  glUseProgram(eauProg);

  // on envoie les données necessaires aux shaders */
  glUniformMatrix4fv(MatrixIDMVP_eau, 1, GL_FALSE, &MVP[0][0]);
  glUniformMatrix4fv(MatrixIDModel_eau, 1, GL_FALSE, &Model[0][0]);
  glUniform1f(Hauteur_eau_uniform, Hauteur_eau);

  // pour l'affichage
  glBindVertexArray(VAO_Eau);                              // on active le VAO_eau
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, 0); // on appelle la fonction dessin
  glBindVertexArray(0);                                    // on desactive les VAO_eau
  glUseProgram(0);                                         // et le pg
}

void reshape(int w, int h)
{
  // set viewport to be the entire window
  glViewport(0, 0, (GLsizei)w, (GLsizei)h); // ATTENTION GLsizei important - indique qu'il faut convertir en entier non négatif

  // set perspective viewing frustum
  float aspectRatio = (float)w / h;

  Projection = glm::perspective(glm::radians(60.0f), (float)(w) / (float)h, 1.0f, 1000.0f);
}

void clavier(unsigned char touche, int x, int y)
{
  switch (touche)
  {
  case 'f': /* affichage du carre plein */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutPostRedisplay();
    break;
  case 'e': /* affichage en mode fil de fer */
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glutPostRedisplay();
    break;
  case 'v': /* Affichage en mode sommets seuls */
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glutPostRedisplay();
    break;
  case 's': /* Affichage en mode sommets seuls */
    materialShininess -= .5;
    glutPostRedisplay();
    break;
  case 'S': /* Affichage en mode sommets seuls */
    materialShininess += .5;
    glutPostRedisplay();
    break;
  case 'x': /* Affichage en mode sommets seuls */
    LightInfoCPU.position.x -= .2;
    glutPostRedisplay();
    break;
  case 'X': /* Affichage en mode sommets seuls */
    LightInfoCPU.position.x += .2;
    glutPostRedisplay();
    break;
  case 'y': /* Affichage en mode sommets seuls */
    LightInfoCPU.position.y -= .2;
    glutPostRedisplay();
    break;
  case 'Y': /* Affichage en mode sommets seuls */
    LightInfoCPU.position.y += .2;
    glutPostRedisplay();
    break;
  case 'z': /* Affichage en mode sommets seuls */
    LightInfoCPU.position.z -= .2;
    glutPostRedisplay();
    break;
  case 'Z': /* Affichage en mode sommets seuls */
    LightInfoCPU.position.z += .2;
    glutPostRedisplay();
    break;
  case 'a': /* Affichage en mode sommets seuls */
    LightInfoCPU.ambientCoeff -= .1;
    cout << LightInfoCPU.ambientCoeff << "\n";
    glutPostRedisplay();
    break;
  case 'A': /* Affichage en mode sommets seuls */
    LightInfoCPU.ambientCoeff += .1;
    cout << LightInfoCPU.ambientCoeff << "\n";
    glutPostRedisplay();
    break;
  case 'h': /* Affichage en mode sommets seuls */
    nbTriangle++;
    glutPostRedisplay();
    break;
  case 'g': /* Affichage en mode sommets seuls */
    nbTriangle--;
    glutPostRedisplay();
    break;
  case 'j': /* Affichage en mode sommets seuls */
    clearBuffers();
    deleteVBO();
    initBuffers();
    initPoints(tSommets, tColors, N, w, h);
    initFaces(&tIndices, N);
    generateDiamondSquare(tSommets, N, maxHauteur);
    computeNormales(tNormales, tSommets, tIndices, N, nbTriangle);
    // afficheInfoData();
    genereVBO();

    glutPostRedisplay();
    break;
  case 'p': /* hauteur d'eau ++ */
    Hauteur_eau = Hauteur_eau + 0.1;
    glutPostRedisplay();
    break;
  case 'm': /* hauteur d'eau -- */
    Hauteur_eau = Hauteur_eau - 0.1;
    break;

  case 'q': /*la touche 'q' permet de quitter le programme */
    exit(0);
  }
}

void afficheInfoData()
{
  cout << "generation...\n\n";
  cout << "nbTriangle générée:" << nbTriangle << "\n";
  cout << "nbPoint:" << nbVertex << "\n";
  cout << "----------\n";
  for (size_t i = 0; i < nbTriangle; i++)
  {
    cout << tIndices[i].x << " " << tIndices[i].y << " " << tIndices[i].z << "\n";
  }
  for (size_t i = 0; i < nbVertex; i++)
  {
    cout << tSommets[i].x << " " << tSommets[i].y << " " << tSommets[i].z << "\n";
  }
}

void mouse(int button, int state, int x, int y)
{
  mouseX = x;
  mouseY = y;

  if (button == GLUT_LEFT_BUTTON)
  {
    if (state == GLUT_DOWN)
    {
      mouseLeftDown = true;
    }
    else if (state == GLUT_UP)
      mouseLeftDown = false;
  }

  else if (button == GLUT_RIGHT_BUTTON)
  {
    if (state == GLUT_DOWN)
    {
      mouseRightDown = true;
    }
    else if (state == GLUT_UP)
      mouseRightDown = false;
  }

  else if (button == GLUT_MIDDLE_BUTTON)
  {
    if (state == GLUT_DOWN)
    {
      mouseMiddleDown = true;
    }
    else if (state == GLUT_UP)
      mouseMiddleDown = false;
  }
}

void mouseMotion(int x, int y)
{
  if (mouseLeftDown)
  {
    cameraAngleY += (x - mouseX);
    cameraAngleX += (y - mouseY);
    mouseX = x;
    mouseY = y;
  }
  if (mouseRightDown)
  {
    cameraDistance += (y - mouseY) * 0.2f;
    mouseY = y;
  }

  glutPostRedisplay();
}