#ifndef GEN_MAP_H
#define GEN_MAP_H

#include "glm/glm.hpp"

double distance(double x1, double y1, double x2, double y2);

void initPoints(glm::vec3 *points, unsigned int N, float w, float h);
void initFaces(glm::uvec3 *faces, unsigned int N);
void generateDiamondSquare(glm::vec3 *points, uint N, float heightMax, double H);
double delta(double distance, double H);

#endif  // GEN_MAP_H