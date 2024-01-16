#include <iostream>
#include <random>

#include "gen_map.h"

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void initFaces(glm::uvec3 *faces, unsigned int N)
{
    const int matSize = 2 * N + 1;
    int indiceFace = 0;
    // Pour chaque points des lignes
    for (int i = 0; i < matSize - 1; i++)
    {
        // Pour chaque colonne
        for (int j = 0; j < matSize - 1; j++)
        {
            int hg = i + matSize * j;           // Haut gauche
            int hd = i + 1 + matSize * j;       // Haut droite
            int bg = i + matSize * (j + 1);     // Bas gauche
            int bd = i + 1 + matSize * (j + 1); // Bas droite
            
            faces[indiceFace] = {
                hg, bg, hd};
            indiceFace++;
            faces[indiceFace] = {
                hd, bg, bd};
            indiceFace++;
        }
    }
}

void initPoints(glm::vec3 *points, unsigned int N, float w, float h)
{
    int matSize = 2 * N + 1;
    const double xStep = w / (matSize - 1);
    const double yStep = h / (matSize - 1);

    for (int i = 0; i < matSize; i++)
    {
        for (int j = 0; j < matSize; j++)
        {
            points[i + j * (2 * N + 1)] = {
                -w / 2 + i * xStep,
                0,
                h / 2 - j * yStep};
        }
    }
}

// diamondSquare algorith
void generateDiamondSquare(glm::vec3 *points, uint N, float heightMax, double H)
{

    int matSize = 2 * N + 1;
    srand(time(NULL));

    // 4 corners initialized
    points[0].y = (rand() / RAND_MAX) * heightMax;
    points[(2 * N + 1)].y = (rand() / RAND_MAX) * heightMax;
    points[2 * (2 * N + 1) * N].y = (rand() / RAND_MAX) * heightMax;
    points[2 * (2 * N + 1) * N + 2 * N].y = (rand() / RAND_MAX) * heightMax;

    int chunkSize = matSize - 1; // taille du chunk à traiter

    while (chunkSize > 1)
    {
        int half = chunkSize / 2;

        for (int x = half; x < matSize; x += chunkSize)
        {
            for (int y = half; y < matSize; y += chunkSize)
            {
                double avg = (points[x - half + (y - half) * (2 * N + 1)].y +
                              points[x + half + (y - half) * (2 * N + 1)].y +
                              points[x - half + (y + half) * (2 * N + 1)].y +
                              points[x + half + (y + half) * (2 * N + 1)].y)
                              /4;
                /*
                points[x - half][y - half].y +
                    points[x + half][y - half].y +
                    points[x - half][y + half].y +
                    points[x + half][y + half].y*/

                // TODO ajuster si necessaire
                double dist2 = glm::dot(points[x + y * (2 * N + 1)], points[x - half + (y - half) * (2 * N + 1)]) 
                + glm::dot(points[x + y * (2 * N + 1)], points[x + half + (y - half) * (2 * N + 1)]) 
                + glm::dot(points[x + y * (2 * N + 1)], points[x - half + (y + half) * (2 * N + 1)]) 
                + glm::dot(points[x + y * (2 * N + 1)], points[x + half + (y + half) * (2 * N + 1)]);

                points[x + y * (2 * N + 1)].y = avg + delta(dist2, H);
            }
        }

        int shift = 0;
        for (int x = 0; x < matSize; x += half)
        {
            if (shift == 0)
                shift = half;
            else
                shift = 0;

            for (int y = shift; y < matSize; y += chunkSize)
            {
                double sum = 0;
                int n = 0;
                double dist2 = 0;

                if (x >= half)
                {
                    sum += points[x - half + y * (2 * N + 1)].y;
                    n++;
                    dist2 = glm::dot(points[x + y * (2 * N + 1)], points[x - half + y * (2 * N + 1)]);
                }
                if (x + half < matSize)
                {
                    sum += points[x + half + y * (2 * N + 1)].y;
                    n++;
                    dist2 = glm::dot(points[x + y * (2 * N + 1)], points[x + half + y * (2 * N + 1)]);
                }
                if (y >= half)
                {
                    sum += points[x + (y - half) * (2 * N + 1)].y;
                    n++;
                    dist2 = glm::dot(points[x + y * (2 * N + 1)], points[x + (y - half) * (2 * N + 1)]);
                }
                if (y + half > matSize)
                {
                    sum += points[x + (y + half) * (2 * N + 1)].y;
                    n++;
                    dist2 = glm::dot(points[x + y * (2 * N + 1)], points[x + (y + half) * (2 * N + 1)]);
                }

                double avg = sum / n;
                points[x + y * (2 * N + 1)].y = avg + delta(dist2, H);
            }
        }
        chunkSize = half;
    }
}

double delta(double distance, double H)
{
    return (2 * (rand() / RAND_MAX) - 1) * distance * pow(2, -H);
}