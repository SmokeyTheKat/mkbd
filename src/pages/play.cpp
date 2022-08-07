#include <mkbd/app.hpp>

#include <mkbd/gui/graphics.hpp>

void App::playPage(void) {
	mWindow.newPage();

	DropDownComponent* deviceList = new DropDownComponent(
		Layout(0, 0, 300, 100)
	);

	deviceList->on("DropDown", asFunction([deviceList](void) {
		std::vector<MidiDevice::Info> devices = MidiDevice::getDevices();

		std::vector<std::string> deviceNames;
		for (auto info : devices) deviceNames.push_back(info.name);

		deviceList->setData(deviceNames);
	}));

	mWindow.addComponent(deviceList);

	mWindow.pageLoop();
}
