#pragma once
#include "ofMain.h"
#include "../ofxGuiExtended.h"

struct ofxGuiDefaultConfig{

		static ofJson get(){

			ofJson config = {

				/// settings for all elements
				{
					ofxGuiElement::getClassType(), {
						{"background-color", "rgba(0,0,0,0.2)"},
						{"fill-color", "rgba(200,200,200,0.42)"},
						{"border-width", 1},
						{"padding", 2},
						{"border-color", "rgb(255,255,255)"},
						{"margin", 4},
						{"text-color", "#ffffff"}
					}
				},

				/// settings for all toggles
				{
					ofxGuiToggle::getClassType(), {
					}
				},

				/// settings for all buttons
				{
					ofxGuiButton::getClassType(), {
					}
				},

				/// settings for all sliders
				{
					ofxGuiFloatSlider::getClassType(), {

					}
				},

				/// settings for all rangesliders
				{
					ofxGuiFloatRangeSlider::getClassType(), {

					}
				},

				/// settings for all labels
				{
					ofxGuiLabel::getClassType(), {
						{"border-width", 0},
						{"background-color", "transparent"}
					}
				},

				/// settings for all containers
				{
					ofxGuiContainer::getClassType(), {
						{"border-color", "rgba(255,255,255,0.7)"},
						{"padding", 0},
						{"border-width", 0}
					}
				},

				/// settings for all groups
				{
					ofxGuiGroup::getClassType(), {
					}
				},

				/// settings for all group headers
				{
					ofxGuiGroupHeader::getClassType(), {
						{"width", "100%"},
						{"align-self", "stretch"},
						{"flex","none"},
						{"margin", 0},
						{"border-width", 0},
						{"padding", 0},
						{"text-color", "#ffffff"}
					}
				},

				/// settings for all panels
				{
					ofxGuiPanel::getClassType(), {
					}
				},

				/// settings for all panel headers
				{
					ofxGuiPanelHeader::getClassType(), {
					}

				},

				/// settings for all tabbed groups
				{
					ofxGuiTabs::getClassType(), {
						{"show-header",false},
						{"margin-top", 10}
					}
				},

				/// settings for all value plotters
				{
					ofxGuiValuePlotter::getClassType(), {

					}
				},

				/// settings for all input fields
				{
					ofxGuiIntInputField::getClassType(), {

					}
				}

			};
			return config;
		}

};
