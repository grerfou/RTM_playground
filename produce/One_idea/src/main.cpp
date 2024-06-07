#include "raylib.h"
#include "displacement.h"
#include "binary_conversion.h"

int main() 
{
  InitWindow(800, 600, "Displacement Map Generation");

  Image image = LoadImage("./data/input.png");
  Image displacement = GenerateDisplacementMap(image);

  ExportImage(displacement, "./data/displacement.png");

  ConvertImageToBinary("./data/displacement.png", "./data/displacement.bin");

  UnloadImage(image);
  UnloadImage(displacement);

  CloseWindow();

  return 0;
}


