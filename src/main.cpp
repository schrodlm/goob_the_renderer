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

  // TEST 1 - DIFFUSE SHADING
  TGAImage image(width, height, TGAImage::RGB);
  Goob goob(image);

  Model model("obj/african_head.obj");

  goob.renderDiffuseShading(model, Vec3f{0, 0, -1});
  goob.finalizeRendering("output.tga");

  // TEST 2 - LINES
  //  TGAImage image2(100, 100, TGAImage::RGB);
  //  Goob goob2(image2);
  //  goob2.line({13,20}, {80, 40}, white);
  //  goob2.line({20, 13}, {40, 80}, red);
  //  goob2.line({80, 40}, {13, 20}, red);
  //  goob2.finalizeRendering("test.tga");

  // TEST 3 - TRIANGLES
  // TGAImage image3(300, 300, TGAImage::RGB);
  // Goob goob3(image3);
  // Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
  // Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
  // Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};
  // goob3.triangle(t0[0], t0[1], t0[2], red);
  // goob3.triangle(t1[0], t1[1], t1[2], white);
  // goob3.triangle(t2[0], t2[1], t2[2], green);
  // goob3.finalizeRendering("test_triangles.tga");

  return 0;
}
