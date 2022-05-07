#include <mkbd/pages.hpp>

#include <mkbd/pagegenerics.hpp>

#include <mkbd/gui/graphics.hpp>
#include <mkbd/gui/style.hpp>

static void generateMainSongView(Window& window);
static void generateMainMenuHeader(Window& window);
static void generateMainMenuButtons(Window& window);

void mainMenuPage2(Window& window) {
	window.newPage();

	generateMainMenuHeader(window);
	generateMainMenuButtons(window);
	generateMainSongView(window);
	generateFooter(window);

	window.pageLoop();
}

static void generateMainSongView(Window& window) {
	int textHeight = 20;
	RectangleComponent* background = new RectangleComponent(
		Layout(
				WindowStyle::menuWidth + 20, WindowStyle::headerHeight + textHeight + 20,
				WindowStyle::menuWidth + 40, 2 * WindowStyle::headerHeight + textHeight + 40,
				Layout::FillX | Layout::FillY
		), WindowStyle::fgColor, WindowStyle::borderColor
	);

	TextComponent* text = new TextComponent(
		Layout(
			WindowStyle::menuWidth + 20, WindowStyle::headerHeight + 20 - textHeight / 2,
			0, textHeight
		),
		"Your Songs", RESOURCE_DIR "/fonts/FreeSans.ttf", textHeight, Color(100, 100, 100)
	);

	window.addComponent(text);
	window.addComponent(background);
}

static void generateMainMenuHeader(Window& window) {
	int gap = WindowStyle::headerHeight / 8;

	RectangleComponent* topBar = new RectangleComponent(
		Layout(0, 0, 0, WindowStyle::headerHeight, Layout::FillX),
		WindowStyle::accColor
	);
	ImageComponent* logo = new ImageComponent(
		Layout(10, gap, 100 - gap * 2, WindowStyle::headerHeight - gap * 2),
		RESOURCE_DIR "/images/logo.png"
	);

	window.addComponent(topBar);
	window.addComponent(logo);
}

static void generateMainMenuButtons(Window& window) {
	ButtonListComponent* buttonList = new ButtonListComponent(
		Layout(10, WindowStyle::headerHeight + 20, WindowStyle::menuWidth, 50),
		10,
		{
//            { "Free Play", std::bind(&App::freePlayPage, this) },
//            { "Test", std::bind(&App::testPage, this) },
//            { "Settings", std::bind(&App::settingsPage, this) },
			{ "Free Play", [](){} },
			{ "Test", [](){} },
			{ "Settings", [](){} },
			{ "Exit", [&window](void){
				window.popPage();
			} },
		},
		25, Colors::White, WindowStyle::accColor
	);

	window.addComponent(buttonList);
}
