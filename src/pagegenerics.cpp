#include <mkbd/pagegenerics.hpp>

#include <mkbd/gui/graphics.hpp>
#include <mkbd/gui/style.hpp>

void generateFooter(Window& window) {
	RectangleComponent* bottomBar = new RectangleComponent(
		Layout(
			0, WindowStyle::headerHeight,
			0, WindowStyle::headerHeight,
			Layout::FillX | Layout::AnchorBottomLeft
		),
		WindowStyle::accColor
	);
	window.addComponent(bottomBar);
}
