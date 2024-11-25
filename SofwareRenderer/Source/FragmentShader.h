#pragma once
#include "Color.h"

struct fragment_input_t
{
	//
};

class FragmentShader
{
public:
	static color4_t Process(const fragment_input_t& fragment);
};