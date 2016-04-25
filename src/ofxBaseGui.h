#pragma once

#include "ofConstants.h"
#include "ofBaseTypes.h"
#include "ofParameter.h"
#include "ofTrueTypeFont.h"
#include "ofBitmapFont.h"

#include "ofx/DOM/Element.h"

using namespace ofx;
using namespace ofx::DOM;

enum TextAlignment{
	LEFT,
	RIGHT,
	CENTERED
};

class ofxBaseGui : public ofx::DOM::Element {
	public:

		ofxBaseGui();
		ofxBaseGui(const ofJson & config);

		void setup();

		virtual ~ofxBaseGui();
		ofxBaseGui(const ofxBaseGui &) = delete;
		ofxBaseGui & operator=(const ofxBaseGui &) = delete;

		void setConfig(const ofJson &config, bool recursive = false);
		void setTheme(const ofJson &config = defaultTheme);
		void loadConfig(const std::string &filename, bool recursive = false);
		void loadTheme(const std::string &filename);

		void saveToFile(const std::string& filename);
		void loadFromFile(const std::string& filename);

		void setDefaultSerializer(std::shared_ptr <ofBaseFileSerializer> serializer);

		virtual void saveTo(ofBaseSerializer & serializer);
		virtual void loadFrom(ofBaseSerializer & serializer);

		virtual std::string getName();
		virtual void setName(const std::string& name);

		virtual void setTextAlignment(const TextAlignment& textLayout=TextAlignment::LEFT);
		virtual void setTextAlignment(const std::string& textLayout);
		TextAlignment getTextAlignment() const;

		virtual void setLayoutPosition(LayoutPosition type);
		virtual LayoutPosition getLayoutPosition();

		ofColor getHeaderBackgroundColor() const;
		ofColor getBackgroundColor() const;
		ofColor getBorderColor() const;
		ofColor getTextColor() const;
		ofColor getFillColor() const;
		bool getShowName() const;
		float getBorderWidth() const;
		float getFontSize() const;

		virtual void setHeaderBackgroundColor(const ofColor & color);
		virtual void setBackgroundColor(const ofColor & color);
		virtual void setBorderColor(const ofColor & color);
		virtual void setTextColor(const ofColor & color);
		virtual void setFillColor(const ofColor & color);
		virtual void setBorderWidth(float width);
		virtual void setFontSize(float size);

		static void setDefaultHeaderBackgroundColor(const ofColor & color);
		static void setDefaultBackgroundColor(const ofColor & color);
		static void setDefaultBorderColor(const ofColor & color);
		static void setDefaultTextColor(const ofColor & color);
		static void setDefaultFillColor(const ofColor & color);
		static void setDefaultBorderWidth(float width);
		static void setDefaultFontSize(float size);

		static void setDefaultTextPadding(int padding);
		static void setDefaultWidth(int width);
		static void setDefaultHeight(int height);

		static void setDefaultTheme(const ofJson& theme);
		static void loadDefaultTheme(const std::string &filename);

		void setShowName(bool show);

		virtual ofAbstractParameter & getParameter();
		static void loadFont(const std::string& filename, int fontsize, bool _bAntiAliased = true, bool _bFullCharacterSet = false, int dpi = 0);
		static void setUseTTF(bool bUseTTF);

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

		ofJson getThisConfigTheme();
		ofJson getGlobalConfigTheme();

		static std::string getClassType();

	protected:

		virtual vector<std::string> getClassTypes();

		virtual void generateDraw();
		virtual void render();

		virtual void _setConfig(const ofJson & config);
		void _setConfigUsingClassifiers(const ofJson &config, bool recursive = false);

		void onResize(ResizeEventArgs& args);

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

		static ofTrueTypeFont font;
		static bool fontLoaded;
		static bool useTTF;
		static ofBitmapFont bitmapFont;
		std::shared_ptr <ofBaseFileSerializer> serializer;

		static ofColor defaultHeaderBackgroundColor;
		static ofColor defaultBackgroundColor;
		static ofColor defaultBorderColor;
		static ofColor defaultTextColor;
		static ofColor defaultFillColor;
		static float defaultBorderWidth;
		static float defaultFontSize;

		static int textPadding;

		static int defaultWidth;
		static int defaultHeight;

		static ofJson defaultTheme;

		static std::string saveStencilToHex(const ofImage & img);
		static void loadStencilFromHex(ofImage & img, unsigned char * data);

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

		bool bRegisteredForMouseEvents;

		ofJson theme;

		ofParameter<void> parameter;

};
