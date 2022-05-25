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
		PercentX =              (1 << 2),
		PercentY =              (1 << 3),
		FillX =                 (1 << 4),
		FillY =                 (1 << 5),
		AnchorTopRight =        (1 << 6),
		AnchorTopLeft =         (1 << 7),
		AnchorBottomLeft =      (1 << 8),
		AnchorBottomRight =     (1 << 9),
		AnchorCenter =          (1 << 10),
		AnchorTopCenter =       (1 << 11),
		AnchorBottomCenter =    (1 << 12),
		AnchorCenterLeft =      (1 << 13),
		AnchorCenterRight =     (1 << 14),
		PositionRelative =      (1 << 15),
		PositionAbsolute =      (1 << 16),
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
