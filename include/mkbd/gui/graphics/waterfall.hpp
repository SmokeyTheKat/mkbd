#ifndef __MKBD_GUI_WATERFALL_HPP__
#define __MKBD_GUI_WATERFALL_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/recorder.hpp>
#include <mkbd/keyboard.hpp>

#include <vector>

class WaterfallGraphic : public Graphic {
	KeyboardRecorder* mRcdr;
	std::vector<KeyboardMessage> mMsgs;
	std::vector<int> mKeyPositions;
	Color mWhiteKeyColor = Color(255, 255, 0);
	Color mBlackKeyColor = Color(255, 255, 0);
	int mKeyCount=52;
	int mKeyWidth;
	int mKeyHeight;
	int mBlackKeyWidth;
	int mBlackKeyHeight;

public:
	bool keys[255] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	WaterfallGraphic(Layout layout, KeyboardRecorder* recorder);
	void draw(void);
	void init(void);
	void onResize(int width, int height);
	
private:
	double getKeyPressLength(std::vector<KeyboardMessage>::iterator it);
	void calculateSizes(void);
	int getKeyWidth(int key);
};

#endif