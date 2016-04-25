#pragma once

#include "ofx/DOM/Layout.h"
#include "ofParameter.h"
#include "Element.h"

namespace ofx {
namespace DOM {

/// \brief Flex direction options
enum class FlexDirection {
	/// \brief Align items horizontally from left to right (default)
	ROW,
	/// \brief Align items vertically from top to bottom
	COLUMN
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

/// \brief Individual alignment options for flex items
enum class AlignSelf {
	/// \brief Use AlignItems value of parent
	AUTO,
	/// \brief Item is aligned at the beginning
	FLEX_START,
	/// \brief Item is aligned at the end
	FLEX_END,
	/// \brief Items are stacked in the center
	CENTER,
	/// \brief Distribute item rows / columns with equal spacing between them, first and last ones without spacing to the edges
	STRETCH
};

class FlexBoxLayout: public BoxLayout
{
public:
	/// If the Orientation::DEFAULT is chosen, the default will be set to
	/// Orientation::HORIZONTAL.
	FlexBoxLayout(Element* parent, Orientation orientation = Orientation::VERTICAL);

	virtual ~FlexBoxLayout();

	virtual void doLayout() override;

protected:

	void align(FlexDirection direction);

	bool elementFlexing(Element* e);
	bool elementAbsolutePositioned(Element* e);

	FlexDirection getFlexDirection(Element* e, FlexDirection defaultVal = FlexDirection::ROW);
	JustifyContent getJustifyContent(Element* e, JustifyContent defaultVal = JustifyContent::FLEX_START);
	AlignItems getAlignItems(Element* e, AlignItems defaultVal = AlignItems::STRETCH);
	AlignContent getAlignContent(Element* e, AlignContent defaultVal = AlignContent::STRETCH);
	AlignSelf getAlignSelf(Element* e, AlignSelf defaultVal = AlignSelf::AUTO);

	float getWidthAndMargin(Element* e);
	float getHeightAndMargin(Element* e);
	float getDesiredWidth(Element* e);
	float getDesiredHeight(Element* e);

	void setWidthInLayout(Element* e, float width);
	void setHeightInLayout(Element* e, float height);

	float getMarginHorizontal(Element* e);
	float getMarginVertical(Element* e);
	float getMarginLeft(Element* e);
	float getMarginRight(Element* e);
	float getMarginTop(Element* e);
	float getMarginBottom(Element* e);

	float getPaddingHorizontal(Element* e);
	float getPaddingVertical(Element* e);
	float getPaddingLeft(Element* e);
	float getPaddingRight(Element* e);
	float getPaddingTop(Element* e);
	float getPaddingBottom(Element* e);


	void setPosition(Element* e, ofPoint p);

};

}}
