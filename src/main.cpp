#include <mkbd/app.hpp>
#include <mkbd/utils.hpp>

//void looperSetLoop(KeyboardRecorder* rcdr) {
//    static int isRecording = false;
//    isRecording = !isRecording;
//
//    if (isRecording) {
//        rcdr->clear();
//        return;
//    }
//
//    std::vector<KeyboardMessage> msgs = rcdr->getMessages();
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

#include <mkbd/sfz.hpp>
#include <mkbd/arrayview.hpp>
#include <mkbd/filemanager.hpp>

#ifdef _WIN64
//#include <windows/wininfo.h>
int WINAPI WinMain(HINSTANCE _hinstance,
				   HINSTANCE hprev_instance,
				   LPSTR cmd_line,
				   int cmd_show)
{
	int argc = 1;
	char** argv = new char*("omg");
#else
int main(int argc, char** argv)
{
#endif
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Could not initialize everyting lol! Error: " << SDL_GetError() << "\n";
	}

	srand(time(0));

	App app(argc, argv);
	return app.main();
}
