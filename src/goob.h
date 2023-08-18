#ifndef __GOOB_H__
#define __GOOB_H__

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include <limits>

/**
 * @class Goob
 * @brief Responsible for rendering graphical primitives onto an image.
 *
 * The Goob class provides methods to draw basic graphics primitives, such as
 * lines and triangles, onto an image. It acts as a central rendering hub for the
 * Goob application and is designed to be extensible for future graphics functionalities.
 */
class Goob
{

private:
	TGAImage &image;
	std::vector<std::vector<float>> z_buffer;
public:
	Goob(TGAImage &_image) : image(_image){
		z_buffer = std::vector<std::vector<float>>(image.get_width(), std::vector<float>(image.get_height(), std::numeric_limits<float>::max()));
	};

	void triangle(Vec3i t0, Vec3i t1, Vec3i t2, TGAImage &image, const TGAColor &color);
	void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, const TGAColor &color);
	
	void line(Vec3i t0, Vec3i t1, TGAImage &image,const TGAColor &color);
	void line(Vec2i t0, Vec2i t1, TGAImage &image,const TGAColor &color);

	void renderFlatshade(Model& model);
	void renderWireframe(Model &model);
	void renderDiffuseShading(Model &model, Vec3f light);

	void finalizeRendering(const char* filename);
	void setPixelWithZBuffer(const Vec2i pixel_coords,int depth, const TGAColor& color);

private:
	void sortTriangleVerticesByY(Vec3i& t0, Vec3i& t1, Vec3i& t2);
	void addStep(Vec2f& current_step, const Vec2f toAdd);
};

#endif