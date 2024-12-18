#pragma once
#include <SDL.h>
#include <vector>

using color_t = SDL_Color;

class Framebuffer {
public:
	Framebuffer(const class Renderer& renderer, int width, int height);
	~Framebuffer();

	void Update();
	void Clear(const color_t& color);

	void DrawPoint(int x, int y, const color_t& color);
	void DrawPointClip(int x, int y, const color_t& color);

	void DrawRect(int x, int y, int w, int h, const color_t& color);
	void DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color);
	void DrawLine(int x1, int y1, int x2, int y2, const color_t& color);
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color);
	void DrawCircle(int xc, int yc, int radius, const color_t& color);

	void DrawLinearCurve(int x1, int y1, int x2, int y2, const color_t& color);
	void DrawQuadraticCurve(int x1, int y1, int x2, int y2, int x3, int y3, int steps, const color_t& color);
	void DrawCubicCurve(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int steps, const color_t& color);

	void DrawImage(int x, int y, const class Image& image);
	void DrawImage(int x, int y, int w, int h, const class Image& image);

	std::vector<float>& GetDepth() { return m_depth; }
private:	
	int INSIDE = 0; // 0000
	int LEFT = 1;	// 0001
	int RIGHT = 2;	// 0010
	int BOTTOM = 4; // 0100
	int TOP = 8;	// 1000

	int maxX;
	int maxY;
	const int minX = 0;
	const int minY = 0;

	void CircleBres(int xc, int yc, int x, int y, const color_t& color);
	void ClipLine(int& x1, int& y1, int& x2, int& y2);
	int ClipCompute(int x, int y);
	
	std::vector<float> m_depth;
public:
	int m_width{ 0 };
	int m_height{ 0 };
	int m_pitch{ 0 };

	SDL_Texture* m_texture{ nullptr };
	std::vector<color_t> m_buffer;
};