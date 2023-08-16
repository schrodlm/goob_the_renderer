#ifndef __GOOB_H__
#define __GEOMETRY_H__

#include "tgaimage.h"


/**
 * @class Goob
 * @brief Responsible for rendering graphical primitives onto an image.
 * 
 * The Goob class provides methods to draw basic graphics primitives, such as
 * lines and triangles, onto an image. It acts as a central rendering hub for the
 * Goob application and is designed to be extensible for future graphics functionalities.
 */
class Goob{

private:
	TGAImage &image;

public:
	Goob(TGAImage &_image) : image(_image){};

};


#endif