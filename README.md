# ofxGuiExtended
//TODO general description

## Usage

### Basic use case
Just initialize some parameters and pass them on to the addon. It will create a panel showing the parameters.
```c++
//ofApp.h

#include "ofxGuiExtended"

//..
ofxGui gui;

ofParameter<bool> moving;
ofParameter<float> speed;
ofParameter<ofPoint> direction;
//..
```
```c++
//ofApp.cpp

void setup(){

  //initialize the parameters you want to work with
  moving.set("moving", true);
  speed.set("speed", 0.5, 0, 1);
  direction.set("direction", ofPoint(1,0), ofPoint(0,0), ofPoint(1,1));

  //add them to the gui
  gui.add(moving, speed, direction);
}
```

### Setting attributes
//TODO
#### ofJson
//TODO

#### JSON file
//TODO

#### Theme
//TODO

### Using containers
//TODO

### Using layouts
//TODO

## Controls

### `ofxGuiElement`
  - Derived from `DOM::Element`.
  - Base class of all other gui elements.

  <table>
    <col width="35%" />
    <col width="15%" />
    <col width="25%" />
    <col width="25%" />
    <tr>
        <td>**Attribute**</td>
        <td>**Value**</td>
        <td>**Example**</td>
        <td>**Description**</td>
    </tr>
    <tr>
        <td>`width`<br>`height`</td>
        <td>`float`<br>`string`</td>
        <td>`800`<br>`"70%"`</td>
        <td>Size of the element.</td>
    </tr>
    <tr>
        <td>`show-name`</td>
        <td>`bool`</td>
        <td>`true`</td>
        <td>Display or hide the name of the element.</td>
    </tr>
    <tr>
        <td>`background-color`<br>`border-color`<br>`fill-color`<br>`text-color`<br>`header-background-color`</td>
        <td>`string`</td>
        <td>`"#ff0000"`<br>`"rgb(255,0,0)"`<br>`"rgba(255,0,0,0.5)"`</td>
        <td>Colors of the element.</td>
    </tr>
    <tr>
        <td>`border-width`</td>
        <td>`float`</td>
        <td>`10`</td>
        <td>Width of the border.</td>
    </tr>
    <tr>
        <td>`text-align`</td>
        <td>`string`</td>
        <td>`left`<br>`right`<br>`center`</td>
        <td>Sets the text alignment. *Currently only affecting fullsize toggles.*</td>
    </tr>
</table>


### `ofxGuiToggle`
  - Derived from `ofxGuiElement`.
  - Default control type for `ofParameter<bool>`.

  <table>
    <col width="30%" />
    <col width="15%" />
    <col width="30%" />
    <col width="25%" />
    <tr>
        <td>**Attribute**</td>
        <td>**Value**</td>
        <td>**Example**</td>
        <td>**Description**</td>
    </tr>
    <tr>
        <td>`type`</td>
        <td>`string`</td>
        <td>`"checkbox"` (default)<br>`"radio"`<br>`"fullsize"`</td>
        <td> Visual type of toggle.</td>
    </tr>
  </table>

### `ofxGuiButton`
  - Derived from `ofxGuiToggle`.
  - Default control type for `ofParameter<void>`.

### `ofxGuiSlider`
  - Derived from `ofxGuiElement`.
  - This is a template class, use `ofxGuiFloatSlider` or `ofxGuiIntSlider`.
  - Default control types for `ofParameter<float>` and `ofParameter<int>`.

  <table>
    <col width="30%" />
    <col width="15%" />
    <col width="30%" />
    <col width="25%" />
    <tr>
        <td>**Attribute**</td>
        <td>**Value**</td>
        <td>**Example**</td>
        <td>**Description**</td>
    </tr>
    <tr>
        <td>`type`</td>
        <td>`string`</td>
        <td>`"straight"` (default)<br>`"circular"`</td>
        <td> Visual type of slider. The straight type will appear as a horizontal or vertical slider depending on the aspect ration of its size.</td>
    </tr>
    <tr>
        <td>`precision`</td>
        <td>`int`</td>
        <td>`6` (default)</td>
        <td>Sets the displayed precision of a `float` value. The value of the parameter won't be changed.</td>
    </tr>
    <tr>
        <td>`update-on-release-only`</td>
        <td>`bool`</td>
        <td>`false` (default)</td>
        <td>If true, the button will trigger on release only.</td>
    </tr>

  </table>

### `ofxGuiLabel`
  - Derived from `ofxGuiElement`.
  - //TODO

### `ofxGuiGraphics`
  - Derived from `ofxGuiElement`.
  - Displays any `ofBaseDraws` reference.
  - //TODO

### `ofxGuiZoomableGraphics`
  - Derived from `ofxGuiGraphics`.
  - `ofBaseDraws` reference can be zoomed with scroll wheel and dragged with mouse button.
  - //TODO

### `ofxGuiValuePlotter`
  - Derived from `ofxGuiElement`.
  - //TODO

### `ofxGuiFpsPlotter`
  - Derived from `ofxGuiValuePlotter`.
  - //TODO

### `ofxGuiFunctionPlotter`
  - Derived from `ofxGuiElement`.
  - //TODO


## Containers

### `ofxGuiGroup`
- derived from `ofxGuiElement`.
- Groups other elements according to the current layout
- Attributes:
  - `header-height`: float
- //TODO

### `ofxGuiPanel`
  - derived from `ofxGuiGroup`.
  - Header can be used to drag group (only if panel is in absolute position mode).
  - //TODO

### `ofxGuiTabs`
  - derived from `ofxGuiGroup`.
  - Add groups to this container and they will be displayed as tabs.
  - //TODO


## Layouts

### `ofxDOMBoxLayout`
- derived from `DOM::Layout`.
- //TODO

### `ofxDOMFlexBoxLayout`
- derived from `ofxDOMBoxLayout`.
- //TODO

### `ofxDOMFloatingBoxLayout`
- derived from `ofxDOMBoxLayout`.
- //TODO
