#include "Framebuffer.h"
#include "Image.h"
#include "MathUtils.h"
#include "Renderer.h"
#include "Color.h"
#include <iostream>

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height) {
	m_width = width;
	m_height = height;
	m_pitch = width * sizeof(color_t);

	m_texture = SDL_CreateTexture(renderer.m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!m_texture) {
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
	}

	m_buffer.resize(m_width * m_height);
}

Framebuffer::~Framebuffer() {
	SDL_DestroyTexture(m_texture);
}

void Framebuffer::Update() {
	SDL_UpdateTexture(m_texture, NULL, m_buffer.data(), m_pitch);
}

void Framebuffer::Clear(const color_t& color) {
	std::fill(m_buffer.begin(), m_buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color) {
	color_t& dest = m_buffer[x + y * m_width];
	dest = ColorBlend(color, dest);
}

void Framebuffer::DrawPointClip(int x, int y, const color_t& color) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) return;

	color_t& dest = m_buffer[x + y * m_width];
	dest = ColorBlend(color, dest);
}

void Framebuffer::DrawRect(int x, int y, int w, int h, const color_t& color){
	if (x + w < 0 || x + w >= m_width || y + h < 0 || y + h >= m_height) return;

	int x1 = std::max(x, 0);
	int x2 = std::min(x + w, m_width);
	int y1 = std::max(y, 0);
	int y2 = std::min(y + h, m_height);

	for (int i = y1; i < y2; i++) {
		int index = x1 + i * m_width;
		std::fill(m_buffer.begin() + index, m_buffer.begin() + (index + x2 - x1), color);
	}
}

void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color) {
	ClipLine(x1, x2, y1, y2);
	
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx == 0) {
		if (y1 > y2)std::swap(y1, y2);
		for (int y = y1; y > y2; y++) {
			m_buffer[x1 + y * m_width] = color;
		}
	}
	else {
		float m = dy / (float)dx;
		float b = y1 - (m * x1);

		if (std::abs(dx) > std::abs(dy)) {
			for (int x = x1; x <= x2; x++) {
				int y = (int)round((m * x) + b);
				m_buffer[x + y * m_width] = color;
			}
		}
		else {
			for (int y = y1; y <= y2; y++) {
				int x = (int)round((y - b) / m);
				m_buffer[x + y * m_width] = color;
			}
		}
	}
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color) {
	ClipLine(x1, x2, y1, y2);

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);

	bool steep = std::abs(dy) > std::abs(dx);
	if (steep) {
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	dx = x2 - x1;
	dy = std::abs(y2 - y1);

	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	//std::cout << "Sorted points: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << ")\n";

	for (int x = x1, y = y1; x < x2; x++) {
		(steep) ? DrawPointClip(y, x, color) : DrawPointClip(x, y, color);

		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color) {
	if (x1 > x2) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}
	if (x2 > x3) {
		std::swap(x2, x3);
		std::swap(y2, y3);
	}
	if (x3 > x1) {
		std::swap(x3, x1);
		std::swap(y3, y1);
	}

	//std::cout << "Sorted points: (" << x1 << "," << y1 << "), (" << x2 << "," << y2 << "), (" << x3 << "," << y3 << ")\n";
	
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);
}

void Framebuffer::DrawCircle(int xc, int yc, int radius, const color_t& color) {
	int x = 0, y = radius;
	int d = 3 - 2 * radius;
	CircleBres(xc, yc, x, y, color);
	while (y >= x) {

		// check for decision parameter
		// and correspondingly 
		// update d, y
		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;

		// Increment x after updating decision parameter
		x++;

		// Draw the circle using the new coordinates
		CircleBres(xc, yc, x, y, color);
	}
}

void Framebuffer::DrawLinearCurve(int x1, int y1, int x2, int y2, const color_t& color) {
	float dt = 1.0f / 10;
	float t1 = 0;
	for (int i = 0; i < 10; i++) {
		int sx1 = Lerp(x1, x2, t1);
		int sy1 = Lerp(y1, y2, t1);

		float t2 = t1 + dt;
		
		int sx2 = Lerp(x1, x2, t2);
		int sy2 = Lerp(y1, y2, t2);

		t1 += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawQuadraticCurve(int x1, int y1, int x2, int y2, int x3, int y3, int steps, const color_t& color) {
	float dt = 1.0f / 10;
	float t1 = 0;
	for (int i = 0; i < 10; i++) {
		int sx1, sy1;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t1, sx1, sy1);
		float t2 = t1 + dt;
		int sx2, sy2;
		QuadraticPoint(x1, y1, x2, y2, x3, y3, t2, sx2, sy2);
		t1 += dt;
		
		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawCubicCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int steps, const color_t& color) {
	float dt = 1.0f / 10;
	float t1 = 0;
	for (int i = 0; i < 10; i++) {
		int sx1, sy1;
		CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t1, sx1, sy1);
		float t2 = t1 + dt;
		int sx2, sy2;
		CubicPoint(x1, y1, x2, y2, x3, y3, x4, y4, t2, sx2, sy2);
		t1 += dt;

		DrawLine(sx1, sy1, sx2, sy2, color);
	}
}

void Framebuffer::DrawImage(int x, int y, const Image& image) {
	// check if off-screen
	if (x + image.m_width < 0 || x >= m_width || y + image.m_height < 0 || y >= m_height) return;

	// iterate through image y
	for (int iy = 0; iy < image.m_height; iy++)	{
		// set screen y 
		int sy = y + iy;
		// check if off-screen, don't draw if off-screen
		if (sy < 0 || sy >= m_height) continue;

		// iterate through image x
		for (int ix = 0; ix < image.m_width; ix++) {
			// set screen x
			int sx = x + ix;
			// check if off-screen, don't draw if off-screen
			if (sx < 0 || sx >= m_width) continue;

			// get image pixel color
			color_t color = image.m_buffer[ix + iy * image.m_width];
			// check alpha, if 0 don't draw
			if (color.a == 0) continue;
			// set buffer to color
			DrawPoint(sx, sy, color);

			//m_buffer[sx + sy * m_width] = color;
		}
	}
}

void Framebuffer::DrawImage(int x, int y, int w, int h, const Image& image) {
	// check if off - screen
	if (x + w < 0 || x >= m_width || y + w < 0 || y >= m_height) return;

	float xScale = (float)image.m_width / (float)w;
	float yScale = (float)image.m_height / (float)h;

	// iterate through image y
	for (int iy = 0; iy < h; iy++) {
		// set screen y 
		int sy = y + iy;
		// check if off-screen, don't draw if off-screen
		if (sy < 0 || sy >= m_height) continue;

		int srcY = (int)(iy * yScale);
		if (srcY >= image.m_height) continue;

		// iterate through image x
		for (int ix = 0; ix < w; ix++) {
			// set screen x
			int sx = x + ix;
			// check if off-screen, don't draw if off-screen
			if (sx < 0 || sx >= m_width) continue;

			int srcX = (int)(ix * xScale);
			if (srcX >= image.m_width) continue;

			// get image pixel color
			color_t color = image.m_buffer[srcX + srcY * image.m_width];
			// check alpha, if 0 don't draw
			if (color.a == 0) continue;
			// set buffer to color
			m_buffer[sx + sy * m_width] = color;
		}
	}
}

void Framebuffer::CircleBres(int xc, int yc, int x, int y, const color_t& color) {
	DrawPoint(xc + x, yc + y, color);
	DrawPoint(xc - x, yc + y, color);
	DrawPoint(xc + x, yc - y, color);
	DrawPoint(xc - x, yc - y, color);
	DrawPoint(xc + y, yc + x, color);
	DrawPoint(xc - y, yc + x, color);
	DrawPoint(xc + y, yc - x, color);
	DrawPoint(xc - y, yc - x, color);
}

void Framebuffer::ClipLine(int& x1, int& y1, int& x2, int& y2) {
	int point1 = ClipCompute(x1, y1);
	int point2 = ClipCompute(x2, y2);

	bool accept = false;

	while (true) {
		if ((point1 == 0) && (point2 == 0)) {
			accept = true;
			break;
		}
		else if (point1 & point2) {
			break;
		}
		else {
			int point_out;
			int x, y;

			if (point1 != 0)
				point_out = point1;
			else
				point_out = point2;

			if (point_out & TOP) {
				x = x1 + (x2 - x1) * (maxY - y1) / (y2 - y1);
				y = maxY;
			} else if (point_out & BOTTOM) {
				x = x1 + (x2 - x1) * (minY - y1) / (y2 - y1);
				y = minY;
			} else if (point_out & RIGHT) {
				y = y1 + (y2 - y1) * (maxX - x1) / (x2 - x1);
				x = maxX;
			} else if (point_out & LEFT) {
				y = y1 + (y2 - y1) * (minX - x1) / (x2 - x1);
				x = minX;
			}

			if (point_out == point1) {
				x1 = x;
				y1 = y;
				point1 = ClipCompute(x1, y1);
			} else {
				x2 = x;
				y2 = y;
				point2 = ClipCompute(x2, y2);
			}
		}
	}
}

int Framebuffer::ClipCompute(int x, int y) {
	maxX = m_width;
	maxY = m_height;

	int point = INSIDE;

	if (x < minX) {
		point |= LEFT;
	} else if (x > maxX) {
		point |= RIGHT;
	}

	if (y < minY) {
		point |= BOTTOM;
	} else if (y > maxY) {
		point |= TOP;
	}

	return point;
}