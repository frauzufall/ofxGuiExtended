# ofxGuiExtended ALPHA

##Features
- **ofxGraphics**: shows any ofBaseDraws reference
- **ofxZoomableGraphics**: ofBaseDraws reference can be zoomed with scroll wheel and dragged with mouse button when in zoom
- **ofxMasterSlaveControl**: set controls like toggles and sliders to be master or slave controls. You can assign a master to control the slaves value. http://frauzufall.de/files/out.gif

##Features that were part of ofxGuiExtended but are now part of ofxGui
- **ofxGuiGroupExtended**, **ofxPanelExtended**: 
  - horizontal alignment 
  - header can be hidden
  - option to allow only one active toggle (only working with ofxMinimalToggle at the moment)
- **ofxMinimalToggle**, **ofxMinimalButton**: toggles and buttons with name inside
- **ofxRotarySlider**: rotary slider
- **ofxGuiSpacer**: spacing element
- **ofxGuiMatrix**: matrix of gui elements, column count and element height adjustable
- **ofxValuePlotter**: plots a value and displays the current value. option to set decimal place and value name
- **ofxFpsPlotter**: plots FPS

##!!!Important notes!!!
This branch is only working with this OF branch: https://github.com/frauzufall/openFrameworks/tree/refactor-gui-extended The masterSlaveControls are not yet ported to this branch so they are not yet working.
