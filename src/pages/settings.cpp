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

void App::settingsPage(void) {
	mWindow.newPage();

	mSettingsPage.clear();

	generateHeader("Settings");

	RectangleComponent* rect = new RectangleComponent(Layout(0, mHeaderHeight, mMenuWidth, mHeaderHeight - 1, Layout::FillY), gConfig.fgColor, Color(0, 0, 0));

	std::vector<Component*> buttons = createListComponent(
		Layout(0, mHeaderHeight, mMenuWidth, 55),
		{
			[this](Layout layout) {
				return new ButtonComponent(layout, "Midi Device", std::bind(&App::settingsPageTabMidiDevice, this), gConfig.fgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonComponent(layout, "Midi Forwarding", std::bind(&App::settingsPageTabMidiForwarding, this), gConfig.fgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonComponent(layout, "Color Scheme", std::bind(&App::settingsPageTabColorScheme, this), gConfig.fgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonComponent(layout, "Instruments", std::bind(&App::settingsPageInstruments, this), gConfig.fgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonComponent(layout, "Performance", std::bind(&App::settingsPageTabProformance, this), gConfig.fgColor, Colors::Black);
			},
			[this](Layout layout) {
				return new ButtonComponent(layout, "About", std::bind(&App::settingsPageTabAbout, this), gConfig.fgColor, Colors::Black);
			},
		}, -1
	);

	mWindow.addComponent(rect);
	for (auto g : buttons)
		mWindow.addComponent(g);

	mWindow.pageLoop();
}

void App::newSettingsPage(void) {
	for (Component* g : mSettingsPage) {
		mWindow.removeComponent(g);
		delete g;
	}
	mSettingsPage.clear();
}

void App::settingsPageTabMidiDevice(void) {
	newSettingsPage();

	std::vector<ComponentCreater> creaters;

	creaters.push_back([this](Layout layout) -> Component* {
		return new TextComponent(layout, "Input Devices", RESOURCE_DIR "/fonts/FreeSans.ttf", 20, Colors::Black);
	});

	std::vector<MidiDevice::Info> keyboards = MidiDevice::getDevices();
	for (auto& ki : keyboards) {
		int port = ki.port;
		creaters.push_back([this, port, ki](Layout layout) -> Component* {
			layout.width = 0;
			layout.height = 0;
			return new ButtonComponent(layout, ki.name, [this, port](void) {
				mMidiPort = port;
				mWindow.popPage();
				freePlayPage();
			}, Colors::White, Colors::Black);
		});
	}

	creaters.push_back([this](Layout layout) -> Component* {
		return new TextComponent(layout, "Output Devices", RESOURCE_DIR "/fonts/FreeSans.ttf", 20, Colors::Black);
	});
	for (auto& ki : keyboards) {
		int port = ki.port;
		creaters.push_back([this, port, ki](Layout layout) -> Component* {
			return new ButtonComponent(layout, ki.name, [this, port](void) {
				mMidiPort = port;
				mWindow.popPage();
				freePlayPage();
			}, Colors::White, Colors::Black);
		});
	}

	int buttonWidth = 500;
	int buttonHeight = 35;
	int buttonGap = 5;

	std::vector<Component*> buttons = createListComponent(
		Layout(
			mMenuWidth + 20, mHeaderHeight + 20,
			buttonWidth, buttonHeight
		),
//        Layout(
//            buttonWidth / -2, mHeaderHeight + 20,
//            buttonWidth, buttonHeight,
//            Layout::AnchorTopCenter
//        ),
		creaters, 5
	);

	for (auto g : buttons) {
		mWindow.addComponent(g);
		mSettingsPage.push_back(g);
	}
}

void App::settingsPageTabMidiForwarding(void) {
	newSettingsPage();

	RectangleComponent* rect = new RectangleComponent(
		Layout(mMenuWidth + 20, mHeaderHeight + 20, 200, 400),
		Color(255, 255, 0), Color(0, 0, 0)
	);

	RectangleComponent* test = new RectangleComponent(
		Layout(50, 20, 50, 50),
		Color(0, 0, 255), Color(0, 0, 0)
	);
	test->setParent(rect);

	mSettingsPage.push_back(rect);
	mWindow.addComponent(rect);
}

void App::settingsPageTabColorScheme(void) {
}

void App::settingsPageInstruments(void) {
}

void App::settingsPageTabProformance(void) {
	newSettingsPage();

	InputComponent* input = new InputComponent(Layout(mMenuWidth + 20, mHeaderHeight + 20, 200, 40), std::to_string(mAudioPlayer.getSampleSize()), InputType::Number);
	input->setMaxValue(8192);
	input->setMinValue(0);

	input->on("Change", asFunction<std::string>([this](std::string text) {
		if (Utils::stringIsNumber(text)) {
			int value = std::stoi(text);
			if (value < 64) return;
			mAudioPlayer.setSampleSize(value);
			mAudioPlayer.restart();
		}
	}));
	
	mSettingsPage.push_back(input);
	mWindow.addComponent(input);
}
