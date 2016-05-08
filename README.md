# ofxGuiExtended
This is a gui addon displaying `ofParameter` based data with a focus on simplicity and extensibility. It can be configured using JSON.

## Versions
This addon was first built as an extension for the OF core ofxGui addon. You can download this version [here](https://github.com/frauzufall/ofxGuiExtended/releases/tag/v0.1).

This is the standalone ofxGuiExtended version build from ofxGui. It also contains a minimized version of [ofxDOM](https://github.com/bakercp/ofxDOM). It does not depend on other addons.

There is a branch that lets you download or clone a [minimal version of ofxGuiExtended](). It contains only the basic controls and containers and the simplest layout.

## Extensions
This addon is built with the best intentions to be as extensible as possible. There are addons working with ofxGuiExtended:
 - [ofxSortableList](https://github.com/frauzufall/ofxSortableList) *compatible version is not online yet*
 - [ofx2DMapping](https://github.com/frauzufall/ofx2DMapping) *compatible version is not online yet*
 - [ofxMasterSlaveControl](https://github.com/frauzufall/ofxMasterSlaveControl)

Please tell me if you wrote something compatible, I will add it to the list. Check out the bottom of this page for contribution and implementation notes.

## Usage

### Basics
Just initialize some parameters and pass them on to the addon. It will create a panel showing the parameters. Have a look at [this example]() to see it in action.
```c++
//ofApp.h

#include "ofxGuiExtended"

//..
ofxGui gui;

ofParameter<bool> moving;
ofParameter<float> speed;
ofParameter<float> rotation;
//..
```
```c++
//ofApp.cpp

void setup(){

  // initialize the parameters you want to work with
  moving.set("moving", true);
  speed.set("speed", 0.5, 0, 1);
  rotation.set("rotation", 0, 0, 360);

  // add them to the gui
  gui.add(moving, speed, rotation);
}
```

### Using containers
You can also create containers yourself. This is the equivalent to the above shortcut:
```c++
ofxGuiPanel* panel = gui.addPanel();
panel->add(moving);
panel->add(speed);
panel->add(rotation);
```
Some parameters automatically create containers if you add them to the gui:

```c++

ofParameter<ofPoint> position;
ofParameter<ofColor> color;

gui.add(position, color);

```

### Setting attributes
There are attributes that you can set for each element. You will find these attributes in the description of the classes beneath the examples.
#### Styling individual items via ofJson
When adding parameters or creating containers, you can append attributes with `ofJson`.
```c++
ofJson panelConfig = {{"background-color", "#ff0000"}, {"padding", 10}};
ofJson itemConfig = {{"fill-color", "rgba(0,255,255,0.5)"}};

ofxGuiPanel* panel = gui.addPanel("panel title", panelConfig);
panel->add(moving, itemConfig);
panel->add(speed, itemConfig);
panel->add(rotation, itemConfig);
```

#### Styling individual items via JSON file
We can also set attributes in an external file to set style and layout without recompiling.

//TODO

#### Styling items by type via Theme
In a theme you can define attributes for the different element class types.

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
        <td>If true, the slider will trigger on release only.</td>
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
- Derived from `DOM::Layout`.
- Items simply get aligned next to each other vertically.
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
        <td>`float`</td>
        <td>`800`</td>
        <td>Size of the element.</td>
    </tr>
</table>


### `ofxDOMFlexBoxLayout`
- Derived from `ofxDOMBoxLayout`.
- Implements parts of the [CSS FlexBox Layout](https://www.w3.org/TR/css-flexbox).
- There are multiple guides online that describe the layout and its options [[1]](https://scotch.io/tutorials/a-visual-guide-to-css3-flexbox-properties), [[2]](https://css-tricks.com/snippets/css/a-guide-to-flexbox/).
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
      <td>`width`<br>`height`</td>
      <td>`float`<br>`string`</td>
      <td>`800`<br>`"70%"`</td>
      <td>Size of the element.</td>
  </tr>
  <tr>
      <td>`flex`</td>
      <td>`string`<br>`float`</td>
      <td>`"none"` (default)<br>`"auto"`<br>`3`</td>
      <td>Flex attribute of the element [[source](https://www.w3.org/TR/css-flexbox/#flex-property)].</td>
  </tr>
  <tr>
      <td>`flex-direction`</td>
      <td>`string`</td>
      <td>`"column"` (default)<br>`"row"`</td>
      <td>The direction child elements get aligned.</td>
  </tr>
  <tr>
      <td>`flex-wrap`</td>
      <td>`string`</td>
      <td>`"nowrap"` (default)<br>`"wrap"`</td>
      <td>Determines if the child elements are aligned on one or multiple lines if space is insufficient
  </tr>
  <tr>
      <td>`justify-content`</td>
      <td>`string`</td>
      <td>`"flex-start"` (default)<br>`"flex-end"`<br>`"center"`<br>`"space-between"`<br>`"space-around"`</td>
      <td>How to align the child items along the main axis.</td>
  </tr>
  <tr>
      <td>`align-items`</td>
      <td>`string`</td>
      <td>`"stretch"` (default)<br>`"flex-start"`<br>`"flex-end"`<br>`"center"`</td>
      <td>How to align child items along the cross axis.</td>
  </tr>
  <tr>
      <td>`align-content`</td>
      <td>`string`</td>
      <td>`"stretch"` (default)<br>`"flex-start"`<br>`"flex-end"`<br>`"center"`<br>`"space-between"`<br>`"space-around"`</td>
      <td>Alignment options for item lines when there is space in the container along the cross axis, only used if multiple rows / columns are present.</td>
  </tr>
  <tr>
      <td>`align-self`</td>
      <td>`string`</td>
      <td>`"auto"` (default)<br>`"flex-start"`<br>`"flex-end"`<br>`"center"`<br>`"stretch"`</td>
      <td>Individual alignment options for a flex item along the cross axis.</td>
  </tr>
</table>

### `ofxDOMFloatingBoxLayout`
- Derived from `ofxDOMBoxLayout`.
- This layout was fine but when I started using the FlexBoxLayout this one got abandoned and it's probably not working anymore. If someone wants to use it, write me, I might fix it.

## Writing custom gui classes
- //TODO

### Add custom elements via template function
- //TODO

### Use attributes
- //TODO

### Create custom layout
- //TODO

## Known issues
- ** *Please report issues!* **
- The headers of containers with horizontally aligned children (`flex-direction: row` in FlexBoxLayout and `direction: horizontal` in BoxLayout) don't get displayed correctly, hide them with `ofJson({{"show-header", false}})`. Until this is fixed, you can use a vertical container with showing header and add the horizontal container with hidden header.
- FlexBoxLayout needs optimization.

## Wish list
- Load attributes by class / element name from file
- Disable controls

## Credits
- @arturoc for his work on [ofxGui](https://github.com/openframeworks/openFrameworks/tree/master/addons/ofxGui/src)
- @bakercp for his work on [ofxDOM](https://github.com/bakercp/ofxDOM)
- @fxlange for his work on [ofxInputField](https://github.com/fx-lange/ofxInputField/)
