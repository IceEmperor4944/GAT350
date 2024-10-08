#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>

namespace PostProcess {
	void Invert(std::vector<color_t>& buffer) {
		/*for (auto& color : buffer) {
			color.r = 255 - color.r;
			color.g = 255 - color.g;
			color.b = 255 - color.b;
		}*/

		std::for_each(buffer.begin(), buffer.end(), [](auto& color) {
			color.r = 255 - color.r;
			color.g = 255 - color.g;
			color.b = 255 - color.b;
			});
	}
	void Monochrome(std::vector<color_t>& buffer) {
		std::for_each(buffer.begin(), buffer.end(), [](auto& color) {
			uint8_t avg = static_cast<uint8_t>(color.r + color.g + color.b) / 3;
			color.r = avg;
			color.g = avg;
			color.b = avg;
			});
	}
	void Brightness(std::vector<color_t>& buffer, int brightness) {
		std::for_each(buffer.begin(), buffer.end(), [&](auto& color) {
			color.r = static_cast<uint8_t>(Clamp(color.r + brightness, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(color.g + brightness, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(color.b + brightness, 0, 255));
			});
	}
}