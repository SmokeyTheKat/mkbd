#include <mkbd/keyboard.hpp>
#include <mkbd/recorder.hpp>
#include <mkbd/player.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/music.hpp>
#include <mkbd/math.hpp>
#include <mkbd/window.hpp>

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <unistd.h>

static std::string getChord(KeyboardRecorder* rcdr) {
	std::vector<int> notes;
	for (int i = 0; i < 256 && notes.size() < 4; i++) {
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

	std::cout << "\x1b[1;" << (int)((width / 4) * 2) << "H";
	for (int y = 0; y < height; y++)
		std::cout << "│" << "\x1b[B\x1b[D";

	std::cout << "\x1b[1;" << (int)((width / 4) * 3) << "H";
	for (int y = 0; y < height; y++)
		std::cout << "│" << "\x1b[B\x1b[D";

	std::cout << "\x1b[1;" << (int)((width / 4) * 4) << "H";
	for (int y = 0; y < height; y++)
		std::cout << "│" << "\x1b[B\x1b[D";

	std::vector<Key>& keys = rcdr->getNotes();

	int x = 1;

	double barLength = 60.0 / (double)rcdr->getBpm()  * 4.0;
	double fullLength = barLength * 4;

	double startTime = std::floor(keys.front().time / fullLength) * fullLength;

	for (int i = 0; i < keys.size(); i++) {
		Key key = keys[i];
		double time = roundTo(key.time - startTime, barLength / 4.0 / 4.0);
		time = key.time - startTime;
		if (time > fullLength) {
			keys.erase(keys.begin(), keys.begin() + i);
			drawSheet(rcdr);
			return;
		}
		if (key.id % 2 == 0) std::cout << "\x1b[9m";
		else std::cout << "\x1b[0m";
		std::cout << "\x1b[" << 42 - key.id << ";"
				  << (int)rmap(time, 0, fullLength, 0, width) - (key.flat ? 0 : 0)
				  << (key.flat ? "HO" : "H♭O");
		x += 16;
	}

	std::cout << "\x1b[35;1H" << getChord(rcdr) << "\n";
}

int main(int argc, char** argv) {
//    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
//    MainWindow mainWindow;
//    return app->run(mainWindow);
//
//    return 0;

	int port = chooseKeyboard();
	Keyboard piano(port);

		piano.sendMessage(KeyboardMessage(3, KBD_MSG_KEY, 66, 127));

	int bpm = 60;

	bpm = chooseBpm(piano);

	KeyboardRecorder recorder(&piano, bpm);

	recorder.onKeyDown = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        showChord(rcdr);
		drawSheet(rcdr);
	};

	recorder.onKeyUp = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
//        showChord(rcdr);
	};
	recorder.onMiddlePedalDown = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		rcdr->restart();
		drawSheet(rcdr);
	};
	recorder.onSoftPedalDown = [](KeyboardRecorder* rcdr, KeyboardMessage msg) {
		KeyboardPlayer player(rcdr->getKeyboard(), rcdr->getNotes(), rcdr->getBpm());
		player.play(rcdr->getKeyboard());
	};

	std::vector<Key> notes = recorder.record(0);
	
	return 0;
}
