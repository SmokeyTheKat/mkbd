#include <mkbd/keyboard.hpp>
#include <mkbd/app.hpp>
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
#include <mkbd/gui/graphics/waterfall.hpp>
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

static bool sustain = false;

static Color fg = Color(186, 238, 255);
static Color bg = Color(176, 205, 219);
static Color bc = Color(8, 119, 188);
static const Color White(255, 255, 255);
static const Color Black(0, 0, 0);


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

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_AUDIO);
	srand(time(0));

	App app(argc, argv);
	return app.main();
}
