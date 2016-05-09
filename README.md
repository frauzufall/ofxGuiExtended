# ofxGuiExtended
This is a gui addon displaying `ofParameter` based data with a focus on simplicity and extensibility. It can be configured using JSON.

## System requirements
Since ofJson got just recently added to openFrameworks, you have to use the current master branch in order for it to work. I am using commit c9cd3e9 right now.

This addon is tested on Ubuntu 15.10 64bit. Please tell me if it runs on other systems or if you have issues.

## Versions
This addon was first built as an extension for the OF core ofxGui addon. You can download this version [here](https://github.com/frauzufall/ofxGuiExtended/releases/tag/v0.1).

This is the standalone ofxGuiExtended version build from ofxGui. It also contains a minimized version of [ofxDOM](https://github.com/bakercp/ofxDOM). It does not depend on other addons.

There is a branch that lets you download or clone a [minimal version of ofxGuiExtended](https://github.com/frauzufall/ofxGuiExtended/tree/minimal). It contains only the basic controls and containers and the simplest layout.

## Extensions
This addon is built with the best intentions to be as extensible as possible. There are addons working with ofxGuiExtended:
 - [ofxSortableList](https://github.com/frauzufall/ofxSortableList) *compatible version is not online yet*
 - [ofx2DMapping](https://github.com/frauzufall/ofx2DMapping) *compatible version is not online yet*
 - [ofxMasterSlaveControl](https://github.com/frauzufall/ofxMasterSlaveControl)

Please tell me if you wrote something compatible, I will add it to the list. Check out the bottom of this page for contribution and implementation notes.

## Usage

### Basics
Just initialize some parameters and pass them on to the addon. It will create a panel showing the parameters. Have a look at [this example](https://github.com/frauzufall/ofxGuiExtended/example) to see it in action.
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
You can also create containers yourself.
```c++
ofxGuiPanel* panel = gui.addPanel();
panel->add(..);
```

In the same way you can add other containers to existing containers:
```c++
ofxGuiGroup* group = panel->addGroup();
group->add(..);
```

Some parameters automatically create containers if you add them to the gui:

```c++

ofParameter<ofPoint> position;
ofParameter<ofColor> color;

gui.add(position, color);

```

### Adding controls
Controls get automatically created whenever you add a parameter to the GUI.
```c++
panel->add(moving);    // creates a toggle
panel->add(speed);     // creates a slider
panel->add(rotation);  // creates a slider
```
You can also add a specific control (this is very useful if you implement your own control classes):
```c++
panel->add<ofxGuiLabel>("labeltext");
panel->add<yourControlElementClass>(/* contructor parameters of control element class */);
```

### Setting attributes
There are attributes that you can set for each element. You will find these attributes in the description of the classes beneath the examples on this page.
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
The goal is to set attributes in an external file to set style and layout without recompiling. This is not yet implemented but most of the work is done, stay tuned.

#### Styling items by type via Theme
In a theme you can define attributes for the different element class types. Have a look at [this example](https://github.com/frauzufall/ofxGuiExtended/exampleThemes). Also read the class descriptions in the next chapters of this page for the class type names and the attributes that you can set for each class.

This is how you load a theme:
```c++
panel = gui.addPanel();
panel->loadTheme("theme_light.json");
```

A theme file could look like this:
```json
//theme_light.json
{
    "light": {
        "base": {
            "background-color": "rgba(255,255,255,0.2)",
            "fill-color": "rgba(100,100,100,0.42)",
            "border-width": 0,
            "padding": 2,
            "border-color": "rgb(0,0,0)",
            "margin": 4,
            "text-color": "#000000"
        },

        "group": {
            "border-color": "rgba(0,0,0,0.7)",
            "padding": 0,
            "border-width": 0
        },

        "group-header": {
            "align-self": "flex-start",
            "flex":"none",
            "width": "100%",
            "margin": 0,
            "border-width": 0,
            "padding": 0,
            "text-padding": 3,
            "text-color": "#ffffff",
            "background-color": "#2da1e3"
        }
    }
}
```

### Using layouts
To be able to create more complex layouts, the ofxDOMFlexBoxLayout is implemented. If you want to use it, you have to set it up before adding elements to the GUI:
```c++
gui.setupFlexBoxLayout();
```
If you want to implement your own layout class, you can apply it to you GUI like this (assuming `ofxDOMLayoutCustom`` is your layout class):
```c++
gui.setup<ofxDOMLayoutCustom>();
```
See the chapter below about this layout class for details and have a look at [this example](https://github.com/frauzufall/ofxGuiExtended/exampleLayout).

## Controls

### `ofxGuiElement`
  - Derived from `DOM::Element`.
  - Base class of all other gui elements.
  - class type name for JSON theme: `base`
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
    <tr>
        <td>`margin`</td>
        <td>`float`<br>`string`</td>
        <td>`10`<br>`"10 20"`<br>`"10 20 30"`<br>`"10 20 30 40"`</td>
        <td>Set the margin of the element. Analog to CSS margin.</td>
    </tr>
    <tr>
        <td>`margin-top`<br>`margin-right`<br>`margin-bottom`<br>`margin-left`</td>
        <td>`float`</td>
        <td>`10`</td>
        <td>Set the margin for the specified side of the element.</td>
    </tr>
    <tr>
        <td>`padding`</td>
        <td>`float`<br>`string`</td>
        <td>`10`<br>`"10 20"`<br>`"10 20 30"`<br>`"10 20 30 40"`</td>
        <td>Set the padding of the element. Analog to CSS padding. Padding currently only has an effect on containers, not on controls.</td>
    </tr>
    <tr>
        <td>`padding-top`<br>`padding-right`<br>`padding-bottom`<br>`padding-left`</td>
        <td>`float`</td>
        <td>`10`</td>
        <td>Set the padding for the specified side of the element.</td>
    </tr>
</table>


### `ofxGuiToggle`
  - Derived from `ofxGuiElement`.
  - Default control type for `ofParameter<bool>`.
  - class type name for JSON theme: `toggle`
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
  - class type name for JSON theme: `button`

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
  - Displays a text or a numerical value as a label.
  - class type name for JSON theme: `label`

### `ofxGuiGraphics`
  - Derived from `ofxGuiElement`.
  - Displays any `ofBaseDraws` reference.
  - class type name for JSON theme: `graphics`

### `ofxGuiZoomableGraphics`
  - Derived from `ofxGuiGraphics`.
  - `ofBaseDraws` reference can be zoomed with scroll wheel and dragged with mouse button.

### `ofxGuiValuePlotter`
  - Derived from `ofxGuiElement`.
  - class type name for JSON theme: `value-plotter`
  - //TODO

### `ofxGuiFpsPlotter`
  - Derived from `ofxGuiValuePlotter`.
  - //TODO

### `ofxGuiFunctionPlotter`
  - Derived from `ofxGuiElement`.
  - class type name for JSON theme: `function-plotter`
  - //TODO


## Containers

### `ofxGuiGroup`
- derived from `ofxGuiElement`.
- Groups other elements according to the current layout
- class type name for JSON theme: `group`
- class type name of group header for JSON theme: `group-header`
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
      <td>`show-header`</td>
      <td>`bool`</td>
      <td>`true` (default)</td>
      <td>Show or hide the header of the group.</td>
  </tr>
</table>

### `ofxGuiPanel`
  - derived from `ofxGuiGroup`.
  - Header can be used to drag group (only if panel is in absolute position mode).
  - class type name for JSON theme: `panel`
- class type name of group header for JSON theme: `panel-header`

### `ofxGuiTabs`
  - derived from `ofxGuiGroup`.
  - Add groups to this container and they will be displayed as tabs.
  - class type name for JSON theme: `tabs`
  - //TODO


## Layouts

### `ofxDOMBoxLayout`
- Derived from `DOM::Layout`.
- Simple layout that lets you align elements in a container horizontally or vertically. Vertically aligned items will always use the full width of the container, horizontally aligned items will always use the full height of the container.
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
        <td>`direction`</td>
        <td>`string`</td>
        <td>`"vertical"` (default)<br>`"horizontal"`</td>
        <td>The direction child elements get aligned.</td>
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
