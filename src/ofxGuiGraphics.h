#pragma once

#include "ofxBaseGui.h"

class ofxGuiGraphics : public ofxBaseGui {
	public:

    struct Config: public ofxBaseGui::Config{
            Config(){}
            Config(const std::string & name, ofBaseDraws * graphics)
            :canvasName(name)
            ,graphics(graphics){

            }
            std::string canvasName;
            ofBaseDraws* graphics=nullptr;
        };

        ofxGuiGraphics();
        ofxGuiGraphics(const Config & config);
        virtual ~ofxGuiGraphics();

        ofxGuiGraphics & setup(string canvasName = "", ofBaseDraws * graphics = 0, float w = 0, float h = 0);

		// Abstract methods we must implement, but have no need for!
		virtual bool mouseMoved(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mousePressed(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mouseDragged(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mouseReleased(ofMouseEventArgs & args){
			return false;
		}
		virtual bool mouseScrolled(ofMouseEventArgs & args){
			return false;
		}

        virtual void setSize(float w, float h);
        virtual void setShape(float x, float y, float w, float h);
        virtual void setShape(ofRectangle r);

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
