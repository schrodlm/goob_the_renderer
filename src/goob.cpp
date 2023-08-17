// TODO: Barymetric version of triangle
// TODO: DrawVerticalLine

#include "goob.h"

void Goob::line(Vec2i t0, Vec2i t1, TGAImage &image, const TGAColor &color)
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

void Goob::triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, const TGAColor &color)
{

	// Sort vertices by y-coordinate
	if (t0.y > t1.y)
		std::swap(t0, t1);
	if (t0.y > t2.y)
		std::swap(t0, t2);
	if (t1.y > t2.y)
		std::swap(t1, t2);

	// Compute the vertical span of the triangle
	int deltaY = t2.y - t0.y;

	if (deltaY == 0)
		return; // triangle is a degenerate point, nothing to fill.

	float x_step1 = (float)(t2.x - t0.x) / deltaY;
	float y_step1 = 1;

	float x_step2;
	float y_step2;

	// Check if t1.y and t0.y are different to prevent division by zero
	if (t1.y != t0.y)
	{
		x_step2 = (float)(t1.x - t0.x) / (t1.y - t0.y);
		y_step2 = 1; //(float)(t1.y - t0.y) / (t1.y - t0.y);
	}
	else
	{
		x_step2 = 0;
		y_step2 = 1;
	}

	Vec2f current_t0 = t0;
	Vec2f current_t1 = t0;

	for (int i = 0; i <= deltaY; i++)
	{
		if (i == (t1.y - t0.y))
		{ // Switch to the bottom half of the triangle
			x_step2 = (t2.y - t1.y) != 0 ? (float)(t2.x - t1.x) / (t2.y - t1.y) : 0;
			y_step2 = 1.0;
			current_t1 = t1;
		}

		line((Vec2i)current_t0, (Vec2i)current_t1, image, color);

		current_t0.x += x_step1;
		current_t0.y += y_step1;

		current_t1.x += x_step2;
		current_t1.y += y_step2;
	}
	return;
}

//============================= TRIANGLES ===================================

void Goob::renderFlatshade(Model &model)
{
	for (int i = 0; i < model.nfaces(); i++)
	{
		std::vector<int> face = model.face(i);
		Vec2i screen_coords[3];
		for (int j = 0; j < 3; j++)
		{
			Vec3f world_coords = model.vert(face[j]);
			screen_coords[j] = Vec2i((world_coords.x + 1.) * image.get_width() / 2., (world_coords.y + 1.) * image.get_height() / 2.);
		}
		triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}
}

void Goob::renderDiffuseShading(Model &model, Vec3f light_dir)
{
	for(int i = 0; i < model.nfaces(); i++)
	{
		std::vector<int> face = model.face(i);
		Vec3f world_coords[3];
		Vec2f screen_coords[3];

		for(int j = 0; j < 3; j++){
			world_coords[j] = model.vert(face[j]);
			screen_coords[j] = Vec2i((world_coords[j].x + 1.) * image.get_width() / 2., (world_coords[j].y + 1.) * image.get_height() / 2.);
		}

		Vec3f normal = compute_normal(world_coords[0], world_coords[1], world_coords[2]);
		normal.normalize();
		float light_intensity = normal * light_dir;
		if(light_intensity < 0) continue;
		triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(255*light_intensity, 255*light_intensity, 255*light_intensity, 255));

	}
}

//============================= WIREFRAME===================================

/*
 Simple orthographic projection.

 The + 1. translates the coordinates from a [-1, 1] range to a [0, 2] range.
 Then, multiplying by width/2 or height/2 scales the coordinates to fit within
 the canvas dimensions. As a result, the vertex coordinates are transformed to fit
 within the canvas (the image's width and height).
*/
void Goob::renderWireframe(Model &model)
{
	for (int i = 0; i < model.nfaces(); i++)
	{
		std::vector<int> face = model.face(i);
		for (int j = 0; j < 3; j++)
		{
			Vec3f v0 = model.vert(face[j]);
			Vec3f v1 = model.vert(face[(j + 1) % 3]);

			int x0 = (v0.x + 1.) * image.get_width() / 2.;
			int y0 = (v0.y + 1.) * image.get_height() / 2.;
			Vec2i t0(x0, y0);
			int x1 = (v1.x + 1.) * image.get_width() / 2.;
			int y1 = (v1.y + 1.) * image.get_height() / 2.;
			Vec2i t1(x1, y1);

			line(t0, t1, image, TGAColor(255, 255, 255, 255));
		}
	}
}

void Goob::finalizeRendering(const char *filename)
{
	image.flip_vertically();
	image.write_tga_file(filename);
}