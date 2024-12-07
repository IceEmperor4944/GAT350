#pragma once
#include "Color.h"

struct material_t {
public:
	color3_t albedo{ 1 };
	color3_t specular{ 1 };
	float shininess = 1.0f;
};