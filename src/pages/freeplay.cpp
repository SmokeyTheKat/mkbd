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
#include <mkbd/midi/file.hpp>
#include <mkbd/midi/track.hpp>
#include <mkbd/filemanager.hpp>
//    [&, gen=gen, idx=idx](Layout layout) -> Component* { \


#define InstrumentButtonCreater(text, gen, idx) \
	[=, this](Layout layout) -> Component* { \
		ButtonComponent* bg = new ButtonComponent(layout, (text), [=, this](void) { \
			mActiveGen = (gen); \
			mCurrentInstrument = (idx); \
		}, Colors::White, Colors::Black); \
		bg->setFontSize(13); \
		return bg; \
	}

using namespace std::placeholders;

void App::freePlayPage(void) {
	SET_FID;

	chooseKeyboardPage();

	mWindow.newPage();

	MidiDevice piano(mMidiPort);
	MidiDevice piano2(0);
	mRecorder = MidiRecorder(&piano, 120);

	mAudioPlayer.start();

	int kgHeight = 200;

	WaterfallComponent* smg = new WaterfallComponent(
		Layout(
			0, mPianoControlHeight,
			0, kgHeight + mPianoControlHeight,
			Layout::FillX | Layout::FillY
		),
		&mRecorder
	);
	smg->on("LateDraw", asFunction([smg](void) {
//        smg->drawEllipse(340, 300, 0, 10, 30, 15);
//        smg->drawEllipse(300, 280, 0, 10, 30, 15);
//        smg->fillCircle(500, 250, 100);
//        smg->drawCircle(300, 250, 100);
	}));

	KeyboardComponent* kg = new KeyboardComponent(
		Layout(
			0, 0,
			0, kgHeight,
			Layout::FillX | Layout::AnchorBottomLeft
		),
		&mRecorder
	);
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

//    std::vector<MidiTrack>* tracks = new std::vector<MidiTrack>(MidiReader::load("./test.mid"));
//    for (MidiTrack& mt : *tracks) {
//        mt.attachRecorder(&mRecorder);
//    }
//    mRecorder.on("FirstNote", asFunction([this, tracks](void) {
//        for (MidiTrack& mt : *tracks) {
//            mt.reset();
//        }
//    }));
//    mRecorder.on("Update", asFunction([this, tracks](void) {
//        for (MidiTrack& mt : *tracks) {
//            if (mt.isNextEventReady()) {
//                mt.emit();
//            }
//        }
//    }));

	mWindow.addComponent(smg);
	mWindow.addComponent(kg);
	mWindow.addComponent(tg);

	ColorPickerComponent* cp = new ColorPickerComponent(
		Layout(100, 100, 100, 100), &gConfig.waterfallBackgroundColor
	);

	cp->on("Change", asFunction([this](void){
	}));

	FileSelectorComponent* test = new FileSelectorComponent(
		Layout(0, 0, 200, 30),
		gConfig.accColor, gConfig.borderColor
	);
	test->on("PreOpen", asFunction([this](void) {
		std::cout << "open\n";
		mRecorder.getTimer().pause();
	}));
	test->on("Change", asFunction([this](void) {
		std::cout << "Close\n";
		mRecorder.getTimer().unpause();
	}));

	test->on("Change", asFunction([this, test](void) {
		gConfig.waterfallBackgroundImage = true;
		gConfig.waterfallBackgroundImagePath = test->getPath();
		std::cout << gConfig.waterfallBackgroundImagePath << "\n";
	}));

	ButtonComponent* off = new ButtonComponent(
		Layout(0, 40, 200, 30),
		"Color",
		[](){}, gConfig.accColor, Colors::White
	);
	off->on("Click", FUNC((int b, int x, int y), {
		gConfig.waterfallBackgroundImage = false;
	}));

	ButtonComponent* mid = new ButtonComponent(
		Layout(0, 130, 200, 30),
		"Color",
		[](){}, gConfig.accColor, Colors::White
	);
	mRecorder.on("Event", asFunction<MidiEvent>([this, &piano](MidiEvent e) {
//        piano.sendEvent(e);
	}));
	mRecorder.on("SustainChange", asFunction<byte>([this, &piano, &piano2](byte val) {
//        std::cout << (int)val << "\n";
		uint16_t value = rmap(55, 0, 127, 0, 16000);
		byte b1 = (value << 2) >> 9;
		byte b2 = (value << 9) >> 9;
		b1 &= ~(1 << 7);
		b2 &= ~(1 << 7);
//        std::cout << value << " " << (int)b1 << " " << (int)b2 << "\n";
//        MidiEvent e = MidiEvent(MidiEvent::PitchWheel, { b2, b1 });
//        std::cout << e << "\n";
//        piano.sendEvent(e);
//        piano.sendEvent(MidiEvent(MidiEvent::ControlChange, { 1, val }));
	}));
	mRecorder.on("NoteOn", asFunction<byte, byte>([this, &piano2](byte note, byte vel) {
//        piano2.sendEvent(MidiEvent(MidiEvent::NoteOn, { note, vel }));
	}));
	mRecorder.on("NoteOff", asFunction<byte>([this, &piano2](byte note) {
//        piano2.sendEvent(MidiEvent(MidiEvent::NoteOff, { note, 0 }));
	}));

	MidiFile* mfile = new MidiFile();

//    std::vector<MidiTrack>* tracks;

	SeekBarComponent* sb = new SeekBarComponent(
		Layout(0, kgHeight, 0, 30, Layout::FillX | Layout::AnchorBottomLeft),
		&mRecorder.getTimer()
	);
	sb->setLength(0);
	sb->on("Seek", asFunction([this, mfile](void) {
//        for (auto& t : *tracks) {
//            t.resync();
//        }
//        mRecorder.resetNotes();
	}));

	mid->on("Click", asFunction<int, int, int>([this, smg, mfile, sb](int b, int x, int y) {
		SET_FID;

		mRecorder.clearGroup(FID);
		mRecorder.restart();

		std::string filePath = FileManager::selectFile("Open Midi File");
//        mfile.load(filePath);
//        for (MidiTrack& mt : *tracks) {
//            mt.attachRecorder(&mRecorder);
//            mt.reset();
//            std::cout << mt.getTLength(mRecorder.getBpm()) << "\n";
//        }
		double max = 0;
//        for (auto& t : *tracks) {
//            double tlength = t.getLength();
//            if (tlength > max) {
//                max = tlength;
//            }
//        }
		sb->setLength(max);
		smg->clearTracks();
//        mRecorder.setBpm((*tracks)[0].getBpm());
//        for (auto& t : *tracks) {
//            smg->addTrack(&t);
//        }
//        mRecorder.on("FirstNote", asFunction([this, tracks](void) {
//            std::cout << "OMG\n";
//            for (MidiTrack& mt : *tracks) {
//                mt.reset();
//            }
//        }));
		mRecorder.on("Update", asFunction([this](void) {
//            for (MidiTrack& mt : *tracks) {
//                if (mt.isNextEventReady()) {
//                    mt.emit();
//                }
//            }
		}), FID);
	}));

	CheckBoxComponent* cb = new CheckBoxComponent(
		Layout(0, 80, 30, 30),
		gConfig.accColor, Colors::Black
	);
	cb->on("Change", FUNC((bool b), {
		gConfig.keyBounceIn = b;
	}));

	FrameComponent* c = new FrameComponent(
		Layout(100, 100, 300, 500)
	);

	c->addChild(cb);
	c->addChild(mid);
	c->addChild(off);
	c->addChild(test);

	ButtonComponent* tog = new ButtonComponent(
		Layout(400, 200, 200, 30),
		"TOGGLE",
		[](){}, gConfig.accColor, Colors::White
	);
	tog->on("Click", asFunction<int, int, int>([this](int b, int x, int y) {
		if (mRecorder.getTimer().isPaused())
			mRecorder.getTimer().unpause();
		else mRecorder.getTimer().pause();
	}));

	ButtonComponent* bck = new ButtonComponent(
		Layout(600, 200, 100, 30),
		"BACK",
		[](){}, gConfig.accColor, Colors::White
	);
	bck->on("Click", asFunction<int, int, int>([this](int b, int x, int y) {
		mRecorder.getTimer().skip(-1);
//        for (auto& t : *tracks) {
//            t.resync();
//        }
		mRecorder.resetNotes();
	}));
	ButtonComponent* fwd = new ButtonComponent(
		Layout(700, 200, 100, 30),
		"BACK",
		[](){}, gConfig.accColor, Colors::White
	);
	fwd->on("Click", asFunction<int, int, int>([this](int b, int x, int y) {
		mRecorder.getTimer().skip(1);
//        for (auto& t : *tracks) {
//            t.resync();
//        }
		mRecorder.resetNotes();
	}));

	mWindow.addComponent(c);
	mWindow.addComponent(tog);
	mWindow.addComponent(bck);
	mWindow.addComponent(fwd);

//    mWindow.addComponent(cp);
	mWindow.addComponent(sb);

	generatePianoControls();

	attachRecorderToAudioPlayer();

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

//    std::vector<ButtonComponent*> buttons = createListComponent<ButtonComponent>(
//        Layout(20 + mInstrumentPanelWidth + 10, mPianoControlHeight + 20, 300, 20),
//        {
//            InstrumentButtonCreater("Piano", &pianoGen, 0),
//            InstrumentButtonCreater("Piano2", &piano2Gen, 1),
//            InstrumentButtonCreater("Piano3", &piano3Gen, 2),
//            InstrumentButtonCreater("synth", &synthGen, 3), 
//            InstrumentButtonCreater("Organ", &organGen, 4),
//            InstrumentButtonCreater("Brass", &brassGen, 5),
//            InstrumentButtonCreater("Reed", &reedGen, 6),
//            InstrumentButtonCreater("Phone", &phoneGen, 7),
//        },
//        -1
//    );
//
//    for (ButtonComponent* c : buttons) {
//        c->on("Click", asFunction<int, int, int>([this, buttons](int, int, int) {
//            std::cout << mCurrentInstrument << "\n";
//            for (int i = 0; i < buttons.size(); i++) {
//                ButtonComponent* c = buttons[i];
//                if (i == mCurrentInstrument)
//                    c->setBgColor(gConfig.accColor);
//                else
//                    c->setBgColor(Colors::White);
//            }
//        }));
//    }
//
//    buttons[0]->setBgColor(gConfig.accColor);
//
//    int border = 10;
//    int width = 400;
//    int height = 300;
//
//    RectangleComponent* panelBg1 = new RectangleComponent(
//        Layout(
//            width + border * 2 + border, mPianoControlHeight + border * 2 - border,
//            width + border * 2, height + border * 2,
//            Layout::AnchorTopRight
//        ),
//        gConfig.accColor
//    );
//
//    RectangleComponent* panelBg2 = new RectangleComponent(
//        Layout(
//            width + border * 2, mPianoControlHeight + border * 2,
//            width, height,
//            Layout::AnchorTopRight
//        ),
//        Colors::White
//    );
//
//    ButtonComponent* menuButton = new ButtonComponent(
//        Layout(
//            10, 5,
//            50, mPianoControlHeight - 10,
//            Layout::AnchorTopRight
//        ),
//        "#",
//        [](){},
//        gConfig.accColor, gConfig.fgColor
//    );
//
//    std::vector<Component*> instrumentPanel;
//
//    instrumentPanel.push_back(panelBg1);
//    instrumentPanel.push_back(panelBg2);
//
//    for (auto g : buttons) {
//        instrumentPanel.push_back(g);
//    }
//
//    for (auto g : instrumentPanel) {
//        g->setActive(!g->isActive());
//    }
//
//    menuButton->on("Click", asFunction([instrumentPanel](void) {
//        for (auto g : instrumentPanel) {
//            g->setActive(!g->isActive());
//        }
//    }));
//
//    panelBg1->on("OffClick", asFunction<int, int, int>([instrumentPanel](int button, int x, int y) {
//        for (auto g : instrumentPanel) {
//            g->setActive(false);
//        }
//    }));
//
//    mWindow.addComponent(menuButton);
//
//    for (auto g : instrumentPanel) {
//        mWindow.addComponent(g);
//    }
}

void App::generateInstrumentPanel(void) {

	int i = 0;
	std::vector<ComponentCreater> creaters;
	creaters.push_back(InstrumentButtonCreater("Piano", pianoGen, i)); i++;
	creaters.push_back(InstrumentButtonCreater("Piano2", piano2Gen, i)); i++;
	creaters.push_back(InstrumentButtonCreater("Piano3", piano3Gen, i)); i++;
	creaters.push_back(InstrumentButtonCreater("synth", synthGen, i)); i++;
	creaters.push_back(InstrumentButtonCreater("Organ", organGen, i)); i++;
	creaters.push_back(InstrumentButtonCreater("Brass", brassGen, i)); i++;
	creaters.push_back(InstrumentButtonCreater("Reed", reedGen, i)); i++;
	creaters.push_back(InstrumentButtonCreater("Phone", phoneGen, i)); i++;

	for (auto& s : instruments) {
		Generator* gen = s.getGenerator();
		std::string name = s.getName();
		creaters.push_back(InstrumentButtonCreater(name, gen, i));
		i++;
	};

	std::vector<ButtonComponent*> buttons = createListComponent<ButtonComponent>(
		Layout(20 + mInstrumentPanelWidth + 10, mPianoControlHeight + 20, 300, 20),
		creaters,
		-1
	);


	for (ButtonComponent* c : buttons) {
		c->on("Click", asFunction<int, int, int>([this, buttons](int, int, int) {
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
			0, 0,
//            -(bpmButtonSize / 2 + bpmTextWidth / 2) + 1, bpmButtonY,
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
			bpmTextWidth / 2 + bpmButtonSize / 2 - 2, bpmButtonY,
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
			bpmTextWidth / 2 + bpmButtonSize - 2 + bpmButtonSize / 2 - 1, bpmButtonY,
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
				mAudioPlayer.noteOn(metronomeGen, Music::noteToFreq(66), 127, 50);
			}), &mMetronomeOn);
		} else {
			metOn->getText().setColor(Colors::White);
			mRecorder.clearGroup(&mMetronomeOn);
		}
	}));
	InputComponent* bpmText = new InputComponent(
		Layout(
			0, bpmButtonY,
			bpmTextWidth, bpmButtonSize,
			Layout::AnchorTopCenter
		),
//        Style(Style::NoBorder
		"120",
		InputType::Number
	);

	metDown->leftOf(bpmText, -1);
	bpmText->setBgColor(gConfig.accColor);
	bpmText->setFgColor(gConfig.fgColor);
	bpmText->getTextComponent().setHAlign(HAlign::Middle);
	bpmText->on("Change", asFunction<std::string>([this](std::string text) {
		int value = 0;
		if (Utils::stringIsNumber(text)) {
			value = std::stoi(text);
		}
		mRecorder.setBpm(value);
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
