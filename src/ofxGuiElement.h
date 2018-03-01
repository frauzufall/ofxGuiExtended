#pragma once

#include "ofConstants.h"
#include "ofParameter.h"
#include "ofTrueTypeFont.h"
#include "ofBitmapFont.h"
#include "ofJson.h"

#include "DOM/Element.h"

enum TextAlignment{
	LEFT,
	RIGHT,
	CENTERED
};

class ofxGuiElement : public DOM::Element {
	public:

		ofxGuiElement();
		ofxGuiElement(const ofJson & config);

		void setup();

		virtual ~ofxGuiElement();
		ofxGuiElement(const ofxGuiElement &) = delete;
		ofxGuiElement & operator=(const ofxGuiElement &) = delete;

		void setConfig(const ofJson &config, bool recursive = false);
		void setTheme();
		void setTheme(const ofJson &config);
		void loadConfig(const std::string &filename, bool recursive = false);
		void loadTheme(const std::string &filename, bool updateOnFileChange = false);

		void saveToFile(const std::string& filename);
		void loadFromFile(const std::string& filename);

		template<class T>
		void saveTo(T & serializer){
			ofSerialize(serializer, getParameter());
		}

		template<class T>
		void loadFrom(T & serializer){
			ofDeserialize(serializer, getParameter());
		}

		virtual std::string getName();
		virtual void setName(const std::string& name);

		virtual void setTextAlignment(const TextAlignment& textLayout=TextAlignment::LEFT);
		virtual void setTextAlignment(const std::string& textLayout);
		TextAlignment getTextAlignment() const;

		virtual void setLayoutPosition(DOM::LayoutPosition type);
		virtual DOM::LayoutPosition getLayoutPosition();

		ofColor getHeaderBackgroundColor() const;
		ofColor getBackgroundColor() const;
		ofColor getBorderColor() const;
		ofColor getTextColor() const;
		ofColor getFillColor() const;
		bool getShowName() const;
		float getBorderWidth() const;
		float getFontSize() const;

		virtual void setBackgroundColor(const ofColor & color);
		virtual void setBorderColor(const ofColor & color);
		virtual void setTextColor(const ofColor & color);
		virtual void setFillColor(const ofColor & color);
		virtual void setBorderWidth(float width);
		virtual void setFontSize(float size);

		void setShowName(bool show);

		virtual ofAbstractParameter & getParameter();
		void loadFont(const std::string& filename, int fontsize, bool _bAntiAliased = true, bool _bFullCharacterSet = false, int dpi = 0);
		void setUseTTF(bool bUseTTF);

		/// \returns true if the mouse is over this element.
		bool isMouseOver() const;

		/// \returns true if the mouse is pressed on this element.
		bool isMousePressed() const;

		/// \brief Set draggability for this element.
		/// \param draggable True if draggability is enabled.
		void setDraggable(bool draggable);

		/// \brief Determine if draggability is enabled for this element.
		/// \returns true if the draggability is enabled.
		bool isDraggable() const;

		/// \brief Determine if this element is being dragged.
		/// \returns true if this element is being dragged.
		bool isDragging() const;

		void registerMouseEvents(int priority = OF_EVENT_ORDER_BEFORE_APP);
		void unregisterMouseEvents(int priority = OF_EVENT_ORDER_BEFORE_APP);

		virtual bool mouseMoved(ofMouseEventArgs & args);
		virtual bool mousePressed(ofMouseEventArgs & args);
		virtual bool mouseDragged(ofMouseEventArgs & args);
		virtual bool mouseReleased(ofMouseEventArgs & args);
		virtual bool mouseScrolled(ofMouseEventArgs & args){return false;}
		virtual void mouseEntered(ofMouseEventArgs & args){}
		virtual void mouseExited(ofMouseEventArgs & args){}

		ofJson getTheme();
		ofJson getGlobalConfigTheme();

		static std::string getClassType();

		int getObjectCount();

	protected:

		int objCount;

		virtual std::vector<std::string> getClassTypes();

		virtual void generateDraw();
		virtual void render();

		virtual void _setConfig(const ofJson & config);
		void _setConfigUsingClassifiers(const ofJson &config, bool recursive = false);

		void copyLayoutFromDocument();

		/// \brief Sets the value of the element based on a position
		/// \param mx The horizontal position
		/// \param my The vertical position
		/// \param boundaryCheck If true, it checks whether the position is inside of the element. If not, the value won't be changed.
		virtual bool setValue(float mx, float my, bool boundaryCheck){return false;}
		void bindFontTexture();
		void unbindFontTexture();
		ofMesh getTextMesh(const std::string & text, ofPoint p);
		ofMesh getTextMesh(const std::string & text, float x, float y);
		ofRectangle getTextBoundingBox(const std::string & text, float x=0, float y=0);
		float getTextWidth(const std::string & text);
		float getTextHeight(const std::string & text);

		static std::string saveStencilToHex(const ofImage & img);
		static void loadStencilFromHex(ofImage & img, unsigned char * data);

		ofTrueTypeFont font;
		bool fontLoaded;
		bool useTTF;
		ofBitmapFont bitmapFont;

		/// \brief True if the Widget is configured to be dragged.
		bool _isDraggable = false;

		/// \brief True if the widget is currently being dragged.
		bool _isDragging = false;

		/// \brief True if the pointer is over the widget.
		bool _isMouseOver = false;

		/// \brief Point where element is grabbed for dragging in screen coordinates
		ofPoint grabPoint;

		ofPath bg, border;

		ofParameter<ofColor> headerBackgroundColor;
		ofParameter<ofColor> backgroundColor;
		ofParameter<ofColor> borderColor;
		ofParameter<ofColor> textColor;
		ofParameter<ofColor> fillColor;
		ofParameter<float> borderWidth;
		ofParameter<TextAlignment> textAlignment;
		ofParameter<bool> showName;
		ofParameter<float> fontSize;
		ofParameter<float> textPadding;
		ofParameter<float> borderRadius;

		bool bRegisteredForMouseEvents;

		ofJson theme, individualConfig;
		std::string themeFilename;
		bool updateOnThemeChange;
		void watchTheme(ofEventArgs& args);
		std::time_t themeUpdated;

		ofParameter<void> parameter;

		bool themeLoading = false;

};

