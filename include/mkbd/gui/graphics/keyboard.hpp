#ifndef __MKBD_GUI_KEYBOARD_HPP__
#define __MKBD_GUI_KEYBOARD_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/midi/recorder.hpp>
#include <mkbd/gui/layout.hpp>

class KeyboardGraphic : public Graphic {
	MidiRecorder* mRcdr;

	std::vector<int> mKeyPositions;
	int mKeyCount=52;
	int mKeyWidth;
	int mKeyHeight;
	int mBlackKeyWidth;
	int mBlackKeyHeight;

	Color mWhiteKeyColor = Color(255, 255, 255);
	Color mBlackKeyColor = Color(0, 0, 0);
	Color mKeyDownColor = Color(255, 255, 0);

	int mVKey = -1;

public:
	bool keys[255] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	KeyboardGraphic(Layout layout, MidiRecorder* rcdr);
	void draw(void);
	void init(void);
	void onClick(int button, int x, int y);
	void onMouseUp(int button, int x, int y);
	void onDrag(int x, int y);
	void onLeave(int x, int y);
	void onResize(int width, int height);
	
private:
	void calculateSizes(void);
	int getKeyAtPos(int x, int y);
	Color getWhiteKeyColor(int key);
	Color getBlackKeyColor(int key);
	void drawKeyboardBlackKey(int key, int i, int offset);
	void drawKeyboardBlackKeyLeft(int key, int i);
	void drawKeyboardBlackKeyRight(int key, int i);
	void drawKeyboardBlackKeyMiddle(int key, int i);
};

#endif