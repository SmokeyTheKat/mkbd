#include <mkbd/gui/components/seekbar.hpp>

#include <mkbd/math.hpp>

SeekBarComponent::SeekBarComponent(Layout layout, Timer* timer)
: Component(layout), mTimer(timer) {
	mCurrentTimeText = new TextComponent(
		Layout(0, 0, 100, 20, Layout::AnchorBottomLeft),
		"00:00.0",
		RESOURCE_DIR "/fonts/FreeSans.ttf",
		12,
		Colors::White
	);
	mLengthTimeText = new TextComponent(
		Layout(0, 0, 50, 20, Layout::AnchorBottomRight),
		"00:00.0",
		RESOURCE_DIR "/fonts/FreeSans.ttf",
		12,
		Colors::White
	);

	mCurrentTimeText->on("Draw", asFunction([this](void) {
		double time = mTimer->now();
		int lastTime = time * 10;
		if (lastTime != mLastTime) {
			mCurrentTimeText->setText(Utils::timeToString(time));
		}
		mLastTime = lastTime;
	}));

	addChild(mCurrentTimeText);
	addChild(mLengthTimeText);
}


void SeekBarComponent::setLength(double length) {
	mLength = length;
	mLengthTimeText->setText(Utils::timeToString(mLength));
}

void SeekBarComponent::init(void) {
}

void SeekBarComponent::onClick(int button, int x, int y) {
	double time = rmap(x, 0, mWidth, 0, mLength);
	mTimer->set(time);
	emit("Seek");
}

void SeekBarComponent::onDrag(int x, int y) {
	onClick(0, x, y);
}

void SeekBarComponent::draw(void) {
	setColor(30, 30, 30);
	fillRectangle(0, 0, mWidth, mHeight);
	setColor(0, 165, 0);
	fillRectangle(0, 0, rmap(mTimer->now(), 0, mLength, 0, mWidth), mHeight);

	setColor(130, 130, 130);
	int segs = 50;
	double segWidth = (double)mWidth / (double)segs;
	for (int i = 1; i < segs; i++) {
		drawLine(i * segWidth, 0, i * segWidth, mHeight);
	}
}
