#ifndef __MKBD_GUI_WATERFALL_HPP__
#define __MKBD_GUI_WATERFALL_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/components/image.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/midi/recorder.hpp>
#include <mkbd/midi/keyboard.hpp>
#include <mkbd/midi/track.hpp>

#include <vector>
#include <string>

class WaterfallComponent : public Component {
	MidiRecorder* mRcdr;
	std::vector<MidiEvent> mEvents;
	std::vector<int> mKeyPositions;

	std::vector<MidiTrack*> mTracks;

	ImageComponent mBackgroundImage;

	std::string mLateBackgroundImagePath = "";

	Color mWhiteKeyColor = Color(161, 229, 92);
	Color mBlackKeyColor = Color(86, 157, 17);

	int mKeyCount=52;
	int mKeyWidth;
	int mKeyHeight;
	int mBlackKeyWidth;
	int mBlackKeyHeight;

public:
	WaterfallComponent(Layout layout, MidiRecorder* recorder);
	void draw(void);
	void init(void);
	void onResize(int width, int height);

	void addTrack(MidiTrack* track) { mTracks.push_back(track); };
	void clearTracks(void) { mTracks.clear(); };
	
private:
	double getKeyPressLength(const std::vector<MidiEvent>& events, std::vector<MidiEvent>::iterator it);
	double getTrackKeyPressLength(const std::vector<MidiEvent>& events, std::vector<MidiEvent>::iterator it);
	void calculateSizes(void);
	int getKeyWidth(int key);

	void drawBackground(void);
	void drawLines(void);
	void drawInput(void);
	void drawTracks(void);
	void drawTrack(MidiTrack* track);
};

#endif