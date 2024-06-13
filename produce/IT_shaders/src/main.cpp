/*
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
*/


#include "raylib.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <filesystem>
#include <mutex>
#include <iostream>

namespace fs = std::filesystem;

std::atomic<bool> running(true);
std::mutex textureMutex;

void WatchDirectory(const std::string& directory, Texture2D &texture, std::string &currentFile) {
    while (running) {
        try {
            if (fs::is_directory(directory)) {
                for (const auto &entry : fs::directory_iterator(directory)) {
                    if (entry.is_regular_file() && entry.path().extension() == ".png") { // Suppose you are looking for .png files
                        std::string newFile = entry.path().string();
                        if (newFile != currentFile) {
                            currentFile = newFile;

                            // Lock the mutex before modifying the texture
                            std::lock_guard<std::mutex> lock(textureMutex);

                            if (texture.id != 0) {
                                UnloadTexture(texture);
                            }

                            Image image = LoadImage(newFile.c_str());
                            if (image.data != nullptr) {
                                texture = LoadTextureFromImage(image);
                                UnloadImage(image);

                                if (texture.id == 0) {
                                    std::cerr << "Failed to load texture from " << newFile << std::endl;
                                }
                            } else {
                                std::cerr << "Failed to load image from " << newFile << std::endl;
                            }
                        }
                    }
                }
            } else {
                std::cerr << "Error: " << directory << " is not a directory." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in watcher thread: " << e.what() << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    std::string directory = "resourcesresources/texturess"; // The directory to watch
    std::string currentFile = "";

    Texture2D texture = {0}; // Initialize the texture

    if (fs::exists(directory) && fs::is_directory(directory)) {
        for (const auto &entry : fs::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".png") {
                currentFile = entry.path().string();
                Image image = LoadImage(currentFile.c_str());
                if (image.data != nullptr) {
                    texture = LoadTextureFromImage(image);
                    UnloadImage(image);
                } else {
                    std::cerr << "Failed to load initial image from " << currentFile << std::endl;
                }
                break;
            }
        }
    } else {
        std::cerr << "Error: " << directory << " does not exist or is not a directory." << std::endl;
        return 1;
    }

    // Launch a thread to watch the directory
    std::thread watcher(WatchDirectory, directory, std::ref(texture), std::ref(currentFile));

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        {
            // Lock the mutex before accessing the texture
            std::lock_guard<std::mutex> lock(textureMutex);
            if (texture.id != 0) {
                DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2, WHITE);
            } else {
                DrawText("No image found", 10, 10, 20, RED);
            }
        }

        EndDrawing();
    }

    // Cleanup
    running = false;
    watcher.join(); // Wait for the watcher thread to finish
    if (texture.id != 0) {
        UnloadTexture(texture);
    }

    CloseWindow();

    return 0;
}

