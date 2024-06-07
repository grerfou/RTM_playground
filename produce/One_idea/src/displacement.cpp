#include "displacement.h"

Image GenerateDisplacementMap(Image image) {
    Image displacement = GenImageColor(image.width, image.height, BLACK);

    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {
            Color pixel = GetImageColor(image, x, y);
            unsigned char brightness = (pixel.r + pixel.g + pixel.b) / 2.5;

      // 2.5
            unsigned char brightnessRight = (x < image.width - 1) ? 
                (GetImageColor(image, x + 1, y).r + GetImageColor(image, x + 1, y).g + GetImageColor(image, x + 1, y).b) / 3 : brightness;
            unsigned char brightnessDown = (y < image.height - 1) ? 
                (GetImageColor(image, x, y + 1).r + GetImageColor(image, x, y + 1).g + GetImageColor(image, x, y + 1).b) / 3 : brightness;

            int gradientX = brightnessRight - brightness;
            int gradientY = brightnessDown - brightness;

            unsigned char displacementX = (unsigned char)(gradientX + 128);
            unsigned char displacementY = (unsigned char)(gradientY + 128);

            ImageDrawPixel(&displacement, x, y, (Color){ displacementX, displacementY, 128, 255 });
        }
    }
    return displacement;
}
