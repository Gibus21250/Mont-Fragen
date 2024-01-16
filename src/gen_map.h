#ifndef GEN_MAP_H
#define GEN_MAP_H

#include "glm/glm.hpp"

void initPoints(glm::vec3 *points, glm::vec3 *colors, unsigned int N, float w, float h);
uint initFaces(glm::uvec3 **faces, unsigned int N);
void generateDiamondSquare(glm::vec3 *points, uint N, double heightMax);
double delta(double distance, double H);
double randomFrom(double a, double b);
void computeNormales(glm::vec3 *normales, glm::vec3 *points, glm::uvec3 *triangles, uint N, uint nbTriangle);

#endif // GEN_MAP_H