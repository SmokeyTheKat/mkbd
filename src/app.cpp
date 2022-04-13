#include <mkbd/app.hpp>

#include <mkbd/midi/keyboard.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/midi/recorder.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/music.hpp>
#include <mkbd/math.hpp>
#include <mkbd/instruments.hpp>
#include <mkbd/audioplayer.hpp>
#include <mkbd/generator.hpp>
#include <mkbd/gui/graphics/keyboard.hpp>
#include <mkbd/gui/graphics/list.hpp>
#include <mkbd/gui/graphics/rectangle.hpp>
#include <mkbd/gui/graphics/input.hpp>
#include <mkbd/gui/graphics/image.hpp>
#include <mkbd/gui/graphics/sheetmusic.hpp>
#include <mkbd/gui/graphics/button.hpp>
#include <mkbd/gui/graphics/buttonlist.hpp>
#include <mkbd/gui/graphics/waterfall.hpp>
#include <mkbd/gui/graphics/text.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/virtualkeyboard.hpp>

using namespace std::placeholders;

static std::string getChord(MidiRecorder* rcdr) {
	std::vector<int> notes;
	for (int i = 0; i < 256; i++) {
		if (rcdr->getDevice()->getNoteState(i))
			notes.push_back(i);
	}

	std::string name;
	bool found = false;
	while (!found && notes.size() > 0) {
		name = Music::getChordName(notes);
		if (name.length() > 0)
			found = true;
		notes.pop_back();
	}
	return name;
}

App::App(int argc, char** argv)
: mWindow(1280, 640) {
	for (int i = 0; i < argc; i++)
		mArgs.push_back(argv[i]);
}

int App::main(void) {
	mWindow.setBgColor(RGB_ARGS(mBgColor));

	pianoInstrument.load();
	piano2Instrument.load();
	piano2Instrument.setVolume(0.2);

	mainMenuPage();

	return 0;
}

void App::generateFooter(void) {
	RectangleGraphic* bottomBar = new RectangleGraphic(Layout(0, mHeaderHeight, 0, mHeaderHeight, Layout::FillX | Layout::AnchorBottomLeft), mAccColor);
	mWindow.addGraphic(bottomBar);
}

void App::generateHeader(const char* title) {
	int gap = mHeaderHeight / 4;
	RectangleGraphic* topBar = new RectangleGraphic(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), mAccColor);
	TextGraphic* titleText = new TextGraphic(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), title, RESOURCE_DIR "/fonts/FreeSans.ttf", 30, mFgColor);
	titleText->setAlign(FC_ALIGN_CENTER);

	ButtonGraphic* backButton = new ButtonGraphic(Layout(10, gap, 70, mHeaderHeight - gap * 2), "Back", [this](void) {
		mWindow.popPage();
	}, Color(4, 92, 149), mFgColor);
	backButton->setFontSize(17);

	mWindow.addGraphic(topBar);
	mWindow.addGraphic(backButton);
	mWindow.addGraphic(titleText);
}

void App::mainMenuPage(void) {
	mWindow.newPage();

	generateMainMenuHeader();
	generateMainMenuButtons();
	generateMainSongView();
	generateFooter();

	mWindow.pageLoop();
}

void App::generateMainSongView(void) {
	int textHeight = 20;
	RectangleGraphic* background = new RectangleGraphic(
		Layout(
				mMenuWidth + 20, mHeaderHeight + textHeight + 20,
				mMenuWidth + 40, 2 * mHeaderHeight + textHeight + 40,
				Layout::FillX | Layout::FillY
		), mFgColor, mBorderColor
	);

	TextGraphic* text = new TextGraphic(
		Layout(
			mMenuWidth + 20, mHeaderHeight + 20 - textHeight / 2,
			0, textHeight
		),
		"Your Songs", RESOURCE_DIR "/fonts/FreeSans.ttf", textHeight, Color(100, 100, 100)
	);

	mWindow.addGraphic(text);
	mWindow.addGraphic(background);
}

void App::generateMainMenuHeader(void) {
	int gap = mHeaderHeight / 8;

	RectangleGraphic* topBar = new RectangleGraphic(Layout(0, 0, 0, mHeaderHeight, Layout::FillX), mAccColor);
	ImageGraphic* logo = new ImageGraphic(Layout(10, gap, 100 - gap * 2, mHeaderHeight - gap * 2), RESOURCE_DIR "/images/logo.png");

	mWindow.addGraphic(topBar);
	mWindow.addGraphic(logo);
}

void App::generateMainMenuButtons(void) {
	ButtonListGraphic* buttonList = new ButtonListGraphic(
		Layout(10, mHeaderHeight + 20, mMenuWidth, 50),
		10,
		{
			{ "Free Play", std::bind(&App::freePlayPage, this) },
			{ "Test", std::bind(&App::testPage, this) },
			{ "Settings", std::bind(&App::settingsPage, this) },
			{ "Exit", [this](void){
				mWindow.popPage();
			} },
		},
		25, Colors::White, mAccColor
	);

	mWindow.addGraphic(buttonList);
}

void App::newSettingsPage(void) {
	for (Graphic* g : mSettingsPage) {
		mWindow.removeGraphic(g);
		delete g;
	}
	mSettingsPage.clear();
}

void App::settingsPageTabMidiDevice(void) {
	newSettingsPage();

	std::vector<GraphicCreater> creaters;

	std::vector<MidiDevice::Info> keyboards = MidiDevice::getDevices();
	for (auto& ki : keyboards) {
		int port = ki.port;
		creaters.push_back([this, port, ki](Layout layout) -> Graphic* {
			return new ButtonGraphic(layout, ki.name, [this, port](void) {
				mMidiPort = port;
				mWindow.popPage();
			}, mAccColor, Colors::White);
		});
	}

	int buttonWidth = 500;
	int buttonHeight = 35;
	int buttonGap = 5;

	std::vector<Graphic*> buttons = createListGraphic(
		Layout(buttonWidth / -2, mHeaderHeight + 20, buttonWidth, buttonHeight, Layout::AnchorTopCenter),
		creaters, 5
	);

	for (auto g : buttons) {
		mWindow.addGraphic(g);
		mSettingsPage.push_back(g);
	}
}

void App::settingsPageTabMidiForwarding(void) {
	newSettingsPage();

	RectangleGraphic* rect = new RectangleGraphic(Layout(mMenuWidth + 20, mHeaderHeight + 20, 200, 400), Color(255, 255, 0), Color(0, 0, 0));
	
	mSettingsPage.push_back(rect);
	mWindow.addGraphic(rect);
}

void App::settingsPageTabColorScheme(void) {
}

void App::settingsPageInstruments(void) {
}

void App::settingsPageTabProformance(void) {
	newSettingsPage();

	InputGraphic* input = new InputGraphic(Layout(mMenuWidth + 20, mHeaderHeight + 20, 200, 40), std::to_string(mAudioPlayer.getSampleSize()), InputType::Number);
	input->setMaxValue(8129);
	input->setMinValue(0);

	input->on("change", asFunction<std::string>([this](std::string text) {
		if (stringIsNumber(text)) {
			int value = std::stoi(text);
			if (value < 64) return;
			mAudioPlayer.setSampleSize(value);
			mAudioPlayer.restart();
		}
	}));
	
	mSettingsPage.push_back(input);
	mWindow.addGraphic(input);
}

void App::settingsPageTabAbout(void) {
	
}

void App::settingsPage(void) {
	mWindow.newPage();

	mSettingsPage.clear();

	generateHeader("Settings");

	RectangleGraphic* rect = new RectangleGraphic(Layout(0, mHeaderHeight, mMenuWidth, mHeaderHeight - 1, Layout::FillY), mFgColor, Color(0, 0, 0));

	std::vector<Graphic*> buttons = createListGraphic(
		Layout(0, mHeaderHeight, mMenuWidth, 55),
		{
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Midi Device", std::bind(&App::settingsPageTabMidiDevice, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Midi Forwarding", std::bind(&App::settingsPageTabMidiForwarding, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Color Scheme", std::bind(&App::settingsPageTabColorScheme, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Instruments", std::bind(&App::settingsPageInstruments, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "Performance", std::bind(&App::settingsPageTabProformance, this), mFgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonGraphic(layout, "About", std::bind(&App::settingsPageTabAbout, this), mFgColor, Colors::Black);
			},
		}, -1
	);

	mWindow.addGraphic(rect);
	for (auto g : buttons)
		mWindow.addGraphic(g);

	mWindow.pageLoop();
}

void App::initVirtualKeyboard(MidiRecorder* rcdr) {
//    window.events.on("keydown")
}

void App::attachRecorderToAudioPlayer(MidiRecorder* rcdr) {
	rcdr->on("SustainChange", asFunction<byte>([this](byte sustain) {
		if (sustain == 0) {
			mAudioPlayer.sustainOff();
		} else {
			mAudioPlayer.sustainOn();
		}
	}));

	rcdr->on("NoteOn", asFunction<byte, byte>([this](byte note, byte velocity) {
		mAudioPlayer.noteOn(*mActiveGen, Music::noteToFreq(note), rmap(velocity, 0, 127, 0, 50));
	}));

	rcdr->on("NoteOff", asFunction<byte>([this](byte note) {
		mAudioPlayer.noteOff(Music::noteToFreq(note));
	}));
}

#include <fstream>
#include <sstream>

void App::testPage(void) {
	SET_FID;

	chooseKeyboardPage();

	mWindow.newPage();

	std::ifstream file("./test.mid");

	std::ostringstream ss;
	ss << file.rdbuf();

	std::string data = ss.str();

	file.close();

	MidiDevice piano(mMidiPort);
	MidiRecorder recorder(&piano, 120);
	mAudioPlayer.start();

	attachRecorderToAudioPlayer(&recorder);

	mAudioPlayer.unpause();
	recorder.record(0);

	mWindow.pageLoop();

	mWindow.clearGroup(FID);
}

void App::freePlayPage(void) {
	SET_FID;

	chooseKeyboardPage();

	mWindow.newPage();


	MidiDevice piano(mMidiPort);
	MidiRecorder recorder(&piano, 120);

	mAudioPlayer.start();

	int smHeight = 300;
	int kgHeight = 300;

	WaterfallGraphic* smg = new WaterfallGraphic(Layout(0, 0, 0, 300, Layout::FillX | Layout::FillY), &recorder);
	KeyboardGraphic* kg = new KeyboardGraphic(Layout(0, kgHeight, 0, kgHeight, Layout::FillX | Layout::AnchorBottomLeft), &recorder);
	TextGraphic* tg = new TextGraphic(Layout(0, 40, 0, 100), "Cmaj9", RESOURCE_DIR "/fonts/FreeSans.ttf", 50, Color(255, 255, 255));

#define InstrumentButtonCreater(text, gen) \
	[this](Layout layout) -> Graphic* { \
		ButtonGraphic* bg = new ButtonGraphic(layout, (text), [this](void) { \
			mActiveGen = (gen); \
		}, mAccColor, mFgColor); \
		bg->setFontSize(20); \
		return bg; \
	}

	std::vector<Graphic*> buttons = createListGraphic(
		Layout(10, 10, 100, 30),
		{
			InstrumentButtonCreater("Piano", &pianoGen),
			InstrumentButtonCreater("Piano2", &piano2Gen),
			InstrumentButtonCreater("Piano3", &piano3Gen),
			InstrumentButtonCreater("synth", &synthGen), 
			InstrumentButtonCreater("Organ", &organGen),
			InstrumentButtonCreater("Brass", &brassGen),
			InstrumentButtonCreater("Reed", &reedGen),
			InstrumentButtonCreater("Phone", &phoneGen),
			[this](Layout layout) -> Graphic* {
				ButtonGraphic* bg = new ButtonGraphic(layout, "Settings", [this](void) {
					settingsPage();
				}, mAccColor, mFgColor);
				bg->setFontSize(20);
				return bg;
			},
			[this, &recorder](Layout layout) -> Graphic* {
				ButtonGraphic* bg = new ButtonGraphic(layout, "Quit", [this, &recorder](void) {
					recorder.stop();
					mAudioPlayer.stop();
					mWindow.popPage();
				}, mAccColor, mFgColor);
				bg->setFontSize(20);
				return bg;
			}
		},
		10,
		ListDirection::Horizontal
	);

	mWindow.on("Exit", asFunction([this, &recorder](void) {
		recorder.stop();
		mAudioPlayer.stop();
		mWindow.popPage();
	}), FID);

	VirtualKeyboard vk(&recorder);
	mWindow.on("KeyDown", asFunction<int>(std::bind(&VirtualKeyboard::onKeyDown, &vk, _1)), FID);
	mWindow.on("KeyUp", asFunction<int>(std::bind(&VirtualKeyboard::onKeyUp, &vk, _1)), FID);

	recorder.on("Update", asFunction(std::bind(&Window::update, &mWindow)));

	recorder.on("SoftPedalDown", asFunction([this, &recorder](void) {
		mAudioPlayer.stop();
		recorder.stop();
		mWindow.popPage();
	}));

	recorder.on("PadOn", asFunction<byte, byte>([this, &recorder](byte note, byte velocity) {
		if (note != 16) return;
		mAudioPlayer.stop();
		recorder.stop();
		mWindow.popPage();
	}));

	recorder.on("NoteOn", asFunction<byte, byte>([this, &tg, &recorder](byte note, byte velocity) {
		tg->setText(getChord(&recorder));
	}));

	recorder.on("NoteOff", asFunction<byte>([this, &tg, &recorder](byte note) {
		tg->setText(getChord(&recorder));
	}));

	attachRecorderToAudioPlayer(&recorder);

	mWindow.addGraphic(smg);
	mWindow.addGraphic(kg);
	mWindow.addGraphic(tg);

	for (auto b : buttons)
		mWindow.addGraphic(b);

	mWindow.update();

	mAudioPlayer.unpause();

	recorder.record(0);

	mWindow.pageLoop();

	mWindow.clearGroup(FID);
}

void App::chooseKeyboardPage(void) {
	mWindow.newPage();

	generateHeader("Choose Keyboard");

	std::vector<ButtonDetails> buttons;

	std::vector<MidiDevice::Info> keyboards = MidiDevice::getDevices();
	for (auto& ki : keyboards) {
		int port = ki.port;
		ButtonDetails bd { ki.name.c_str(),
			[this, port](void) {
				mMidiPort = port;
				mWindow.popPage();
			}
		};
		buttons.push_back(bd);
	}

	int buttonWidth = 500;
	int buttonHeight = 35;
	int buttonGap = 5;

	ButtonListGraphic* buttonList = new ButtonListGraphic(
		Layout(buttonWidth / -2, mHeaderHeight + 20, buttonWidth, buttonHeight, Layout::AnchorTopCenter),
		buttonGap, buttons, 20, Colors::White, mAccColor
	);

	mWindow.addGraphic(buttonList);

	generateFooter();

	mWindow.pageLoop();
}
