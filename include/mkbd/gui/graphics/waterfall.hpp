#ifndef __MKBD_GUI_WATERFALL_HPP__
#define __MKBD_GUI_WATERFALL_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/recorder.hpp>
#include <mkbd/keyboard.hpp>

#include <vector>

class WaterfallGraphic : public Graphic {
	MidiRecorder* mRcdr;
	std::vector<MidiEvent> mEvents;
	std::vector<int> mKeyPositions;
	Color mWhiteKeyColor = Color(255, 255, 0);
	Color mBlackKeyColor = Color(255, 255, 0);
	int mKeyCount=52;
	int mKeyWidth;
	int mKeyHeight;
	int mBlackKeyWidth;
	int mBlackKeyHeight;

	int mInvisible = 0;

public:
	bool keys[255] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	WaterfallGraphic(Layout layout, MidiRecorder* recorder);
	void draw(void);
	void init(void);
	void onResize(int width, int height);
	
private:
	double getKeyPressLength(std::vector<MidiEvent>::iterator it);
	void calculateSizes(void);
	int getKeyWidth(int key);
};

#endif