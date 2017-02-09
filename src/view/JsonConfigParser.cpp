#include "JsonConfigParser.h"
#include <regex>

bool JsonConfigParser::_parse(const ofJson &config, const string &name, bool& val){

	if (config.find(name) != config.end()) {
		ofJson content = config[name];
		if(content.is_boolean()){
			val = content;
			return true;
		}else {
			ofLogError("JsonConfigParser::parse") << "Could not parse " << config << " to boolean.";
		}
	}
	return false;
}

bool JsonConfigParser::_parse(const ofJson &config, const string &name, std::string& val){

	if (config.find(name) != config.end()) {
		ofJson content = config[name];
		if(content.is_string()){
			val = content;
			return true;
		}else {
			ofLogError("JsonConfigParser::parse") << "Could not parse " << config << " to string.";
		}
	}
	return false;
}

//source: https://gist.github.com/olmokramer/82ccce673f86db7cda5e
//complete check: (#(?:[\\da-f]{3}){1,2}|rgb\\((?:\d{1,3},\\s*){2}\\d{1,3}\\)|rgba\\((?:\\d{1,3},\\s*){3}\\d*\\.?\\d+\\)|hsl\\(\\d{1,3}(?:,\\s*\\d{1,3}%){2}\\)|hsla\\(\\d{1,3}(?:,\\s*\\d{1,3}%){2},\\s*\\d*\\.?\\d+\\))
bool JsonConfigParser::_parse(const ofJson &config, const string &name, ofColor& val){

	if (config.find(name) != config.end()) {

		ofJson content = config[name];

		if(content.is_string()){
			std::string s_value = content;

			if(s_value == "transparent"){
				val.set(ofColor(0,0));
				return true;
			}

			//look for colors in hex format

			std::string hexval = s_value;
			std::transform(hexval.begin(), hexval.end(), hexval.begin(), ::tolower);
			vector<string> matches = JsonConfigParser::getMatchedStrings(hexval, "#(?:[\\da-f]{3}){1,2}");
			if(matches.size() > 0){
				int x;
				ofStringReplace(matches[0],"#","");
				std::stringstream ss;
				ss << std::hex << matches[0];
				ss >> x;
				val.set(ofColor::fromHex(x,255));
				return true;

			}

			//look for colors in rgba format
			matches.clear();
			matches = JsonConfigParser::getMatchedStrings(s_value, "rgba\\((?:\\d{1,3},\\s*){3}\\d*\\.?\\d+\\)");
			if(matches.size() > 0){
				vector<string> vals = ofSplitString(ofSplitString(ofSplitString(s_value, "rgba(")[1],")")[0],",");
				ofColor res(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]), ofToFloat(vals[3])*255);
				val.set(res);
				return true;
			}

			//loook for colors in rgb format
			matches.clear();
			matches = JsonConfigParser::getMatchedStrings(s_value, "rgb\\((?:\\d{1,3},\\s*){2}\\d{1,3}\\)");
			if(matches.size() > 0){
				vector<string> vals = ofSplitString(ofSplitString(ofSplitString(s_value, "rgb(")[1],")")[0],",");
				ofColor res(ofToFloat(vals[0]), ofToFloat(vals[1]), ofToFloat(vals[2]));
				val.set(res);
				return true;
			}
		}/*else {
			// look for ofColor format
			if(ofColor* color = dynamic_cast<ofColor>(&content)){
				val.set(color);
				return true;
			}
		}*/


	}
	return false;

}

bool JsonConfigParser::_parse(const ofJson &config, const string &name, ofPoint& val){

	if (config.find("left") != config.end()) {
		ofJson left = config["left"];
		if(left.is_number()){
			val.x = left;
		}
	}
	if (config.find("top") != config.end()) {
		ofJson top = config["top"];
		if(top.is_number()){
			val.y = top;
		}
	}

	return true;
}


bool JsonConfigParser::_parse(const ofJson &config, const string &name, ofRectangle& val){

	if (config.find("left") != config.end()) {
		ofJson left = config["left"];
		if(left.is_number()){
			val.x = left;
		}
	}
	if (config.find("top") != config.end()) {
		ofJson top = config["top"];
		if(top.is_number()){
			val.y = top;
		}
	}
	if (config.find("width") != config.end()) {
		ofJson width = config["width"];
		if(width.is_number()){
			val.width = width;
		}
	}
	if (config.find("height") != config.end()) {
		ofJson height = config["height"];
		if(height.is_number()){
			val.height = height;
		}
	}
	return true;

}

bool JsonConfigParser::_parse(const ofJson &config, const string &name, DOM::LayoutFloat& val){

	if (config.find(name) != config.end()) {
		ofJson content = config[name];
		if(content == "none"){
			val = DOM::LayoutFloat::NONE;
			return true;
		}
		if(content == "left"){
			val = DOM::LayoutFloat::LEFT;
			return true;
		}
		if(content == "right"){
			val = DOM::LayoutFloat::RIGHT;
			return true;
		}
	}
	return false;
}

bool JsonConfigParser::_parse(const ofJson &config, const string &name, DOM::LayoutPosition& val){

	if (config.find(name) != config.end()) {
		ofJson content = config[name];
		if(content == "static"){
			val = DOM::LayoutPosition::POSITION_STATIC;
			return true;
		}
		if(content == "absolute"){
			val = DOM::LayoutPosition::POSITION_ABSOLUTE;
			return true;
		}
	}
	return false;
}

bool JsonConfigParser::parse(const ofJson &config, DOM::Element* val){

	if(!config.is_null()){
		if (config.find("left") != config.end()) {
			ofJson left = config["left"];
			if(left.is_number()){
				val->setPosition(left, val->getPosition().y);
			}
		}
		if (config.find("top") != config.end()) {
			ofJson top = config["top"];
			if(top.is_number()){
				val->setPosition(val->getPosition().x, top);
			}
		}
	}
	return true;

}


vector<string> JsonConfigParser::getMatchedStrings(string contents, string regex){

	vector <string> results;
	std::regex regEx(regex);
	std::smatch match;

	if(std::regex_search(contents, match, regEx)) {
		for (size_t i = 0; i < match.size(); ++i){
			results.push_back(match[i]);
		}
	}

	return results;
}
