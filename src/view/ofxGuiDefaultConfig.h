#pragma once
#include "ofMain.h"
#include "ofxGuiExtended.h"

struct ofxGuiDefaultConfig{

		static ofJson get(){

			ofJson config = {

				/// settings for all elements
				{
					ofxGuiElement::getClassType(), {
						{"background-color", "rgba(0,0,0,0)"},
						{"fill-color", "rgba(255,255,255,0.5)"},
						{"border-width", 5},
//						{"padding", 5},
						{"border-color", "rgb(255,255,255)"},
						{"margin", 5},
						{"text-color", "#ffffff"}
					}
				},

				/// settings for all toggles
				{
					ofxGuiToggle::getClassType(), {
						{"border-color", "rgba(255,255,255,0.42)"},
					}
				},

				/// settings for all buttons
				{
					ofxGuiButton::getClassType(), {
						{"type", "fullsize"},
						{"text-align", "center"},
						{"border-color", "rgba(255,255,255,0.42)"},
					}
				},

				/// settings for all sliders
				{
					ofxGuiFloatSlider::getClassType(), {

					}
				},

				/// settings for all labels
				{
					ofxGuiLabel::getClassType(), {
						{"border-width", 0}
					}
				},

				/// settings for all groups
				{
					ofxGuiGroup::getClassType(), {
						//{"background-color", "rgba(0,0,0,0.3)"},
//						{"border-width", 6},
						{"border-color", "rgba(255,255,255,0.7)"},
//						{"margin", 10}
					}
				},

				/// settings for all group headers
				{
					ofxGuiGroupHeader::getClassType(), {
						{"align-self", "flex-start"},
						{"flex","none"},
						{"width", "100%"},
						{"margin", 0},
						{"border-width", 0},
						{"background-color", "#ffffff"},
						{"text-color", "#000000"}
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
