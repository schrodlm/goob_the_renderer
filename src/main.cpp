#include "tgaimage.h"
#include <algorithm>
#include <iostream>
#include <valarray>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void line(int x0, int y0, int x1, int y1, TGAImage &image,
          const TGAColor &color) {
  unsigned int bigger_steps = std::max(std::abs(x1 - x0), std::abs(y1 - y0));
  float x_step = (x1 - x0) / (float)bigger_steps;
  float y_step = (y1 - y0) / (float)bigger_steps;

  // std::cout << "x_step: " << x_step << "\ny_step: " << y_step << std::endl;

  float current_x = x0;
  float current_y = y0;

  for (unsigned int i = 0; i <= bigger_steps; i++) {
    image.set((int)(current_x + 0.5), (int)(current_y + 0.5), color);
    current_x += x_step;
    current_y += y_step;
  }
}

int main(int argc, char **argv) {
  // image.flip_vertically(); // i want to have the origin at the left bottom
  //                          // corner of the image

  TGAImage image(100, 100, TGAImage::RGB);
  line(13, 20, 80, 40, image, white);
  line(20, 13, 40, 80, image, red);
  line(80, 40, 13, 20, image, red);

  image.flip_vertically();

  image.write_tga_file("output.tga");

  // Create a line alghorithm

  return 0;
}
