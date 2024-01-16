#ifndef GEN_MAP_H
#define GEN_MAP_H

#include "glm/glm.hpp"

double distance(double x1, double y1, double x2, double y2);

void initPoints(glm::vec3 *points, glm::vec3 *colors, unsigned int N, float w, float h);
uint initFaces(glm::uvec3 **faces, unsigned int N);
void generateDiamondSquare(glm::vec3 *points, uint N, float heightMax, double H);
double delta(double distance, double H);
void computeNormales(glm::vec3 *normales, uint N);

#endif // GEN_MAP_H