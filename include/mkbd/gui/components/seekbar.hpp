#ifndef __MKBD_GUI_SEEKBAR_HPP__
#define __MKBD_GUI_SEEKBAR_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/gui/components/text.hpp>

#include <string>

class SeekBarComponent : public Component {
	Timer* mTimer = 0;
	double mLength = 0;
	int mLastTime = 0;

	TextComponent* mCurrentTimeText;
	TextComponent* mLengthTimeText;

public:
	SeekBarComponent(Layout layout, Timer* timer);

	void setLength(double length);

	void init(void);
	void onClick(int button, int x, int y);
	void onDrag(int x, int y);
	void draw(void);
};

#endif