#include "raylib.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

#define SAMPLE_RATE         44100
#define SAMPLE_SIZE         16        // bit depth: 8 bit, 16 bit or 32 bit
#define AUDIO_BUFFER_SIZE   4096      // sample count

AudioStream stream; // Audio stream

// Structure pour stocker les données de chaque point
struct PointData {
    float x;
    float y;
    float z;
};

std::vector<PointData> pointCloudData;

// Définition de la fonction de rappel pour mettre à jour le flux audio
void UpdateAudioStreamCallback(void* data, unsigned int samplesCount) {
    short* output = (short*)data;
    static float t = 0.0f; // Temps

    for (int i = 0; i < samplesCount; i++) {
        // Utilisation des données binaires pour générer le son
        // Par exemple, vous pouvez utiliser la position en x du point comme amplitude du son
        float amplitude = 0.5f * sin(2.0f * PI * pointCloudData[(int)t % pointCloudData.size()].x * t / SAMPLE_RATE);
        output[i * 2] = (short)(amplitude * SHRT_MAX); // Canal gauche
        output[i * 2 + 1] = (short)(amplitude * SHRT_MAX); // Canal droit
        // Mise à jour du temps
        t += 1.0f;
    }
}

int main() {
    // Lecture des données binaires à partir du fichier
    std::ifstream file("ressources/data.bin", std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file!" << std::endl;
        return -1;
    }

    PointData point;
    while (file.read(reinterpret_cast<char*>(&point), sizeof(PointData))) {
        pointCloudData.push_back(point);
    }
    file.close();

    // Initialisation de la fenêtre
    InitWindow(800, 600, "Point Cloud and Audio Generation");

    // Initialisation du dispositif audio
    InitAudioDevice();

    // Définition du nombre d'échantillons à stocker en une seconde
    stream = LoadAudioStream(SAMPLE_RATE, SAMPLE_SIZE, 2);

    // Définition de la fonction de rappel pour mettre à jour le flux audio
    SetAudioStreamCallback(stream, UpdateAudioStreamCallback);

    // Démarrage de la lecture du flux audio
    PlayAudioStream(stream);

    // Boucle principale
    while (!WindowShouldClose()) {
        // Dessin du nuage de points
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D({ 400, 300 });

        // Dessin des points du nuage
        for (const auto& point : pointCloudData) {
            DrawSphere({ point.x, point.y, point.z }, 1000.1f, RAYWHITE);
        }
        DrawSphere({ 0, 0, 0 }, 1000.1f, RAYWHITE);

        EndMode3D();

        DrawText("Press ESC to close window", 10, 10, 20, BLACK);

        EndDrawing();
    }

    // Libération des ressources
    UnloadAudioStream(stream);
    CloseAudioDevice(); // Fermeture du dispositif audio
    CloseWindow(); // Fermeture de la fenêtre et du contexte OpenGL

    return 0;
}

