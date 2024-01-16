#include <iostream>
#include <random>

#include "gen_map.h"

uint initFaces(glm::uvec3 **faces, unsigned int N)
{
#define PAS 8
    *faces = (glm::uvec3 *)malloc(PAS * sizeof(glm::uvec3));

    int sizeBuff = PAS;

    const int matSize = pow(2, N) + 1;
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
    int matSize = pow(2, N) + 1;
    const double xStep = w / (matSize - 1);
    const double yStep = h / (matSize - 1);

    // Pour chaque lignes
    for (int i = 0; i < matSize; i++)
    {
        // Chaque colonnes
        for (int j = 0; j < matSize; j++)
        {
            points[j + i * matSize] = {
                -w / 2 + i * xStep,
                0,
                h / 2 - j * yStep};

            // Rouge: ligne
            // Bleu: colonne
            colors[j + i * matSize] = {
                (double)j / matSize,
                0,
                (double)i / matSize};
        }
    }
}

// diamondSquare algorith
void generateDiamondSquare(glm::vec3 *points, uint N, double heightMax)
{

    int matSize = pow(2, N) + 1;
    // donne le dernier index d'une ligne
    int twoPowN = pow(2, N);
    srand(time(NULL));

    // 4 corners initialized
    points[0].y = randomFrom(-1, 1) * heightMax;                     // coin haut gauche
    points[twoPowN].y = randomFrom(-1, 1) * heightMax;               // coin haut droit
    points[matSize * twoPowN].y = randomFrom(-1, 1) * heightMax;     // coin bas gauche
    points[matSize * matSize - 1].y = randomFrom(-1, 1) * heightMax; // coin bas droit

    int chunkSize = matSize - 1; // taille du chunk à traiter
    double tH = -1;
    while (chunkSize > 1)
    {
        int half = chunkSize / 2;

        for (int x = half; x < matSize; x += chunkSize)
        {
            for (int y = half; y < matSize; y += chunkSize)
            {
                double avg = (points[x - half + (y - half) * matSize].y +
                              points[x + half + (y - half) * matSize].y +
                              points[x - half + (y + half) * matSize].y +
                              points[x + half + (y + half) * matSize].y) /
                             4;

                // TODO ajuster car NECESSAIRE
                double dist = sqrt(glm::dot(points[x + y * matSize], points[x - half + (y - half) * matSize])) + sqrt(glm::dot(points[x + y * matSize], points[x + half + (y - half) * matSize])) + sqrt(glm::dot(points[x + y * matSize], points[x - half + (y + half) * matSize])) + sqrt(glm::dot(points[x + y * matSize], points[x + half + (y + half) * matSize]));

                dist /= 4;
                points[x + y * matSize].y = avg + delta(dist, tH);
                // points[x + y * matSize].y = avg + randomFrom(-1, 1);
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
                    sum += points[x - half + y * matSize].y;
                    n++;
                    dist = sqrt(glm::dot(points[x + y * matSize], points[x - half + y * matSize]));
                }
                if (x + half < matSize)
                {
                    sum += points[x + half + y * matSize].y;
                    n++;
                    dist = sqrt(glm::dot(points[x + y * matSize], points[x + half + y * matSize]));
                }
                if (y >= half)
                {
                    sum += points[x + (y - half) * matSize].y;
                    n++;
                    dist = sqrt(glm::dot(points[x + y * matSize], points[x + (y - half) * matSize]));
                }
                if (y + half < matSize)
                {
                    sum += points[x + (y + half) * matSize].y;
                    n++;
                    dist = sqrt(glm::dot(points[x + y * matSize], points[x + (y + half) * matSize]));
                }

                double avg = sum / n;
                dist /= 4;
                points[x + y * matSize].y = avg + delta(avg, tH);
                // points[x + y * matSize].y = avg + randomFrom(-1, 1);
            }
        }
        tH++;
        chunkSize = half;
    }
}

double randomFrom(double a, double b)
{
    double t = (((double)rand()) / RAND_MAX);
    return (1 - t) * a + t * b;
}

double delta(double facteur, double H)
{
    return randomFrom(-1, 1) * facteur * pow(2, -H);
}

void computeNormales(glm::vec3 *normales, uint N)
{
}