#include <iostream>
#include <random>

#include "gen_map.h"

double distance(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

uint initFaces(glm::uvec3 **faces, unsigned int N)
{
#define PAS 8
    *faces = (glm::uvec3 *)malloc(PAS * sizeof(glm::uvec3));

    int sizeBuff = PAS;

    const int matSize = 2 * N + 1;
    uint indiceFace = 0;

    // Pour chaque ligne
    for (int i = 0; i < matSize - 1; i++)
    {
        // Pour chaque colonne
        for (int j = 0; j < matSize - 1; j++)
        {
            if ((indiceFace + 2) > sizeBuff)
            {
                *faces = (glm::uvec3 *)realloc(*faces, (sizeBuff + PAS) * sizeof(glm::uvec3));
                sizeBuff += PAS;
            }

            int hg = j + matSize * i;           // Haut gauche
            int hd = j + 1 + matSize * i;       // Haut droite
            int bg = j + matSize * (i + 1);     // Bas gauche
            int bd = j + 1 + matSize * (i + 1); // Bas droite

            (*faces)[indiceFace] = {
                hg, bg, hd};
            indiceFace++;
            (*faces)[indiceFace] = {
                hd, bg, bd};
            indiceFace++;
        }
    }

    return indiceFace;
}

void initPoints(glm::vec3 *points, glm::vec3 *colors, unsigned int N, float w, float h)
{
    int matSize = 2 * N + 1;
    const double xStep = w / (matSize - 1);
    const double yStep = h / (matSize - 1);

    // Pour chaque lignes
    for (int i = 0; i < matSize; i++)
    {
        // Chaque colonnes
        for (int j = 0; j < matSize; j++)
        {
            points[j + i * (2 * N + 1)] = {
                -w / 2 + i * xStep,
                0,
                h / 2 - j * yStep};

            // Rouge: ligne
            // Bleu: colonne
            colors[j + i * (2 * N + 1)] = {
                (double)j / matSize,
                0,
                (double)i / matSize};
        }
    }
}

// diamondSquare algorith
void generateDiamondSquare(glm::vec3 *points, uint N, float heightMax, double H)
{

    int matSize = 2 * N + 1;
    srand(time(NULL));

    // 4 corners initialized
    points[0].y = (rand() / (RAND_MAX + 1.0)) * heightMax;                             // coin haut gauche
    points[(2 * N)].y = (rand() / (RAND_MAX + 1.0)) * heightMax;                       // coin haut droit
    points[(2 * N + 1) * (2 * N)].y = (rand() / (RAND_MAX + 1.0)) * heightMax;         // coin bas gauche
    points[(2 * N + 1) * (2 * N + 1) - 1].y = (rand() / (RAND_MAX + 1.0)) * heightMax; // coin bas droit

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
                              points[x + half + (y + half) * (2 * N + 1)].y) /
                             4;

                // TODO ajuster car NECESSAIRE
                double dist = sqrt(glm::dot(points[x + y * (2 * N + 1)], points[x - half + (y - half) * (2 * N + 1)])) + sqrt(glm::dot(points[x + y * (2 * N + 1)], points[x + half + (y - half) * (2 * N + 1)])) + sqrt(glm::dot(points[x + y * (2 * N + 1)], points[x - half + (y + half) * (2 * N + 1)])) + sqrt(glm::dot(points[x + y * (2 * N + 1)], points[x + half + (y + half) * (2 * N + 1)]));

                dist = dist / 4;
                points[x + y * (2 * N + 1)].y = avg + delta(dist, H);
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
                double dist = 0;

                if (x >= half)
                {
                    sum += points[x - half + y * (2 * N + 1)].y;
                    n++;
                    dist = sqrt(glm::dot(points[x + y * (2 * N + 1)], points[x - half + y * (2 * N + 1)]));
                }
                if (x + half < matSize)
                {
                    sum += points[x + half + y * (2 * N + 1)].y;
                    n++;
                    dist = sqrt(glm::dot(points[x + y * (2 * N + 1)], points[x + half + y * (2 * N + 1)]));
                }
                if (y >= half)
                {
                    sum += points[x + (y - half) * (2 * N + 1)].y;
                    n++;
                    dist = sqrt(glm::dot(points[x + y * (2 * N + 1)], points[x + (y - half) * (2 * N + 1)]));
                }
                if (y + half > matSize)
                {
                    sum += points[x + (y + half) * (2 * N + 1)].y;
                    n++;
                    dist = sqrt(glm::dot(points[x + y * (2 * N + 1)], points[x + (y + half) * (2 * N + 1)]));
                }

                double avg = sum / n;
                points[x + y * (2 * N + 1)].y = avg + delta(dist, H);
            }
        }
        chunkSize = half;
    }
}

double delta(double distance, double H)
{
    return (2 * (rand() / RAND_MAX + 1.0) - 1) * distance * pow(2, -H);
}

void computeNormales(glm::vec3 *normales, uint N)
{
}