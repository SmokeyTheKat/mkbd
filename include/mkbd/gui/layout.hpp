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
		AnchorBottomLeft =      (1 << 5),
		AnchorBottomRight =     (1 << 6),
		AnchorCenter =          (1 << 7),
		AnchorTopCenter =       (1 << 8),
	};

	inline Layout(double x, double y, double width, double height, int details = Layout::Default)
	: x(x), y(y), width(width), height(height), details(details) {};
};

#endif