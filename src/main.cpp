#include "tgaimage.h"
#include "model.h"
#include "goob.h"
#include <algorithm>
#include <iostream>
#include <valarray>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

const int width = 800;
const int height = 800;

//================================================================
int main(int argc, char **argv)
{
  TGAImage image(width, height, TGAImage::RGB);
  Goob goob(image);

  Model model("obj/african_head.obj");

  goob.renderDiffuseShading(model, Vec3f{0,0,-1});
  goob.finalizeRendering("output.tga");
 
  return 0;
}
