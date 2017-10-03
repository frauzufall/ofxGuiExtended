#pragma once

#include "../DOM/Layout.h"
#include "ofParameter.h"
#include "Element.h"

class ofxDOMFlexBoxLayout: public DOM::_Layout<ofxDOMFlexBoxLayout>
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
	ofxDOMFlexBoxLayout(DOM::Element* parent);

	virtual ~ofxDOMFlexBoxLayout();

	virtual void doLayout() override;

protected:

	struct FlexBoxObject {
		DOM::Element* origin;
		ofRectangle shape;
		bool sizeSet = false;
		bool flex = false;
		float flexBasis = 0;
	};

	struct FlexBoxLine {
		FlexBoxLine(float initialSpaceMainAxis){
			totalSpaceMainAxis = initialSpaceMainAxis;
		}
		std::vector<FlexBoxObject> items;
		float totalSpaceMainAxis;
		float sizeCrossAxis;
	};

	void align(FlexDirection direction);

	static bool elementFlexing(DOM::Element* e);

	static FlexDirection getFlexDirection(DOM::Element* e, FlexDirection defaultVal = FlexDirection::COLUMN);
	static FlexWrap getFlexWrap(DOM::Element* e, FlexWrap defaultVal = FlexWrap::NOWRAP);
	static JustifyContent getJustifyContent(DOM::Element* e, JustifyContent defaultVal = JustifyContent::FLEX_START);
	static AlignItems getAlignItems(DOM::Element* e, AlignItems defaultVal = AlignItems::STRETCH);
	static AlignContent getAlignContent(DOM::Element* e, AlignContent defaultVal = AlignContent::STRETCH);
	static AlignSelf getAlignSelf(DOM::Element* e, AlignSelf defaultVal = AlignSelf::AUTO);

	static float getWidthPlusMargin(DOM::Element* e);
	static float getHeightPlusMargin(DOM::Element* e);

	static void setLayoutWidthMinusMargin(DOM::Element* e, float width);
	static void setLayoutHeightMinusMargin(DOM::Element* e, float height);

};
