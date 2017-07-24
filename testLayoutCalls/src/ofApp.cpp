#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFrameRate(120);


	ofSetFullscreen(true);

	ofxXmlSettings xml;

	std::string path = "ofxGuiDebugLog.txt";

	xml.clear();
	objs.clear();
	objsSorted.clear();
	if(xml.loadFile(path)){
		xml.pushTag("alignments");

		int alignments = xml.getNumTags("alignment");

		for(int i = 0; i < alignments; i++){
			xml.pushTag("alignment", i);
				ofPtr<GuiObj> obj = ofPtr<GuiObj>(new GuiObj());
				obj->name = xml.getValue("name", "");
				obj->shape.width = xml.getValue("width", 0);
				obj->shape.height = xml.getValue("height", 0);
				obj->shape.x = xml.getValue("x", 0);
				obj->shape.y = xml.getValue("y", 0);
				obj->children.clear();
				obj->id = xml.getValue("id", -100);
				int parentId = xml.getValue("parent", -100);
				objs.push_back(obj);

				ofPtr<GuiObj> sortedObj = nullptr;
				for(ofPtr<GuiObj> _obj: objsSorted){
					if(_obj->id == obj->id){
						sortedObj = _obj;
					}
				}
				if(!sortedObj){
					sortedObj = obj;
					objsSorted.push_back(sortedObj);
				}
				for(ofPtr<GuiObj> parent : objsSorted){
					if(parent->id == parentId){
						bool isChild = false;
						for(ofPtr<GuiObj> child : parent->children){
							if(child->id == sortedObj->id){
								isChild = true;
								break;
							}
						}
						if(!isChild){
							parent->children.push_back(sortedObj);
							sortedObj->hasParent = true;
							break;
						}
					}
				}
			xml.popTag();
		}

	}

	process.set("process", 0, 0, 1);
	speed.set("speed", 1, 0.01, 3); // in sec
	gui.add(process, speed);


	// create obj labels

	labels = gui2.addContainer("", ofJson({{"width", "100%"}, {"height", "100%"}}));

	int noparentobjcount = 0;
	int totalLeafs = 0;
	for(ofPtr<GuiObj> obj: objsSorted){
		if(!obj->hasParent){
			noparentobjcount++;
			totalLeafs += totalLeafSize(obj);
		}
	}

	float x = 0;
	for(ofPtr<GuiObj> obj: objsSorted){
		if(!obj->hasParent){
			float childWidth = ofGetWidth() / totalLeafs * totalLeafSize(obj);
			drawObj(labels, obj, childWidth, ofPoint(x, 0));
			x += childWidth;
		}
	}



	lastUpdate = ofGetElapsedTimef();

}

void ofApp::exit(){
}

//--------------------------------------------------------------
void ofApp::update(){

	float time = ofGetElapsedTimef();
	float timediff = time - lastUpdate;

	float totaltime = speed*objs.size();
	if(totaltime > 0){
		process += timediff / totaltime;
	}
	lastUpdate = time;
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);

	int index = objs.size()*process;

	if(index < objs.size()){

		ofPtr<GuiObj> obj = objs.at(index);

		if(activeElement)
			activeElement->setBackgroundColor(ofColor(0,0,0,0));
		activeElement = labels->getControl(obj->id);
		if(activeElement)
			activeElement->setBackgroundColor(ofColor::lightCoral);

//		ofSetColor(255,0,0);
//		ofFill();
//		ofDrawRectangle(obj->shape);
	}

}

void ofApp::drawObj(ofxGuiContainer *container, ofPtr<GuiObj> obj, float width, ofPoint pos){
	ofParameter<std::string> nameLabel;
	container->add(nameLabel.set(ofToString(obj->id), obj->name), ofJson({
																				  {"show-name", true},
																				  {"position", "absolute"},
																				  {"left", pos.x},
																				  {"top", pos.y},
																				  {"width", width}, {"text-color", "#000000"}}));
	int totalLeafs = totalLeafSize(obj);
	float x = 0;
	for(unsigned int i = 0; i < obj->children.size(); i++){
		float childWidth = width / totalLeafs * totalLeafSize(obj->children.at(i));
		drawObj(container, obj->children.at(i), childWidth, ofPoint(x+pos.x, 33+pos.y));
		x += childWidth;
	}
}

int ofApp::totalLeafSize(ofPtr<GuiObj> obj){
	int leafs = 0;
	for(ofPtr<GuiObj> child : obj->children){
		leafs += totalLeafSize(child);
	}
	return max(1, leafs);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
