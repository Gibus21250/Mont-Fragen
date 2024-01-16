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
    double f = 0;
    double at = randomFrom(0.05, 0.15);

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

        f = randomFrom(0.8, 1.2);
        tH = tH + f + at;
        at = at - 0.04;
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

void computeNormales(glm::vec3 *normales, glm::vec3 *points, glm::uvec3 *triangles, uint N, uint nbTriangle)
{
    uint longMat = (uint) pow(2, N) + 1;
    uint nbTotalPoints = longMat * longMat;

    //Initialisation d'une structure accumulant les normales de chaque points
    struct NormalesInfo {
        glm::vec3 accumulatedNormale;
        uint nb;
    };

    NormalesInfo* infos = new NormalesInfo[nbTotalPoints];

    //Pour chaque triangle, on accumule la normal de la face aux points de ce triangle
    for (size_t i = 0; i < nbTriangle; i++)
    {
        //Calcule de la normale de la face
        glm::vec3 ab = points[triangles[i].y] - points[triangles[i].x];
        glm::vec3 ac = points[triangles[i].z] - points[triangles[i].x];

        glm::vec3 normaleFace = glm::cross(ab, ac);

        normaleFace = glm::normalize(normaleFace);

        infos[triangles[i].x].accumulatedNormale += normaleFace;
        infos[triangles[i].x].nb++;

        infos[triangles[i].y].accumulatedNormale += normaleFace;
        infos[triangles[i].y].nb++;

        infos[triangles[i].z].accumulatedNormale += normaleFace;
        infos[triangles[i].z].nb++;
    }

    for (size_t i = 0; i < nbTotalPoints; i++)
    {
        normales[i] = infos[i].accumulatedNormale / (float) infos[i].nb;
    }
}