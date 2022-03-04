#include <mkbd/rect.hpp>

bool Rect::isPointIntersecting(int px, int py) {
	return (
		px >= x && px < (x + width) &&
		py >= y && py < (y + height)
	);
}
