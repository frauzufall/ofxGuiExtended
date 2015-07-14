#include "ofxBaseGui.h"

class ofxCanvas: public ofxBaseGui {
public:
    ofxCanvas();
    ofxCanvas(string canvasName, ofTexture* tex = 0, float w = 0, float h = 0);
    ofxCanvas(ofTexture* tex, float w = defaultWidth, float h = 0);
    virtual ~ofxCanvas();

    ofxCanvas * setup(string canvasName = "", ofTexture* tex = 0, float w = 0, float h = 0);

    // Abstract methods we must implement, but have no need for!
    virtual bool mouseMoved(ofMouseEventArgs & args){return false;}
    virtual bool mousePressed(ofMouseEventArgs & args){return false;}
    virtual bool mouseDragged(ofMouseEventArgs & args){return false;}
    virtual bool mouseReleased(ofMouseEventArgs & args){return false;}
    virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}

    virtual void setSize(float w, float h);

    virtual ofAbstractParameter & getParameter();

protected:

    bool setValue(float mx, float my, bool bCheckBounds){return false;}
    void update();
    void render();
    void generateDraw();
    ofPath bg;
    ofVboMesh textMesh;
    ofTexture* texture;
    ofParameter<string> label;

};
