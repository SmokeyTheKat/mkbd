#include <mkbd/keyboard.hpp>
#include <mkbd/recorder.hpp>
#include <mkbd/player.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/music.hpp>
#include <mkbd/math.hpp>
#include <mkbd/audioplayer.hpp>
#include <mkbd/generator.hpp>
#include <mkbd/gui/graphics/keyboard.hpp>
#include <mkbd/gui/graphics/rectangle.hpp>
#include <mkbd/gui/graphics/image.hpp>
#include <mkbd/gui/graphics/sheetmusic.hpp>
#include <mkbd/gui/graphics/button.hpp>
#include <mkbd/gui/graphics/text.hpp>
#include <mkbd/gui/window.hpp>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <unistd.h>

static int screenWidth = 1280;
static int screenHeight = 640;

static int barHeight = 60;

static int gPort = 1;

static Color fg = Color(0xe1, 0xe2, 0xe2);
static Color bg = Color(0x1d, 0x22, 0x28);
static Color bc = Color(0xfb, 0x81, 0x22);

Generator pianoGen;
Generator phoneGen;
Generator brassGen;
Generator reedGen;
Generator organGen;
Generator* activeGen = &organGen;

static std::string getChord(KeyboardRecorder* rcdr) {
	std::vector<int> notes;
	for (int i = 0; i < 256; i++) {
		if (rcdr->getKeyboard()->getKeyState(i))
			notes.push_back(i);
	}
	return getChordName(notes);
}
//
//static int chooseKeyboard(void) {
//    std::vector<KeyboardInfo> keyboards = Keyboard::getKeyboards();
//
//    if (keyboards.size() == 0)
//        return -1;
//
//    std::cout << "Open MIDI ports:\n";
//    for (KeyboardInfo kbd : keyboards) {
//        std::cout << "Port " << kbd.port << " \"" << kbd.name << "\"\n";
//    }
//    std::cout << "Enter port number (0-" << keyboards.size() << ")\n";
//
//    unsigned int port;
//    std::cin >> port;
//
//    if (port >= keyboards.size())
//        return chooseKeyboard();
//
//    return port;
//}
//
//static int chooseBpm(const Keyboard& kbd) {
//    std::cout << "Enter bmp (0 for record)\n";
//    int bpm;
//    std::cin >> bpm;
//    if (bpm != 0)
//        return bpm;
//
//    Timer timer;
//    int beats = 0;
//    bool firstKeyPressed = false;
//
//    while (!firstKeyPressed || timer.getLap() < 2.0) {
//        KeyboardMessage msg = kbd.getMessage();
//
//        if (msg.getType() == KBD_MSG_KEY) {
//            if (msg[2] == 0) continue;
//            if (!firstKeyPressed) timer.start();
//            firstKeyPressed = true;
//            timer.lap();
//            beats++;
//        }
//
//        usleep(100);
//    }
//
//    return (double)beats / timer.stop() * 60.0;
//}
//
//static void drawLine(int width, const char* style) {
//    std::cout << style;
//    for (int i = 0; i < width; i++)
//        std::cout << " ";
//    std::cout << "\n";
//}
//
//static void drawSheet(KeyboardRecorder* rcdr) {
//    std::cout << "\x1b[2J\x1b[1;1H\n\n";
//
//    int width = 170;
//    int height = 27;
//
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//
//    drawLine(width, "\x1b[0m");
//
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//    drawLine(width, "\x1b[9m");
//    drawLine(width, "\x1b[0m");
//
//    std::cout << "\x1b[1;" << (int)((width / 4) * 1) << "H";
//    for (int y = 0; y < height; y++)
//        std::cout << "│" << "\x1b[B\x1b[D";
//
//    std::cout << "\x1b[1;" << (int)((width / 4) * 2) + 1 << "H";
//    for (int y = 0; y < height; y++)
//        std::cout << "│" << "\x1b[B\x1b[D";
//
//    std::cout << "\x1b[1;" << (int)((width / 4) * 3) + 1 << "H";
//    for (int y = 0; y < height; y++)
//        std::cout << "│" << "\x1b[B\x1b[D";
//
//    std::cout << "\x1b[1;" << (int)((width / 4) * 4) + 1 << "H";
//    for (int y = 0; y < height; y++)
//        std::cout << "│" << "\x1b[B\x1b[D";
//
//    std::vector<Key>& keys = rcdr->getNotes();
//
//    double barLength = 60.0 / (double)rcdr->getBpm() * 4.0;
//    double fullLength = barLength * 4;
//    double startTime = std::floor(rcdr->getTime() / fullLength) * fullLength;
//
//    double now = roundTo(rcdr->getTime() - startTime, barLength / 4.0);
//
//    std::cout << "\x1b[38;2;255;255;0m" << "\x1b[1;" << (int)rmap(now, 0, fullLength, 0, width) << "H";
//    for (int y = 0; y < height; y++)
//        std::cout << "│" << "\x1b[B\x1b[D";
//
//    std::cout << "\x1b[0m\n";
//
//    if (keys.size() == 0) return;
//
//    for (int i = 0; i < keys.size(); i++) {
//        Key key = keys[i];
//        if (key.time < startTime) continue;
//        double time = roundTo(key.time - startTime, barLength / 4.0 / 4.0);
//        time = key.time - startTime;
//        if (time > fullLength) {
//            break;
//        }
//        if (key.id % 2 == 0) std::cout << "\x1b[9m";
//        else std::cout << "\x1b[0m";
//        std::cout << "\x1b[" << 42 - key.id << ";"
//                  << (int)rmap(time, 0, fullLength, 0, width) - (key.flat ? 0 : 0)
//                  << (key.flat ? "HO" : "H♭O");
//    }
//
//    std::cout << "\x1b[35;1H" << getChord(rcdr) << "\n";
//}
//
//void looperSetLoop(KeyboardRecorder* rcdr) {
//    static int isRecording = false;
//    isRecording = !isRecording;
//
//    if (isRecording) {
//        rcdr->clear();
//        return;
//    }
//
//    std::vector<KeyboardMessage> rcdrMsgs = rcdr->getMessages();
//    std::vector<KeyboardMessage> msgs = rcdrMsgs;
//
//    double barLength = 60.0 / (double)rcdr->getBpm() * 4.0;
//    double fullLength = barLength * 4;
//    double firstKeyTime = 0;
//
//    for (auto m : msgs) {
//        if (m.getType() == KBD_MSG_KEY) {
//            firstKeyTime = m.stamp;
//            break;
//        }
//    }
//
//    double startTime = std::floor(firstKeyTime / fullLength) * fullLength;
//
//    for (auto m : msgs) {
//        if (m.stamp < startTime) continue;
//        if (m.data.size() >= 3) {
//            rcdr->addTimedCallback([m](KeyboardRecorder* rcdr) {
//                rcdr->getKeyboard()->sendMessage(m);
//            }, m.stamp - startTime, fullLength);
//        }
//    }
//}
//
//void runLooper(void) {
//    Keyboard piano(chooseKeyboard());
//
//    int bpm = chooseBpm(piano);
//
//    KeyboardRecorder recorder(&piano, bpm);
//
//    recorder.onBeat = [](KeyboardRecorder* rcdr) {
//        drawSheet(rcdr);
//    };
//
//    recorder.onSoftPedalDown = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        rcdr->stop();
//    };
//
//    recorder.onKeyDown = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        drawSheet(rcdr);
//    };
//
//    recorder.onMiddlePedalDown = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        looperSetLoop(rcdr);
//    };
//
//    recorder.onPadDown = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        if (msg[1] != 16) return;
//        rcdr->getMessages().pop_back();
//        looperSetLoop(rcdr);
//    };
//
//    recorder.record(0);
//}
//
//void playSynth(int port) {
//    Keyboard piano(port);
//
//    KeyboardRecorder recorder(&piano, 120);
//
//    window.clearPage();
//
//    SheetMusicGraphic smg(0, 0, 1280, 300, &recorder);
//    window.addGraphic(&smg);
//
//    KeyboardGraphic kg(0, 300, 1280, 200);
//    window.addGraphic(&kg);
//    KeyboardGraphic kg2(0, 300, 1280, 200);
//
//    TextGraphic tg(0, 550, 1280, 100, "Cmaj9", "fonts/FreeSans.ttf", 50, Color(255, 255, 255));
//    window.addGraphic(&tg);
//
//    int buttonX = 10;
//    int buttonY = 510;
//    int buttonWidth = 100;
//    int buttonHeight = 30;
//
//    ButtonGraphic butPiano(buttonX, buttonY, buttonWidth, buttonHeight, "Piano", [=](void) {
//        activeGen = &pianoGen;
//    }, bc, fg);
//    butPiano.setFontSize(20);
//    window.addGraphic(&butPiano);
//    buttonX += buttonWidth + 10;
//
//    ButtonGraphic butOrgan(buttonX, buttonY, buttonWidth, buttonHeight, "Organ", [=](void) {
//        activeGen = &organGen;
//    }, bc, fg);
//    butOrgan.setFontSize(20);
//    window.addGraphic(&butOrgan);
//    buttonX += buttonWidth + 10;
//
//    ButtonGraphic butBrass(buttonX, buttonY, buttonWidth, buttonHeight, "Brass", [=](void) {
//        activeGen = &brassGen;
//    }, bc, fg);
//    butBrass.setFontSize(20);
//    window.addGraphic(&butBrass);
//    buttonX += buttonWidth + 10;
//
//    ButtonGraphic butReed(buttonX, buttonY, buttonWidth, buttonHeight, "Reed", [=](void) {
//        activeGen = &reedGen;
//    }, bc, fg);
//    butReed.setFontSize(20);
//    window.addGraphic(&butReed);
//    buttonX += buttonWidth + 10;
//
//    ButtonGraphic butPhone(buttonX, buttonY, buttonWidth, buttonHeight, "Phone", [=](void) {
//        activeGen = &phoneGen;
//    }, bc, fg);
//    butPhone.setFontSize(20);
//    window.addGraphic(&butPhone);
//    buttonX += buttonWidth + 10;
//
//    window.update();
//
//    AudioPlayer ap;
//    ap.start();
//
//    window.onExit = [&ap, &recorder](Window* window) {
//        recorder.stop();
//        ap.stop();
//    };
//
//    recorder.onMessage = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        window.update();
//    };
//
//    recorder.onSoftPedalDown = [&ap](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        ap.stop();
//        rcdr->stop();
//        window.popPage();
//    };
//
//    recorder.onPadDown = [&ap](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        if (msg[1] != 16) return;
//        ap.stop();
//        rcdr->stop();
//        window.popPage();
//    };
//
//    recorder.onKeyDown = [&ap, &kg, &tg](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        kg.keys[msg[1] - 12] = 1;
//        tg.setText(getChord(rcdr));
//        window.update();
//        ap.addSample(*activeGen, std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440, rmap(msg[2], 0, 127, 0, 50));
//    };
//
//    recorder.onKeyUp = [&ap, &kg, &tg](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        kg.keys[msg[1] - 12] = 0;
//        tg.setText(getChord(rcdr));
//        window.update();
//        ap.removeSample(std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440);
//    };
//
//    recorder.record(0);
//}
//
//void runSynth(void) {
//    window.newPage();
//
//    std::vector<KeyboardInfo> keyboards = Keyboard::getKeyboards();
//
//    std::vector<ButtonGraphic> keyboardOptions;
//
//    int buttonWidth = 500;
//    int buttonHeight = 30;
//    int buttonX = (window.getWidth() / 2) - (buttonWidth / 2);
//
//    int buttonY = 100;
//
//    for (auto& ki : keyboards) {
//        int port = ki.port;
//        std::cout << "port: " << port << "\n";
//        ButtonGraphic bg(buttonX, buttonY, buttonWidth, buttonHeight, ki.name, [port](void) {
//            window.clearPage();
//            playSynth(port);
//        }, bc, fg);
//        bg.setFontSize(20);
//
//        keyboardOptions.push_back(bg);
//        buttonY += buttonHeight + 5;
//    }
//
//    RectangleGraphic topBar(0, 0, screenWidth, barHeight, bc);
//    ImageGraphic logo(0, 0, 100, 60, "./logo.png");
//
//    window.addGraphic(&topBar);
//    window.addGraphic(&logo);
//
//    for (auto& bg : keyboardOptions) {
//        window.addGraphic(&bg);
//    }
//
//    window.eventLoop();
//}
//
//void runRawView(void) {
//    Keyboard piano(chooseKeyboard());
//
//    KeyboardRecorder recorder(&piano, 120);
//
//    recorder.onMessage = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        if (msg.data.size() == 0) return;
//        for (byte i : msg.data) {
//            std::cout << (int)i << ", ";
//        }
//        std::cout << "\n";
//    };
//
//    recorder.record(0);
//}
//
//void mkbdExit(void) {
//    window.close();
//}
//
//void runModeSelect(void) {
//    int barHeight = 60;
//    window.setBgColor(0x1d, 0x22, 0x28);
//    RectangleGraphic topBar(0, 0, screenWidth, barHeight, bc);
//    ImageGraphic logo(0, 0, 100, 60, "./logo.png");
//
//    int buttonWidth = 300;
//    int buttonHeight = 60;
//    int buttonX = 10;
//    int buttonY = barHeight + 20;
//
//    ButtonGraphic buttonFreePlay(buttonX, buttonY, buttonWidth, buttonHeight, "Free Play", runSynth, bc, fg);
//    buttonFreePlay.setFontSize(34);
//    buttonY += buttonHeight + 10;
//    ButtonGraphic buttonForwarding(buttonX, buttonY, buttonWidth, buttonHeight, "Midi Forwarding", runSynth, bc, fg);
//    buttonForwarding.setFontSize(34);
//    buttonY += buttonHeight + 10;
//    ButtonGraphic buttonSettings(buttonX, buttonY, buttonWidth, buttonHeight, "Settings", runSynth, bc, fg);
//    buttonSettings.setFontSize(34);
//    buttonY += buttonHeight + 10;
//    ButtonGraphic buttonExit(buttonX, buttonY, buttonWidth, buttonHeight, "Exit", mkbdExit, bc, fg);
//    buttonExit.setFontSize(34);
//
//    window.addGraphic(&topBar);
//    window.addGraphic(&logo);
//    window.addGraphic(&buttonFreePlay);
//    window.addGraphic(&buttonForwarding);
//    window.addGraphic(&buttonSettings);
//    window.addGraphic(&buttonExit);
//
//    window.eventLoop();
//}
void selectKeyboardPage(Window* window);
void freePlayPage(Window* window);
void pageHeaderGraphics(Window* window);
void settingsPage(Window* window);
void mainMenuButtonGraphics(Window* window);
void mainMenuPage(Window* window);

void selectKeyboardPage(Window* window) {
	window->newPage();

	pageHeaderGraphics(window);

	int buttonWidth = 500;
	int buttonHeight = 30;
	int buttonX = (window->getWidth() / 2) - (buttonWidth / 2);
	int buttonY = 100;
	int buttonGap = 5;

	std::vector<KeyboardInfo> keyboards = Keyboard::getKeyboards();
	std::vector<ButtonGraphic*> keyboardOptions;
	for (auto& ki : keyboards) {
		int port = ki.port;
		ButtonGraphic* bg = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, ki.name, [port](Window* window) {
			gPort = port;
			window->popPage();
		}, bc, fg);
		bg->setFontSize(20);

		keyboardOptions.push_back(bg);
		buttonY += buttonHeight + buttonGap;
	}

	for (auto bg : keyboardOptions) {
		window->addGraphic(bg);
	}

	window->pageLoop();
}

void freePlayPage(Window* window) {
	window->newPage();

	selectKeyboardPage(window);

	pageHeaderGraphics(window);

	Keyboard piano(gPort);
	KeyboardRecorder recorder(&piano, 120);

	AudioPlayer ap;
	ap.start();

	SheetMusicGraphic* smg = new SheetMusicGraphic(0, 0, 1280, 300, &recorder);
	KeyboardGraphic* kg = new KeyboardGraphic(0, 300, 1280, 200);
	TextGraphic* tg = new TextGraphic(0, 550, 1280, 100, "Cmaj9", "fonts/FreeSans.ttf", 50, Color(255, 255, 255));

	int buttonX = 10;
	int buttonY = 510;
	int buttonWidth = 100;
	int buttonHeight = 30;
	int buttonGap = 10;

	ButtonGraphic* butPiano = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Piano", [=](Window* window) {
		activeGen = &pianoGen;
	}, bc, fg);
	butPiano->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butOrgan = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Organ", [=](Window* window) {
		activeGen = &organGen;
	}, bc, fg);
	butOrgan->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butBrass = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Brass", [=](Window* window) {
		activeGen = &brassGen;
	}, bc, fg);
	butBrass->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butReed = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Reed", [=](Window* window) {
		activeGen = &reedGen;
	}, bc, fg);
	butReed->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butPhone = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Phone", [=](Window* window) {
		activeGen = &phoneGen;
	}, bc, fg);
	butPhone->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	ButtonGraphic* butQuit = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Quit", [&recorder, &ap](Window* window) {
		recorder.stop();
		ap.stop();
		window->popPage();
	}, bc, fg);
	butQuit->setFontSize(20);
	buttonX += buttonWidth + buttonGap;

	window->onExit = [&ap, &recorder](Window* window) {
		recorder.stop();
		ap.stop();
		window->popPage();
	};

	recorder.onMessage = [&window](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		window->update();
	};

	recorder.onSoftPedalDown = [&ap, &window](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		ap.stop();
		rcdr->stop();
		window->popPage();
	};

	recorder.onPadDown = [&ap, &window](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		if (msg[1] != 16) return;
		ap.stop();
		rcdr->stop();
		window->popPage();
	};

	recorder.onKeyDown = [&ap, &kg, &tg, &window](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		kg->keys[msg[1] - 12] = 1;
		tg->setText(getChord(rcdr));
		window->update();
		ap.addSample(*activeGen, std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440, rmap(msg[2], 0, 127, 0, 50));
	};

	recorder.onKeyUp = [&ap, &kg, &tg, &window](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		kg->keys[msg[1] - 12] = 0;
		tg->setText(getChord(rcdr));
		window->update();
		ap.removeSample(std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440);
	};

	window->addGraphic(smg);
	window->addGraphic(kg);
	window->addGraphic(tg);
	window->addGraphic(butPiano);
	window->addGraphic(butOrgan);
	window->addGraphic(butBrass);
	window->addGraphic(butReed);
	window->addGraphic(butPhone);
	window->addGraphic(butQuit);

	window->update();

	recorder.record(0);

	window->pageLoop();
}

void pageHeaderGraphics(Window* window) {
	RectangleGraphic* topBar = new RectangleGraphic(0, 0, screenWidth, barHeight, bc);
	ImageGraphic* logo = new ImageGraphic(0, 0, 100, 60, "./logo.png");
	window->addGraphic(topBar);
	window->addGraphic(logo);
}

void settingsPage(Window* window) {
	window->newPage();

	pageHeaderGraphics(window);

	ButtonGraphic* butQuit = new ButtonGraphic(10, barHeight + 10, 100, 50, "Quit", [](Window* window) {
		window->popPage();
	}, bc, fg);
	butQuit->setFontSize(20);

	window->addGraphic(butQuit);

	window->pageLoop();
}

void exitWindow(Window* window) {
	window->close();
}

void mainMenuButtonGraphics(Window* window) {
	int buttonWidth = 300;
	int buttonHeight = 60;
	int buttonX = 10;
	int buttonY = barHeight + 20;

	int fontSize = 34;
	int buttonGap = 10;

	ButtonGraphic* buttonFreePlay = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Free Play", freePlayPage, bc, fg);
	buttonFreePlay->setFontSize(fontSize);
	buttonY += buttonHeight + buttonGap;

	ButtonGraphic* buttonForwarding = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Midi Forwarding", freePlayPage, bc, fg);
	buttonForwarding->setFontSize(fontSize);
	buttonY += buttonHeight + buttonGap;

	ButtonGraphic* buttonSettings = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Settings", settingsPage, bc, fg);
	buttonSettings->setFontSize(fontSize);
	buttonY += buttonHeight + buttonGap;

	ButtonGraphic* buttonExit = new ButtonGraphic(buttonX, buttonY, buttonWidth, buttonHeight, "Exit", exitWindow, bc, fg);
	buttonExit->setFontSize(fontSize);

	window->addGraphic(buttonFreePlay);
	window->addGraphic(buttonForwarding);
	window->addGraphic(buttonSettings);
	window->addGraphic(buttonExit);
}

void mainMenuPage(Window* window) {
	window->newPage();

	pageHeaderGraphics(window);
	mainMenuButtonGraphics(window);

	window->pageLoop();
}

int main(int argc, char** argv) {
	srand(time(0));

	pianoGen.waveform = new PianoWaveform();
	pianoGen.release = new LinearRelease(1200);

	phoneGen.waveform = new WaveformFunction([](double t, double freq) -> double {
		return 0.5 * Waves::multi(t, freq, 0.2, 2.0, 0.4);
	});

	brassGen.waveform = new WaveformFunction([](double t, double freq) -> double {
		return 0.5 * (Waves::pulse(t, freq, 25) + 0.25 * Waves::sawtooth(t, freq) + 0.0001 * Waves::noise());
	});

	reedGen.waveform = new WaveformFunction([](double t, double freq) -> double {
		return 0.5 * (Waves::triangle(t, freq * 2) + 0.5 * Waves::sine(t, freq) + 0.25 * Waves::sawtooth(t, freq) + 0.04 * Waves::noise());
	});

	organGen.waveform = new WaveformFunction([](double t, double freq) -> double {
		return (0.5 * (Waves::sine(t, freq * 1.0) +
					  0.5 * Waves::sine(t, freq * 2.0) +
					  0.25 * Waves::sine(t, freq * 4.0) +
					  0.125 * Waves::sine(t, freq * 8.0)));
	});


	Window window(screenWidth, screenHeight);

	window.setBgColor(RGB_ARGS(bg));

	mainMenuPage(&window);

	return 0;
}
