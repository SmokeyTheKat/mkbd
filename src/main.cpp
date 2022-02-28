#include <mkbd/keyboard.hpp>
#include <mkbd/recorder.hpp>
#include <mkbd/player.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/music.hpp>
#include <mkbd/math.hpp>
#include <mkbd/window.hpp>
#include <mkbd/audioplayer.hpp>

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <unistd.h>

static std::string getChord(KeyboardRecorder* rcdr) {
	std::vector<int> notes;
	for (int i = 0; i < 256; i++) {
		if (rcdr->getKeyboard()->getKeyState(i))
			notes.push_back(i);
	}
	return getChordName(notes);
}

static int chooseKeyboard(void) {
	std::vector<KeyboardInfo> keyboards = Keyboard::getKeyboards();

	if (keyboards.size() == 0)
		return -1;

	std::cout << "Open MIDI ports:\n";
	for (KeyboardInfo kbd : keyboards) {
		std::cout << "Port " << kbd.port << " \"" << kbd.name << "\"\n";
	}
	std::cout << "Enter port number (0-" << keyboards.size() << ")\n";

	unsigned int port;
	std::cin >> port;

	if (port >= keyboards.size())
		return chooseKeyboard();

	return port;
}

static int chooseBpm(const Keyboard& kbd) {
	std::cout << "Enter bmp (0 for record)\n";
	int bpm;
	std::cin >> bpm;
	if (bpm != 0)
		return bpm;

	Timer timer;
	int beats = 0;
	bool firstKeyPressed = false;

	while (!firstKeyPressed || timer.getLap() < 2.0) {
		KeyboardMessage msg = kbd.getMessage();

		if (msg.getType() == KBD_MSG_KEY) {
			if (msg[2] == 0) continue;
			if (!firstKeyPressed) timer.start();
			firstKeyPressed = true;
			timer.lap();
			beats++;
		}

		usleep(100);
	}

	return (double)beats / timer.stop() * 60.0;
}

static void drawLine(int width, const char* style) {
	std::cout << style;
	for (int i = 0; i < width; i++)
		std::cout << " ";
	std::cout << "\n";
}

static void drawSheet(KeyboardRecorder* rcdr) {
//    std::cout << "\x1b[2J\x1b[1;1H\n\n";
//    std::cout << "\x1b[35;1H" << getChord(rcdr) << "\n";
//    return;
	std::cout << "\x1b[2J\x1b[1;1H\n\n";

	int width = 170;
	int height = 27;

	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");

	drawLine(width, "\x1b[0m");

	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");
	drawLine(width, "\x1b[9m");
	drawLine(width, "\x1b[0m");

	std::cout << "\x1b[1;" << (int)((width / 4) * 1) << "H";
	for (int y = 0; y < height; y++)
		std::cout << "│" << "\x1b[B\x1b[D";

	std::cout << "\x1b[1;" << (int)((width / 4) * 2) + 1 << "H";
	for (int y = 0; y < height; y++)
		std::cout << "│" << "\x1b[B\x1b[D";

	std::cout << "\x1b[1;" << (int)((width / 4) * 3) + 1 << "H";
	for (int y = 0; y < height; y++)
		std::cout << "│" << "\x1b[B\x1b[D";

	std::cout << "\x1b[1;" << (int)((width / 4) * 4) + 1 << "H";
	for (int y = 0; y < height; y++)
		std::cout << "│" << "\x1b[B\x1b[D";

	std::vector<Key>& keys = rcdr->getNotes();

	double barLength = 60.0 / (double)rcdr->getBpm() * 4.0;
	double fullLength = barLength * 4;
	double startTime = std::floor(rcdr->getTime() / fullLength) * fullLength;
//    double startTime = keys.size() > 0
//                            ? std::floor(keys.front().time / fullLength) * fullLength
//                            : std::floor(rcdr->getTime() / fullLength) * fullLength;

	double now = roundTo(rcdr->getTime() - startTime, barLength / 4.0);

	std::cout << "\x1b[38;2;255;255;0m" << "\x1b[1;" << (int)rmap(now, 0, fullLength, 0, width) << "H";
	for (int y = 0; y < height; y++)
		std::cout << "│" << "\x1b[B\x1b[D";

	std::cout << "\x1b[0m\n";

	if (keys.size() == 0) return;
//    return;

	for (int i = 0; i < keys.size(); i++) {
		Key key = keys[i];
		if (key.time < startTime) continue;
		double time = roundTo(key.time - startTime, barLength / 4.0 / 4.0);
		time = key.time - startTime;
		if (time > fullLength) {
//            keys.erase(keys.begin(), keys.begin() + i);
//            drawSheet(rcdr);
			break;
		}
		if (key.id % 2 == 0) std::cout << "\x1b[9m";
		else std::cout << "\x1b[0m";
		std::cout << "\x1b[" << 42 - key.id << ";"
				  << (int)rmap(time, 0, fullLength, 0, width) - (key.flat ? 0 : 0)
				  << (key.flat ? "HO" : "H♭O");
	}

	std::cout << "\x1b[35;1H" << getChord(rcdr) << "\n";
}

int main(int argc, char** argv) {
	srand(time(0));
//    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
//    MainWindow mainWindow;
//    return app->run(mainWindow);
//
//    return 0;

	Keyboard piano(chooseKeyboard());

	int bpm = 60;

	bpm = chooseBpm(piano);

	AudioPlayer ap;
	ap.start();

	KeyboardRecorder recorder(&piano, bpm);

	recorder.onBeat = [](KeyboardRecorder* rcdr) {
//        drawSheet(rcdr);
	};

	recorder.onSoftPedalDown = [&ap](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		ap.stop();
		rcdr->stop();
	};

	recorder.onKeyDown = [&ap](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		ap.addSample(std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440, rmap(msg[2], 0, 127, 0, 10));
//        std::cout << std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440 << " " << rmap(msg[2], 0, 127, 0, 4) << "\n";
//        drawSheet(rcdr);
	};

	recorder.onKeyUp = [&ap](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		ap.removeSample(std::pow(std::pow(2.0, 1.0/12.0), msg[1] - 69) * 440);
	};

	recorder.onMiddlePedalDown = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		drawSheet(rcdr);
		static int isRecording = false;
		isRecording = !isRecording;

		if (isRecording) {
			rcdr->clear();
			return;
		}

		std::vector<KeyboardMessage> rcdrMsgs = rcdr->getMessages();
		std::vector<KeyboardMessage> msgs = rcdrMsgs;

		double barLength = 60.0 / (double)rcdr->getBpm() * 4.0;
		double fullLength = barLength * 4;
		double firstKeyTime = 0;

		for (auto m : msgs) {
			if (m.getType() == KBD_MSG_KEY) {
				firstKeyTime = m.stamp;
				break;
			}
		}

		double startTime = std::floor(firstKeyTime / fullLength) * fullLength;

		for (auto m : msgs) {
			if (m.stamp < startTime) continue;
			if (m.data.size() >= 3) {
				rcdr->addTimedCallback([m](KeyboardRecorder* rcdr) {
					rcdr->getKeyboard()->sendMessage(m);
				}, m.stamp - startTime, fullLength);
			}
		}
	};

	std::vector<Key> notes = recorder.record(0);
	
	return 0;
}
