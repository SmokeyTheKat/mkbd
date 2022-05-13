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
#include <mkbd/pages.hpp>
#include <mkbd/gui/graphics.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/virtualkeyboard.hpp>
#include <mkbd/settings.hpp>

using namespace std::placeholders;

void App::freePlayPage(void) {
	SET_FID;

	chooseKeyboardPage();

	mWindow.newPage();

	MidiDevice piano(mMidiPort);
	mRecorder = MidiRecorder(&piano, 120);

	mAudioPlayer.start();

	int kgHeight = 200;

	WaterfallComponent* smg = new WaterfallComponent(Layout(0, mPianoControlHeight, 0, kgHeight + mPianoControlHeight, Layout::FillX | Layout::FillY), &mRecorder);
	KeyboardComponent* kg = new KeyboardComponent(Layout(0, kgHeight, 0, kgHeight, Layout::FillX | Layout::AnchorBottomLeft), &mRecorder);
	TextComponent* tg = new TextComponent(
		Layout(mInstrumentButtonWidth + 40, 50, 0, 100),
		"Cmaj9",
		RESOURCE_DIR "/fonts/FreeSans.ttf",
		30,
		Color(255, 255, 255)
	);

	mWindow.on("Exit", asFunction([this](void) {
		mRecorder.stop();
		mAudioPlayer.stop();
		mWindow.popPage();
	}), FID);

	VirtualKeyboard vk(&mRecorder);
	mWindow.on("KeyDown", asFunction<int>(std::bind(&VirtualKeyboard::onKeyDown, &vk, _1)), FID);
	mWindow.on("KeyUp", asFunction<int>(std::bind(&VirtualKeyboard::onKeyUp, &vk, _1)), FID);

	vk.on("KeyDown", asFunction<byte>([&piano](byte note) {
		Color c(125, 25, 212);
		c.toHsv();
		c.toRgb();
		piano.sendEvent(MidiEvent({MidiEvent::NoteOn, note, 100}));
	}), FID);
	vk.on("KeyUp", asFunction<byte>([&piano](byte note) {
		piano.sendEvent(MidiEvent({MidiEvent::NoteOff, note, 0}));
	}), FID);

	mRecorder.on("Event", asFunction<MidiEvent>([this, &piano](MidiEvent e) {
		if (e.length() <= 0) return;
//        piano.sendEvent(e);
	}));

	mRecorder.on("Update", asFunction(std::bind(&Window::update, &mWindow)));

	mRecorder.on("SoftPedalDown", asFunction([this](void) {
		mAudioPlayer.stop();
		mRecorder.stop();
		mWindow.popPage();
	}));

	mRecorder.on("PadOn", asFunction<byte, byte>([this](byte note, byte velocity) {
		if (note != 16) return;
		mAudioPlayer.stop();
		mRecorder.stop();
		mWindow.popPage();
	}));

	mRecorder.on("NoteOn", asFunction<byte, byte>([this, &tg](byte note, byte velocity) {
		tg->setText(getChord());
	}));

	mRecorder.on("NoteOff", asFunction<byte>([this, &tg](byte note) {
		tg->setText(getChord());
	}));

	attachRecorderToAudioPlayer();

	mWindow.addComponent(smg);
	mWindow.addComponent(kg);
	mWindow.addComponent(tg);

	ColorPickerComponent* cp = new ColorPickerComponent(
		Layout(100, 100, 200, 200), &gConfig.waterfallBackgroundColor
	);

	cp->on("Change", asFunction([this](void){
	}));

//    mWindow.addComponent(cp);

	generatePianoControls();

	mWindow.update();

	mAudioPlayer.unpause();

	mRecorder.record(0);

	mWindow.pageLoop();

	mWindow.clearGroup(FID);
}

void App::generatePianoControls(void) {
	ButtonComponent* backButton = new ButtonComponent(
		Layout(4, mPianoControlHeight / 4, 70, mPianoControlHeight / 2),
		"Back",
		[this](void) {
			mRecorder.stop();
			mAudioPlayer.stop();
			mWindow.popPage();
		},
		gConfig.accColor,
		Colors::White
	);
	backButton->setFontSize(12);

	RectangleComponent* background = new RectangleComponent(
		Layout(0, 0, 0, mPianoControlHeight, Layout::FillX),
		gConfig.accColor
	);

	mWindow.addComponent(background);
	mWindow.addComponent(backButton);

	generateInstrumentPanel();
	generateKeyboardSizePanel();
	generateBpmControls();
}

void App::generateKeyboardSizePanel(void) {
#define InstrumentButtonCreater(text, gen, idx) \
	[this](Layout layout) -> Component* { \
		ButtonComponent* bg = new ButtonComponent(layout, (text), [this](void) { \
			mActiveGen = (gen); \
			mCurrentInstrument = (idx); \
		}, Colors::White, Colors::Black); \
		bg->setFontSize(13); \
		return bg; \
	}

	std::vector<ButtonComponent*> buttons = createListComponent<ButtonComponent>(
		Layout(20 + mInstrumentPanelWidth + 10, mPianoControlHeight + 20, 300, 20),
		{
			InstrumentButtonCreater("Piano", &pianoGen, 0),
			InstrumentButtonCreater("Piano2", &piano2Gen, 1),
			InstrumentButtonCreater("Piano3", &piano3Gen, 2),
			InstrumentButtonCreater("synth", &synthGen, 3), 
			InstrumentButtonCreater("Organ", &organGen, 4),
			InstrumentButtonCreater("Brass", &brassGen, 5),
			InstrumentButtonCreater("Reed", &reedGen, 6),
			InstrumentButtonCreater("Phone", &phoneGen, 7),
		},
		-1
	);

	for (ButtonComponent* c : buttons) {
		c->on("Click", asFunction<int, int, int>([this, buttons](int, int, int) {
			std::cout << mCurrentInstrument << "\n";
			for (int i = 0; i < buttons.size(); i++) {
				ButtonComponent* c = buttons[i];
				if (i == mCurrentInstrument)
					c->setBgColor(gConfig.accColor);
				else
					c->setBgColor(Colors::White);
			}
		}));
	}

	buttons[0]->setBgColor(gConfig.accColor);

	int border = 10;
	int width = 400;
	int height = 300;

	RectangleComponent* panelBg1 = new RectangleComponent(
		Layout(
			width + border * 2 + border, mPianoControlHeight + border * 2 - border,
			width + border * 2, height + border * 2,
			Layout::AnchorTopRight
		),
		gConfig.accColor
	);

	RectangleComponent* panelBg2 = new RectangleComponent(
		Layout(
			width + border * 2, mPianoControlHeight + border * 2,
			width, height,
			Layout::AnchorTopRight
		),
		Colors::White
	);

	ButtonComponent* menuButton = new ButtonComponent(
		Layout(
			10 + 50, 5,
			50, mPianoControlHeight - 10,
			Layout::AnchorTopRight
		),
		"#",
		[](){},
		gConfig.accColor, gConfig.fgColor
	);

	std::vector<Component*> instrumentPanel;

	instrumentPanel.push_back(panelBg1);
	instrumentPanel.push_back(panelBg2);

	for (auto g : buttons) {
		instrumentPanel.push_back(g);
	}

	for (auto g : instrumentPanel) {
		g->setActive(!g->isActive());
	}

	menuButton->on("Click", asFunction([instrumentPanel](void) {
		for (auto g : instrumentPanel) {
			g->setActive(!g->isActive());
		}
	}));

	panelBg1->on("OffClick", asFunction<int, int, int>([instrumentPanel](int button, int x, int y) {
		for (auto g : instrumentPanel) {
			g->setActive(false);
		}
	}));

	mWindow.addComponent(menuButton);

	for (auto g : instrumentPanel) {
		mWindow.addComponent(g);
	}
}

void App::generateInstrumentPanel(void) {
#define InstrumentButtonCreater(text, gen, idx) \
	[this](Layout layout) -> Component* { \
		ButtonComponent* bg = new ButtonComponent(layout, (text), [this](void) { \
			mActiveGen = (gen); \
			mCurrentInstrument = (idx); \
		}, Colors::White, Colors::Black); \
		bg->setFontSize(13); \
		return bg; \
	}

	std::vector<ButtonComponent*> buttons = createListComponent<ButtonComponent>(
		Layout(20 + mInstrumentPanelWidth + 10, mPianoControlHeight + 20, 300, 20),
		{
			InstrumentButtonCreater("Piano", &pianoGen, 0),
			InstrumentButtonCreater("Piano2", &piano2Gen, 1),
			InstrumentButtonCreater("Piano3", &piano3Gen, 2),
			InstrumentButtonCreater("synth", &synthGen, 3), 
			InstrumentButtonCreater("Organ", &organGen, 4),
			InstrumentButtonCreater("Brass", &brassGen, 5),
			InstrumentButtonCreater("Reed", &reedGen, 6),
			InstrumentButtonCreater("Phone", &phoneGen, 7),
		},
		-1
	);

	for (ButtonComponent* c : buttons) {
		c->on("Click", asFunction<int, int, int>([this, buttons](int, int, int) {
			std::cout << mCurrentInstrument << "\n";
			for (int i = 0; i < buttons.size(); i++) {
				ButtonComponent* c = buttons[i];
				if (i == mCurrentInstrument)
					c->setBgColor(gConfig.accColor);
				else
					c->setBgColor(Colors::White);
			}
		}));
	}

	buttons[0]->setBgColor(gConfig.accColor);

	int border = 10;

	RectangleComponent* instPanelBg = new RectangleComponent(
		Layout(20 + mInstrumentPanelWidth + 10 - border, mPianoControlHeight + 20 - border, 300 + border * 2, 400 + border  * 2),
		gConfig.accColor
	);

	RectangleComponent* instPanelBg2 = new RectangleComponent(
		Layout(20 + mInstrumentPanelWidth + 10, mPianoControlHeight + 20, 300, 400),
		Colors::White
	);

	RectangleComponent* instBackground = new RectangleComponent(
		Layout(10, mPianoControlHeight + 10, mInstrumentPanelWidth, mInstrumentPanelHeight),
		gConfig.accColor
	);

	ButtonComponent* instButton = new ButtonComponent(
		Layout(20, mPianoControlHeight + 20, mInstrumentButtonWidth, mInstrumentButtonWidth),
		"@",
		[](){},
		gConfig.accColor,
		gConfig.fgColor
	);

	ButtonComponent* colorButton = new ButtonComponent(
		Layout(20, mPianoControlHeight + 30 + mInstrumentButtonWidth, mInstrumentButtonWidth, mInstrumentButtonWidth),
		"C",
		[](){},
		gConfig.accColor,
		Colors::Green
	);

	instButton->setFontSize(20);

	std::vector<Component*> instrumentPanel;

	instrumentPanel.push_back(instPanelBg);
	instrumentPanel.push_back(instPanelBg2);

	for (auto g : buttons) {
		instrumentPanel.push_back(g);
	}

	for (auto g : instrumentPanel) {
		g->setActive(false);
	}

	instButton->on("Click", asFunction([instrumentPanel, instButton](void) {
		for (auto g : instrumentPanel) {
			g->setActive(!g->isActive());
		}
	}));


	instPanelBg->on("OffClick", asFunction<int, int, int>([instrumentPanel, instButton](int button, int x, int y) {
		for (auto g : instrumentPanel) {
			g->setActive(false);
		}
	}));

	mWindow.addComponent(instBackground);
	mWindow.addComponent(instButton);
	mWindow.addComponent(colorButton);

	for (auto g : instrumentPanel) {
		mWindow.addComponent(g);
	}
}

void App::generateBpmControls(void) {
	int bpmButtonSize = 40;
	int bpmButtonY = mPianoControlHeight / 2 - bpmButtonSize / 2;
	int bpmTextWidth = 100;
	int bpmButtonGap = 1;

	ButtonComponent* metUp = new ButtonComponent(
		Layout(
			bpmTextWidth / 2 - 1, bpmButtonY,
			bpmButtonSize, bpmButtonSize,
			Layout::AnchorTopCenter
		),
		"+",
		[this](void) {
			mRecorder.setBpm(mRecorder.getBpm() + 5);
		},
		gConfig.accColor,
		Colors::White
	);

	ButtonComponent* metDown = new ButtonComponent(
		Layout(
			-bpmButtonSize - bpmTextWidth / 2 + 1, bpmButtonY,
			bpmButtonSize, bpmButtonSize,
			Layout::AnchorTopCenter
		),
		"-",
		[this](void) {
			mRecorder.setBpm(mRecorder.getBpm() - 5);
		},
		gConfig.accColor,
		Colors::White
	);

	ButtonComponent* metOn = new ButtonComponent(
		Layout(
			bpmTextWidth / 2 + bpmButtonSize - 2, bpmButtonY,
			bpmButtonSize, bpmButtonSize,
			Layout::AnchorTopCenter
		),
		"&",
		[](){},
		gConfig.accColor,
		Colors::White
	);

	ButtonComponent* controlUp = new ButtonComponent(
		Layout(
			bpmTextWidth / 2 + bpmButtonSize - 2 + bpmButtonSize - 1, bpmButtonY,
			bpmButtonSize, bpmButtonSize,
			Layout::AnchorTopCenter
		),
		"+",
		[](){},
		gConfig.accColor,
		Colors::White
	);

	metOn->on("Click", asFunction([this, metOn](void) {
		mMetronomeOn = !mMetronomeOn;
		if (mMetronomeOn) {
			metOn->getText().setColor(Colors::Green);
			mRecorder.on("Beat", asFunction([this](void) {
				mAudioPlayer.noteOn(metronomeGen, Music::noteToFreq(66), 50);
			}), &mMetronomeOn);
		} else {
			metOn->getText().setColor(Colors::White);
			mRecorder.clearGroup(&mMetronomeOn);
		}
	}));
	InputComponent* bpmText = new InputComponent(
		Layout(
			-bpmTextWidth / 2, bpmButtonY,
			bpmTextWidth, bpmButtonSize,
			Layout::AnchorTopCenter
		),
//        Style(Style::NoBorder
		"120",
		InputType::Number
	);
	bpmText->setBgColor(gConfig.accColor);
	bpmText->setFgColor(gConfig.fgColor);
	bpmText->getTextComponent().setAlign(FC_ALIGN_CENTER);
	bpmText->on("Change", asFunction<std::string>([this](std::string text) {
		int value = 0;
		if (stringIsNumber(text)) {
			value = std::stoi(text);
		}
		mRecorder.setBpm(value);
	}));
	bpmText->on("Hover", asFunction<int, int>([this](int x, int y) {
		std::cout << "hover " << x << " " << y << "\n";
	}));
	mRecorder.on("Update", asFunction([this, bpmText](void) {
		bpmText->setText(std::to_string(mRecorder.getBpm()));
	}));

	mWindow.addComponent(metUp);
	mWindow.addComponent(metDown);
	mWindow.addComponent(metOn);
	mWindow.addComponent(controlUp);
	mWindow.addComponent(bpmText);
}
