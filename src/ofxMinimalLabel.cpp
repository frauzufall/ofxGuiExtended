#include "ofxMinimalLabel.h"
using namespace std;

void ofxMinimalLabel::generateDraw(){
	bg.clear();

	bg.setFillColor(thisBackgroundColor);
	bg.setFilled(true);
	bg.rectangle(b);

    textMesh = getTextMesh((string)label, b.x + textPadding, b.y + b.height / 2 + 4);
}
