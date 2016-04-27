#pragma once

#include "ofxDOMBoxLayout.h"
#include "ofParameter.h"
#include "Element.h"

class ofxDOMFlexBoxLayout: public ofxDOMBoxLayout
{
public:

	/// \brief Flex direction options
	enum class FlexDirection {
		/// \brief Align items horizontally from left to right (default)
		ROW,
		/// \brief Align items vertically from top to bottom
		COLUMN
	};

	/// \brief Flex wrap options
	enum class FlexWrap {
		/// \brief Set items in one single line (default)
		NOWRAP,
		/// \brief Use multiple lines to align items if needed
		WRAP
	};

	/// \brief Align items along the main axis options
	enum class JustifyContent {
		/// \brief Align items at the beginning according to flex direction (default)
		FLEX_START,
		/// \brief Align items at the end according to flex direction
		FLEX_END,
		/// \brief Align items at the center
		CENTER,
		/// \brief Distribute items with equal spacing between them, first and last items without spacing to the edges
		SPACE_BETWEEN,
		/// \brief Distribute items with equal spacing around all items
		SPACE_AROUND
	};

	/// \brief Align items along the cross axis options
	enum class AlignItems {
		/// \brief Items fill the whole height (or width)
		STRETCH,
		/// \brief Align items at the beginning (default)
		FLEX_START,
		/// \brief Align items at the end
		FLEX_END,
		/// \brief Align items at the center
		CENTER
	};

	/// \brief Alignment options for item lines when there is space in the container along the cross axis, only used if multiple rows / columns are present
	enum class AlignContent {
		/// \brief Distributed space after every item row / column
		STRETCH,
		/// \brief Items are stacked towards the beginning
		FLEX_START,
		/// \brief Items are stacked towards the end
		FLEX_END,
		/// \brief Items are stacked in the center
		CENTER,
		/// \brief Distribute item rows / columns with equal spacing between them, first and last ones without spacing to the edges
		SPACE_BETWEEN,
		/// \brief Distribute item rows / columns with equal spacing around all of them
		SPACE_AROUND
	};

	/// \brief Individual alignment options for flex items along the cross axis
	enum class AlignSelf {
		/// \brief Use AlignItems value of parent
		AUTO,
		/// \brief Item is aligned at the beginning
		FLEX_START,
		/// \brief Item is aligned at the end
		FLEX_END,
		/// \brief Item is aligned at the center
		CENTER,
		/// \brief Item is stretched
		STRETCH
	};


	/// If the Orientation::DEFAULT is chosen, the default will be set to
	/// Orientation::HORIZONTAL.
	ofxDOMFlexBoxLayout(DOM::Element* parent, DOM::Orientation orientation = DOM::Orientation::VERTICAL);

	virtual ~ofxDOMFlexBoxLayout();

	virtual void doLayout() override;

protected:

	void align(FlexDirection direction);

	bool elementFlexing(DOM::Element* e);
	bool elementAbsolutePositioned(DOM::Element* e);

	FlexDirection getFlexDirection(DOM::Element* e, FlexDirection defaultVal = FlexDirection::COLUMN);
	FlexWrap getFlexWrap(DOM::Element* e, FlexWrap defaultVal = FlexWrap::NOWRAP);
	JustifyContent getJustifyContent(DOM::Element* e, JustifyContent defaultVal = JustifyContent::FLEX_START);
	AlignItems getAlignItems(DOM::Element* e, AlignItems defaultVal = AlignItems::STRETCH);
	AlignContent getAlignContent(DOM::Element* e, AlignContent defaultVal = AlignContent::STRETCH);
	AlignSelf getAlignSelf(DOM::Element* e, AlignSelf defaultVal = AlignSelf::AUTO);

	float getWidthPlusMargin(DOM::Element* e);
	float getHeightPlusMargin(DOM::Element* e);
	float getDesiredWidth(DOM::Element* e);
	float getDesiredHeight(DOM::Element* e);

	void setLayoutWidthMinusMargin(DOM::Element* e, float width);
	void setLayoutHeightMinusMargin(DOM::Element* e, float height);

	void setWidthInLayoutAddPadding(DOM::Element* e, float width);
	void setHeightInLayoutAddPadding(DOM::Element* e, float height);

	float getMarginHorizontal(DOM::Element* e);
	float getMarginVertical(DOM::Element* e);
	float getMarginLeft(DOM::Element* e);
	float getMarginRight(DOM::Element* e);
	float getMarginTop(DOM::Element* e);
	float getMarginBottom(DOM::Element* e);

	float getPaddingHorizontal(DOM::Element* e);
	float getPaddingVertical(DOM::Element* e);
	float getPaddingLeft(DOM::Element* e);
	float getPaddingRight(DOM::Element* e);
	float getPaddingTop(DOM::Element* e);
	float getPaddingBottom(DOM::Element* e);


	void setPosition(DOM::Element* e, ofPoint p);

};
