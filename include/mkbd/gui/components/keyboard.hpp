#ifndef __MKBD_GUI_KEYBOARD_HPP__
#define __MKBD_GUI_KEYBOARD_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/midi/recorder.hpp>
#include <mkbd/gui/layout.hpp>

class KeyboardComponent : public Component {
	MidiRecorder* mRcdr;
	std::vector<int> mKeyPositions;
	int mKeyCount=52;
	int mBorder = 10;
	int mKeyWidth;
	int mKeyHeight;
	int mBlackKeyWidth;
	int mBlackKeyHeight;
	double mViewAngle = 0.13;
	int mPushDistance = 7;
	bool mLatch = true;
	int mVKey = -1;

public:
	KeyboardComponent(Layout layout, MidiRecorder* rcdr);
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
	void drawKey3DPart(int key, Color color, int x, int width, int height);
	void drawKeyPushShadowLeft(int i, Color color, int offset);
	void drawKeyPushShadowRight(int i, Color color, int offset);
	void drawKeyboardWhiteKey(int key, int i);
	void drawKeyboardBlackKey(int key, int i, int offset);
	void drawKeyboardBlackKeyLeft(int key, int i);
	void drawKeyboardBlackKeyRight(int key, int i);
	void drawKeyboardBlackKeyMiddle(int key, int i);
};

#endif