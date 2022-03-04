#ifndef __MKBD_GUI_KEYBOARD_HPP__
#define __MKBD_GUI_KEYBOARD_HPP__

#include <mkbd/gui/graphic.hpp>

class KeyboardGraphic : public Graphic {
	int mKeyCount=52;
	int mKeyWidth;
	int mKeyHeight;
	int mBlackKeyWidth;
	int mBlackKeyHeight;
	Color mWhiteKeyColor = Color(255, 255, 255);
	Color mBlackKeyColor = Color(0, 0, 0);
	Color mKeyDownColor = Color(255, 255, 0);

public:
	bool keys[255] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	KeyboardGraphic(int x, int y, int width, int height);
	void draw(void);
	void init(void);
	void onClick(int button, int x, int y);
	
private:
	Color getWhiteKeyColor(int key);
	Color getBlackKeyColor(int key);
	void drawKeyboardBlackKey(int key, int i, int offset);
	void drawKeyboardBlackKeyLeft(int key, int i);
	void drawKeyboardBlackKeyRight(int key, int i);
	void drawKeyboardBlackKeyMiddle(int key, int i);
};

#endif