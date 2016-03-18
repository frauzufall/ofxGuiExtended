#pragma once

#include "ofxBaseGui.h"

class ofxGuiGraphics : public ofxBaseGui {
	public:

		ofxGuiGraphics(string canvasName="", const ofJson& config = ofJson());
		ofxGuiGraphics(string canvasName, ofBaseDraws * graphics, const ofJson& config = ofJson());
		ofxGuiGraphics(string canvasName, ofBaseDraws * graphics, float w, float h = 0);
		virtual ~ofxGuiGraphics();

		void setup(string canvasName = "", ofBaseDraws * graphics = 0, float w = 0, float h = 0);

		virtual void setWidth(float w) override;
		virtual void setHeight(float h) override;
		virtual void setSize(float w, float h) override;
		virtual void setShape(float x, float y, float w, float h) override;
		virtual void setShape(const ofRectangle &r) override;

		virtual void setGraphics(ofBaseDraws* graphics);

		virtual ofAbstractParameter & getParameter();

	protected:

		bool setValue(float mx, float my, bool bCheckBounds){
			return false;
		}
		virtual void render();
		virtual void generateDraw();
		ofPath bg;
		ofVboMesh textMesh;
		ofBaseDraws * graphics;
		ofParameter <std::string> label;
		bool _bLoaded;

};
