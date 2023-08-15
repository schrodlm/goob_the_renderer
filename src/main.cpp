#include "tgaimage.h"
#include "model.h"
#include <algorithm>
#include <iostream>
#include <valarray>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

Model *model = NULL;
const int width = 800;
const int height = 800;

//================================================================
void line(Vec2i t0, Vec2i t1, TGAImage &image,
          const TGAColor &color)
{
  // bound check
  int width = image.get_width();
  int height = image.get_height();
  if (t0.x < 0 || t0.x > width || t1.x < 0 || t1.x > width ||
      t0.y < 0 || t0.y > height || t1.y < 0 || t1.y > height)
    throw std::out_of_range("Line out of bounds");

  unsigned int bigger_steps = std::max(std::abs(t1.x - t0.x), std::abs(t1.y - t0.y));
  Vec2f step = {((t1.x - t0.x) / (float)bigger_steps), ((t1.y - t0.y) / (float)bigger_steps)};

  Vec2f current_t = t0;

  for (unsigned int i = 0; i <= bigger_steps; i++)
  {
    image.set((int)(current_t.x + 0.5), (int)(current_t.y + 0.5), color);
    current_t.x += step.x;
    current_t.y += step.y;
  }
}

void triangle_fill(Vec2i t0,Vec2i t1, Vec2i t2, TGAImage &image, const TGAColor &color)
{


  unsigned int bigger_steps1 = std::max(std::abs(t1.x - t2.x), std::abs(t1.y - t2.y));
  unsigned int bigger_steps2 = std::max(std::abs(t0.x - t2.x), std::abs(t0.y - t2.y));

  unsigned int bigger_steps = std::max(bigger_steps1, bigger_steps2);

  float x_step1 = (t2.x - t0.x) / (float)bigger_steps;
  float y_step1 = (t2.y - t0.y) / (float)bigger_steps;

  float x_step2 = (t2.x - t1.x) / (float)bigger_steps;
  float y_step2 = (t2.y - t1.y) / (float)bigger_steps;

  Vec2f current_t0 = t0;
  Vec2f current_t1 = t1;

  for (unsigned int i = 0; i <= bigger_steps; i++)
  {

    line(current_t0, current_t1, image, red);

    current_t0.x += x_step1;
    current_t0.y += y_step1;

    current_t1.x += x_step2;
    current_t1.y += y_step2;
  }
  return;
}

int main(int argc, char **argv)
{

  TGAImage image(width, height, TGAImage::RGB);

  //============================== WIREFRAME ======================================

  /*
   Simple orthographic projection.

   The + 1. translates the coordinates from a [-1, 1] range to a [0, 2] range.
   Then, multiplying by width/2 or height/2 scales the coordinates to fit within
   the canvas dimensions. As a result, the vertex coordinates are transformed to fit
   within the canvas (the image's width and height).
  */
  model = new Model("obj/african_head.obj");

  for (int i = 0; i < model->nfaces(); i++)
  {
    std::vector<int> face = model->face(i);
    for (int j = 0; j < 3; j++)
    {
      Vec3f v0 = model->vert(face[j]);
      Vec3f v1 = model->vert(face[(j + 1) % 3]);
      
      int x0 = (v0.x + 1.) * width / 2.;
      int y0 = (v0.y + 1.) * height / 2.;
      Vec2i t0(x0,y0);
      int x1 = (v1.x + 1.) * width / 2.;
      int y1 = (v1.y + 1.) * height / 2.;
      Vec2i t1(x1,y1);

      line(t0, t1, image, white);

      std::cout << "{" << x0 << "," << x1 << "," << y0 << "," << y1 << "}" << std::endl;
    }
  }
  
  
  
  
  //============================== TRIANGLES ======================================
  
  
  
  // Vec2f t0 = {10,10};
  // Vec2f t1 = {50,50};
  // Vec2f t2 = {100,10};



  //triangle_fill(t0, t1, t2, image, white);

  image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
  image.write_tga_file("output.tga");
  delete model;
  return 0;
}
