// TODO: Barymetric version of triangle
// TODO: DrawVerticalLine

#include "goob.h"

//================================= LINE ===============================================

void Goob::line(Vec2i t0, Vec2i t1, const TGAColor &color)
{
	line((Vec2iZ)t0, (Vec2iZ)t1, color);
}

void Goob::line(Vec2iZ t0, Vec2iZ t1, const TGAColor &color)
{
	// bound check
	int width = image.get_width();
	int height = image.get_height();
	if (t0.x < 0 || t0.x > width || t1.x < 0 || t1.x > width ||
		t0.y < 0 || t0.y > height || t1.y < 0 || t1.y > height)
		throw std::out_of_range("Line out of bounds");

	unsigned int bigger_steps = std::max(std::abs(t1.x - t0.x), std::abs(t1.y - t0.y));
	Vec2f step = {((t1.x - t0.x) / (float)bigger_steps), ((t1.y - t0.y) / (float)bigger_steps)};

	float depthStep = (t1.z - t0.z) / static_cast<float>(bigger_steps);

	Vec2f current_t = (Vec2f)t0;
	float currentDepth = t0.z;

	for (unsigned int i = 0; i <= bigger_steps; i++)
	{
		int x = static_cast<int>(current_t.x + 0.5);
		int y = static_cast<int>(current_t.y + 0.5);
		
		if (currentDepth > z_buffer[y][x])
		{
			z_buffer[y][x] = currentDepth;
			image.set(x, y, color);
		}

		currentDepth += depthStep;
		addStep(current_t, step);
	}
}

//=================================TRIANGLE===============================================
void Goob::triangle(Vec2i t0, Vec2i t1, Vec2i t2, const TGAColor &color)
{
	triangle((Vec2iZ)t0, (Vec2iZ)t1, (Vec2iZ)t2, color);
}

void Goob::triangle(Vec2iZ t0, Vec2iZ t1, Vec2iZ t2, const TGAColor &color)
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

	Vec2f step1((float)(t2.x - t0.x) / deltaY, 1);
	Vec2f step2;

	// Check if t1.y and t0.y are different to prevent division by zero
	if (t1.y != t0.y)
	{
		step2.x = (float)(t1.x - t0.x) / (t1.y - t0.y);
		step2.y = 1; //(float)(t1.y - t0.y) / (t1.y - t0.y);
	}
	else
	{
		step2.x = 0;
		step2.y = 1;
	}

	Vec2fZ current_t0 = static_cast<Vec2fZ>(t0);
	Vec2fZ current_t1 = static_cast<Vec2fZ>(t0);

	for (int i = 0; i <= deltaY; i++)
	{
		if (i == (int)(t1.y - t0.y))
		{
			// Switch to the bottom half of the triangle
			step2.x = (t2.y - t1.y) != 0 ? (float)(t2.x - t1.x) / (t2.y - t1.y) : 0;
			step2.y = 1.0;
			current_t1 = static_cast<Vec2fZ>(t1);
		}

		line(current_t0, current_t1, color);

		addStep(current_t0, step1);
		addStep(current_t1, step2);
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
		triangle(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
	}
}

void Goob::renderDiffuseShading(Model &model, Vec3f light_dir)
{
	for (int i = 0; i < model.nfaces(); i++)
	{
		std::vector<int> face = model.face(i);
		Vec3f world_coords[3];
		Vec2fZ screen_coords[3];

		for (int j = 0; j < 3; j++)
		{
			world_coords[j] = model.vert(face[j]);
			screen_coords[j] = Vec2fZ((world_coords[j].x + 1.) * image.get_width() / 2., (world_coords[j].y + 1.) * image.get_height() / 2., world_coords[j].z );
		}

		Vec3f normal = compute_normal(world_coords[0], world_coords[1], world_coords[2]);
		normal.normalize();
		float light_intensity = normal * light_dir;
		if (light_intensity < 0)
			continue;
		triangle(screen_coords[0], screen_coords[1], screen_coords[2], TGAColor(255 * light_intensity, 255 * light_intensity, 255 * light_intensity, 255));
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

			line(t0, t1, TGAColor(255, 255, 255, 255));
		}
	}
}

void Goob::finalizeRendering(const char *filename)
{
	image.flip_vertically();
	image.write_tga_file(filename);
}

void Goob::setPixelWithZBuffer(const Vec2i coords, int depth, const TGAColor &color)
{
	if (z_buffer[coords.y][coords.x] < depth)
	{
		z_buffer[coords.y][coords.x] = depth;
		image.set(coords.x, coords.y, color);
	}
}

// UTILITIES
void Goob::sortTriangleVerticesByY(Vec3i &t0, Vec3i &t1, Vec3i &t2)
{
	// Sort vertices by y-coordinate
	if (t0.y > t1.y)
		std::swap(t0, t1);
	if (t0.y > t2.y)
		std::swap(t0, t2);
	if (t1.y > t2.y)
		std::swap(t1, t2);
}

void Goob::addStep(Vec2f &current_step, const Vec2f toAdd)
{
	current_step.x += toAdd.x;
	current_step.y += toAdd.y;
}

void Goob::addStep(Vec2fZ &current_step, const Vec2f toAdd)
{
	current_step.x += toAdd.x;
	current_step.y += toAdd.y;
}