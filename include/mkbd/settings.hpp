#ifndef __MKBD_SETTINGS_HPP__
#define __MKBD_SETTINGS_HPP__

#include <mkbd/color.hpp>
#include <string>

struct Settings {
	Color fgColor = Color(186, 238, 255);
	Color bgColor = Color(176, 205, 219);
	Color accColor = Color(16, 126, 181);
	Color borderColor = Color(150, 150, 150);

	Color whiteKeyColor = Color(255, 255, 255);
	Color blackKeyColor = Color(50);
//    Color whiteKeyDownColor = Color(135, 170, 207);
//    Color blackKeyDownColor = Color(55, 107, 174);
	Color whiteKeyDownColor = Color(161, 229, 92);
	Color blackKeyDownColor = Color(86, 157, 17);

	Color waterfallBackgroundColor = Color(35);
	std::string waterfallBackgroundImagePath = RESOURCE_DIR "/images/waterfall4.jpg";

	bool waterfallBackgroundImage = false;
	bool showKeyPress = true;
	bool showKeys3D = true;
	bool showKeyDownColor = true;
};

extern Settings gConfig;

#endif