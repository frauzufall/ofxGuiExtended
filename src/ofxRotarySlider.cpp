#include "ofxRotarySlider.h"
#include "ofGraphics.h"
using namespace std;

template<typename Type>
ofxRotarySlider<Type>::ofxRotarySlider(){
    this->bUpdateOnReleaseOnly = false;
    this->bGuiActive = false;
    this->mouseInside = false;
}

template<typename Type>
ofxRotarySlider<Type>::~ofxRotarySlider(){
    ofxSlider<Type>::~ofxSlider();
}

template<typename Type>
ofxRotarySlider<Type>::ofxRotarySlider(ofParameter<Type> _val, float width, float height){
    ofxSlider<Type>::setup(_val,width,height);
}

template<typename Type>
ofxRotarySlider<Type>* ofxRotarySlider<Type>::setup(ofParameter<Type> _val, float width, float height){
    ofxSlider<Type>::setup(_val,width,height);
    return this;
}

template<typename Type>
ofxRotarySlider<Type>* ofxRotarySlider<Type>::setup(string sliderName, Type _val, Type _min, Type _max, float width, float height){
    ofxSlider<Type>::setup(sliderName,_val,_min,_max,width,height);
    return this;
}

template<typename Type>
void ofxRotarySlider<Type>::generateDraw(){

    this->bg.clear();
    this->bar.clear();

    this->bg.setFillColor(this->thisBackgroundColor);
    this->bg.setFilled(true);
    arcStrip(this->bg,this->b.getCenter(),min(this->b.width, this->b.height)/2, min(this->b.width, this->b.height)/6, 1);

    float val = ofMap(this->value, this->value.getMin(), this->value.getMax(), 0, 1);
    this->bar.setFillColor(this->thisFillColor);
    this->bar.setFilled(true);
    arcStrip(this->bar,this->b.getCenter(),min(this->b.width, this->b.height)/2-1, min(this->b.width, this->b.height)/6+1, val);

    generateText();
}


template<typename Type>
void ofxRotarySlider<Type>::generateText(){
    string valStr = ofToString(this->value);
    this->textMesh = this->getTextMesh(this->getName(), this->b.x + this->textPadding, this->b.y + this->b.height - this->textPadding);
    this->textMesh.append(this->getTextMesh(valStr, this->b.x + this->b.width - this->textPadding - this->getTextBoundingBox(valStr,0,0).width, this->b.y +this->b.height - this->textPadding));
}

template<>
void ofxRotarySlider<unsigned char>::generateText(){
    string valStr = ofToString((int)this->value);
    this->textMesh = this->getTextMesh(this->getName(), this->b.x + this->textPadding, this->b.y + this->b.height - this->textPadding);
    this->textMesh.append(this->getTextMesh(valStr, this->b.x + this->b.width - this->textPadding - this->getTextBoundingBox(valStr,0,0).width, this->b.y + this->b.height - this->textPadding));
}

template<typename Type>
void ofxRotarySlider<Type>::render(){
    ofColor c = ofGetStyle().color;

    this->bg.draw();
    this->bar.draw();

    ofBlendMode blendMode = ofGetStyle().blendingMode;
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableAlphaBlending();
    }
    ofSetColor(this->thisTextColor);

    this->bindFontTexture();
    this->textMesh.draw();
    this->unbindFontTexture();

    ofSetColor(c);
    if(blendMode!=OF_BLENDMODE_ALPHA){
        ofEnableBlendMode(blendMode);
    }
}

/*
 * adapted from ofxUI by Reza Ali (www.syedrezaali.com || syed.reza.ali@gmail.com || @rezaali)
 *
 */
template<typename Type>
void ofxRotarySlider<Type>::arcStrip(ofPath& path, ofPoint center, float outer_radius, float inner_radius, float percent)
{
    float theta = ofMap(percent, 0, 1, 0, 360.0, true);

    {
        float x = sin(-ofDegToRad(0));
        float y = cos(-ofDegToRad(0));
        path.moveTo(center.x+outer_radius*x,center.y+outer_radius*y);
    }

    for(int i = 0; i <= theta; i+=10)
    {
        float x = sin(-ofDegToRad(i));
        float y = cos(-ofDegToRad(i));

        path.lineTo(center.x+outer_radius*x,center.y+outer_radius*y);
    }

    {
        float x = sin(-ofDegToRad(theta));
        float y = cos(-ofDegToRad(theta));
        path.lineTo(center.x+outer_radius*x,center.y+outer_radius*y);
        path.lineTo(center.x+inner_radius*x,center.y+inner_radius*y);
    }

    for(int i = theta; i >= 0; i-=10)
    {
        float x = sin(-ofDegToRad(i));
        float y = cos(-ofDegToRad(i));

        path.lineTo(center.x+inner_radius*x,center.y+inner_radius*y);
    }

    {
        float x = sin(-ofDegToRad(0));
        float y = cos(-ofDegToRad(0));
        path.lineTo(center.x+inner_radius*x,center.y+inner_radius*y);
    }

    path.close();
}

template<typename Type>
bool ofxRotarySlider<Type>::setValue(float mx, float my, bool bCheck){
    if( !this->isGuiDrawing() ){
        this->bGuiActive = false;
        return false;
    }
    if( bCheck ){
        if( this->b.inside(mx, my) ){
            this->bGuiActive = true;
        }else{
            this->bGuiActive = false;
        }
    }
    if( this->bGuiActive ){

        ofPoint mappedHitPoint = ofPoint(mx, my) - this->b.getCenter();

        ofVec2f cVector = -ofPoint(0,this->b.height/2);
        float val = ofMap(cVector.angle(mappedHitPoint), -180, 180, 0, 1.0, true);

        val = MIN(1.0, MAX(0.0, val));

        this->value = ofMap(val, 0, 1, this->value.getMin(), this->value.getMax(), true);
        return true;
    }
    return false;
}

template<typename Type>
void ofxRotarySlider<Type>::valueChanged(Type & value){
    this->setNeedsRedraw();
}

template class ofxRotarySlider<int>;
template class ofxRotarySlider<unsigned int>;
template class ofxRotarySlider<float>;
template class ofxRotarySlider<double>;
template class ofxRotarySlider<signed char>;
template class ofxRotarySlider<unsigned char>;
template class ofxRotarySlider<unsigned short>;
template class ofxRotarySlider<short>;
