#ifndef __MKBD_GUI_LAYOUT_HPP__
#define __MKBD_GUI_LAYOUT_HPP__


struct Layout {
	double x, y;
	double width, height;
	int details;

	enum {
		Default =               (0),
		PercentWidth =          (1 << 0),
		PercentHeight =         (1 << 1),
		FillX =                 (1 << 2),
		FillY =                 (1 << 3),
		AnchorTopRight =        (1 << 4),
		AnchorTopLeft =         (1 << 5),
		AnchorBottomLeft =      (1 << 6),
		AnchorBottomRight =     (1 << 7),
		AnchorCenter =          (1 << 8),
		AnchorTopCenter =       (1 << 9),
		AnchorBottomCenter =    (1 << 10),
		AnchorCenterLeft =      (1 << 11),
		AnchorCenterRight =     (1 << 12),
		PositionRelative =      (1 << 13),
		PositionAbsolute =      (1 << 14),
	};

	Layout(double x, double y, double width, double height, int details = Layout::Default)
	: x(x), y(y), width(width), height(height), details(details) {};

	Layout resetPosition(void) {
		Layout l = *this;
		l.details = Layout::Default;
		l.x = 0;
		l.y = 0;
		return l;
	};
};

#endif
