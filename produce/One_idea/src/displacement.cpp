#include "displacement.h"
#include <iostream>

Image GenerateDisplacementMap(Image image, int newWidth, int newHeight) {
    Image displacement = GenImageColor(newWidth, newHeight, BLACK);

    float scaleX = (float)newWidth / image.width;
    float scaleY = (float)newHeight / image.height;

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int originalX = (int)(x / scaleX);
            int originalY = (int)(y / scaleY);

            Color pixel = GetImageColor(image, originalX, originalY);
            unsigned char brightness = (pixel.r + pixel.g + pixel.b) / 2;

            unsigned char brightnessRight = (originalX < image.width - 1) ?
                (GetImageColor(image, originalX + 1, originalY).r + GetImageColor(image, originalX + 1, originalY).g + GetImageColor(image, originalX + 1, originalY).b) / 3 : brightness;
            unsigned char brightnessDown = (originalY < image.height - 1) ?
                (GetImageColor(image, originalX, originalY + 1).r + GetImageColor(image, originalX, originalY + 1).g + GetImageColor(image, originalX, originalY + 1).b) / 3 : brightness;

            int gradientX = brightnessRight - brightness;
            int gradientY = brightnessDown - brightness;

            unsigned char displacementX = (unsigned char)(gradientX + 128);
            unsigned char displacementY = (unsigned char)(gradientY + 128);

            ImageDrawPixel(&displacement, x, y, (Color){ displacementX, displacementY, 128, 255 });
        }
    }
    return displacement;
}

