#pragma once
#include <vector>
#ifdef RACK_JSON
#include <jansson.h>
#endif /* RACK_JSON */
#include "newwidgets.hpp"


namespace rack {


struct Model;
struct Module;
struct Wire;

struct RackWidget;
struct ParamWidget;
struct Port;
#ifdef RACK_GUI
struct SVGPanel;
#endif /* RACK_GUI */

////////////////////
// module
////////////////////

// A 1U module should be 15x380. Thus the width of a module should be a factor of 15.
#define RACK_GRID_WIDTH 15
#define RACK_GRID_HEIGHT 380

static const Vec RACK_GRID_SIZE = Vec(15, 380);


struct ModuleWidget : OpaqueWidget {
	Model *model = NULL;
	/** Owns the module pointer */
	Module *module = NULL;

#ifdef RACK_GUI
	SVGPanel *panel = NULL;
#endif /* RACK_GUI */
	std::vector<Port*> inputs;
	std::vector<Port*> outputs;
	std::vector<ParamWidget*> params;

	~ModuleWidget();
	void setModule(Module *module);
	/** Convenience functions for adding special widgets (calls addChild()) */
	void addInput(Port *input);
	void addOutput(Port *output);
	void addParam(ParamWidget *param);
#ifdef RACK_GUI
	void setPanel(std::shared_ptr<SVG> svg);
#endif /* RACK_GUI */

#ifdef RACK_JSON
	virtual json_t *toJson();
	virtual void fromJson(json_t *rootJ);
#endif /* RACK_JSON */

	/** Disconnects cables from all ports
	Called when the user clicks Disconnect Cables in the context menu.
	*/
	virtual void disconnect();
	/** Resets the parameters of the module and calls the Module's randomize().
	Called when the user clicks Initialize in the context menu.
	*/
	virtual void reset();
	/** Deprecated */
	virtual void initialize() final {}
	/** Randomizes the parameters of the module and calls the Module's randomize().
	Called when the user clicks Randomize in the context menu.
	*/
	virtual void randomize();
#ifdef RACK_GUI
	virtual Menu *createContextMenu();

	void draw(NVGcontext *vg) override;

	Vec dragPos;
	void onMouseDown(EventMouseDown &e) override;
	void onMouseMove(EventMouseMove &e) override;
	void onHoverKey(EventHoverKey &e) override;
	void onDragStart(EventDragStart &e) override;
	void onDragEnd(EventDragEnd &e) override;
	void onDragMove(EventDragMove &e) override;
#endif /* RACK_GUI */
};

struct ValueLight;
struct WireWidget : OpaqueWidget {
	Port *outputPort = NULL;
	Port *inputPort = NULL;
	Port *hoveredOutputPort = NULL;
	Port *hoveredInputPort = NULL;
#ifdef RACK_GUI
	ValueLight *inputLight;
	ValueLight *outputLight;
#endif /* RACK_GUI */
	Wire *wire = NULL;
#ifdef RACK_GUI
	NVGcolor color;
#endif /* RACK_GUI */

	WireWidget();
	~WireWidget();
	/** Synchronizes the plugged state of the widget to the owned wire */
	void updateWire();
#ifdef RACK_GUI
	Vec getOutputPos();
	Vec getInputPos();
	void draw(NVGcontext *vg) override;
	void drawPlugs(NVGcontext *vg);
#endif /* RACK_GUI */
};

struct WireContainer : TransparentWidget {
	WireWidget *activeWire = NULL;
	/** Takes ownership of `w` and adds it as a child if it isn't already */
	void setActiveWire(WireWidget *w);
	/** "Drops" the wire onto the port, making an engine connection if successful */
	void commitActiveWire();
	void removeTopWire(Port *port);
	void removeAllWires(Port *port);
	/** Returns the most recently added wire connected to the given Port, i.e. the top of the stack */
	WireWidget *getTopWire(Port *port);
#ifdef RACK_GUI
	void draw(NVGcontext *vg) override;
#endif /* RACK_GUI */
};

struct RackWidget : OpaqueWidget {
	FramebufferWidget *rails;
	// Only put ModuleWidgets in here
	Widget *moduleContainer;
	// Only put WireWidgets in here
	WireContainer *wireContainer;
	std::string lastPath;
	Vec lastMousePos;

	RackWidget();
	~RackWidget();

	/** Completely clear the rack's modules and wires */
	void clear();
	/** Clears the rack and loads the template patch */
	void reset();
#ifdef RACK_GUI
	void openDialog();
	void saveDialog();
	void saveAsDialog();
#endif /* RACK_GUI */
	void savePatch(std::string filename);
	void loadPatch(std::string filename);
#ifdef RACK_JSON
	json_t *toJson();
	void fromJson(json_t *rootJ);
#endif /* RACK_JSON */

	void addModule(ModuleWidget *m);
	/** Transfers ownership to the caller so they must `delete` it if that is the intension */
	void deleteModule(ModuleWidget *m);
	void cloneModule(ModuleWidget *m);
#ifdef RACK_GUI
	/** Sets a module's box if non-colliding. Returns true if set */
	bool requestModuleBox(ModuleWidget *m, Rect box);
	/** Moves a module to the closest non-colliding position */
	bool requestModuleBoxNearest(ModuleWidget *m, Rect box);
#endif /* RACK_GUI */
	void step() override;
#ifdef RACK_GUI
	void draw(NVGcontext *vg) override;

	void onMouseMove(EventMouseMove &e) override;
	void onMouseDown(EventMouseDown &e) override;
	void onZoom(EventZoom &e) override;
#endif /* RACK_GUI */
};

#ifdef RACK_GUI
struct RackRail : TransparentWidget {
	void draw(NVGcontext *vg) override;
};
#endif /* RACK_GUI */

struct Panel : TransparentWidget {
#ifdef RACK_GUI
	NVGcolor backgroundColor;
	std::shared_ptr<Image> backgroundImage;
	void draw(NVGcontext *vg) override;
#endif /* RACK_GUI */
};

struct SVGPanel : FramebufferWidget {
#ifdef RACK_GUI
	void step() override;
	void setBackground(std::shared_ptr<SVG> svg);
#endif /* RACK_GUI */
};

////////////////////
// params
////////////////////

#ifdef RACK_GUI
struct CircularShadow : TransparentWidget {
	float blur = 0.0;
	void draw(NVGcontext *vg) override;
};
#endif /* RACK_GUI */

struct ParamWidget : OpaqueWidget, QuantityWidget {
	Module *module = NULL;
	int paramId;
	/** Used to momentarily disable value randomization
	To permanently disable or change randomization behavior, override the randomize() method instead of changing this.
	*/
	bool randomizable = true;

#ifdef RACK_JSON
	json_t *toJson();
	void fromJson(json_t *rootJ);
#endif /* RACK_JSON */
	virtual void randomize();
#ifdef RACK_GUI
	void onMouseDown(EventMouseDown &e) override;
	void onChange(EventChange &e) override;
#endif /* RACK_GUI */
};

/** Implements vertical dragging behavior for ParamWidgets */
struct Knob : ParamWidget {
#ifdef RACK_GUI
	/** Snap to nearest integer while dragging */
	bool snap = false;
	float dragValue;
	void onDragStart(EventDragStart &e) override;
	void onDragMove(EventDragMove &e) override;
	void onDragEnd(EventDragEnd &e) override;
	/** Tell engine to smoothly vary this parameter */
	void onChange(EventChange &e) override;
#endif /* RACK_GUI */
};

#ifdef RACK_GUI
struct SpriteKnob : virtual Knob, SpriteWidget {
	int minIndex, maxIndex, spriteCount;
	void step() override;
};
#endif /* RACK_GUI */

/** A knob which rotates an SVG and caches it in a framebuffer */
struct SVGKnob : virtual Knob, FramebufferWidget {
	/** Angles in radians */
	float minAngle, maxAngle;
#ifdef RACK_GUI
	/** Not owned */
	TransformWidget *tw;
	SVGWidget *sw;

	SVGKnob();
	void setSVG(std::shared_ptr<SVG> svg);
	void step() override;
	void onChange(EventChange &e) override;
#else
	void step(){
		FramebufferWidget::step();
	}
#endif /* RACK_GUI */
};

struct SVGSlider : Knob, FramebufferWidget {
#ifdef RACK_GUI
	/** Intermediate positions will be interpolated between these positions */
	Vec minHandlePos, maxHandlePos;
	/** Not owned */
	SVGWidget *background;
	SVGWidget *handle;

	SVGSlider();
	void step() override;
	void onChange(EventChange &e) override;
#endif /* RACK_GUI */
};

struct Switch : ParamWidget {
};

struct SVGSwitch : virtual Switch, FramebufferWidget {
#ifdef RACK_GUI
	std::vector<std::shared_ptr<SVG>> frames;
	/** Not owned */
	SVGWidget *sw;

	SVGSwitch();
	/** Adds an SVG file to represent the next switch position */
	void addFrame(std::shared_ptr<SVG> svg);
	void step() override;
	void onChange(EventChange &e) override;
#endif /* RACK_GUI */
};

/** A switch that cycles through each mechanical position */
struct ToggleSwitch : virtual Switch {
#ifdef RACK_GUI
	void onDragStart(EventDragStart &e) override {
		// Cycle through values
		// e.g. a range of [0.0, 3.0] would have modes 0, 1, 2, and 3.
		if (value >= maxValue)
			setValue(minValue);
		else
			setValue(value + 1.0);
	}
#endif /* RACK_GUI */
};

/** A switch that is turned on when held */
struct MomentarySwitch : virtual Switch {
	/** Don't randomize state */
	void randomize() override {}
#ifdef RACK_GUI
	void onDragStart(EventDragStart &e) override {
		setValue(maxValue);
	}
	void onDragEnd(EventDragEnd &e) override {
		setValue(minValue);
	}
#endif /* RACK_GUI */
};

////////////////////
// ports
////////////////////

struct Port : OpaqueWidget {
	enum PortType {
		INPUT,
		OUTPUT
	};

	Module *module = NULL;
	PortType type = INPUT;
	int portId;

	~Port();
#ifdef RACK_GUI
	void draw(NVGcontext *vg) override;
	void onMouseDown(EventMouseDown &e) override;
	void onDragStart(EventDragStart &e) override;
	void onDragEnd(EventDragEnd &e) override;
	void onDragDrop(EventDragDrop &e) override;
	void onDragEnter(EventDragEnter &e) override;
	void onDragLeave(EventDragEnter &e) override;
#endif /* RACK_GUI */
};

struct SVGPort : Port, FramebufferWidget {
#ifdef RACK_GUI
	SVGWidget *background;

	SVGPort();
	void draw(NVGcontext *vg) override;
#endif /* RACK_GUI */
};

/** If you don't add these to your ModuleWidget, they will fall out of the rack... */
struct SVGScrew : FramebufferWidget {
#ifdef RACK_GUI
	SVGWidget *sw;

	SVGScrew();
#endif /* RACK_GUI */
};

////////////////////
// lights
////////////////////

struct LightWidget : TransparentWidget {
#ifdef RACK_GUI
	NVGcolor bgColor = nvgRGBf(0, 0, 0);
	NVGcolor color = nvgRGBf(1, 1, 1);
	void draw(NVGcontext *vg) override;
#endif /* RACK_GUI */
};

/** A LightWidget that points to a module's Light or a range of lights */
struct ModuleLightWidget : LightWidget {
#ifdef RACK_GUI
	Module *module = NULL;
	int lightId;
#endif /* RACK_GUI */
};

/** Mixes colors based on the brightness of the module light at lightId, lightId + 1, etc */
struct ColorLightWidget : ModuleLightWidget {
#ifdef RACK_GUI
	std::vector<NVGcolor> colors;
	void addColor(NVGcolor c);
	void step() override;
#endif /* RACK_GUI */
};

////////////////////
// scene
////////////////////

#ifdef RACK_GUI
struct Toolbar : OpaqueWidget {
	Slider *wireOpacitySlider;
	Slider *wireTensionSlider;
	Slider *zoomSlider;
	RadioButton *cpuUsageButton;
	RadioButton *plugLightButton;

	Toolbar();
	void draw(NVGcontext *vg) override;
};

struct PluginManagerWidget : Widget {
	Widget *loginWidget;
	Widget *manageWidget;
	Widget *downloadWidget;
	PluginManagerWidget();
	void step() override;
};

struct RackScrollWidget : ScrollWidget {
	void step() override;
};

#endif /* RACK_GUI */
struct RackScene : Scene {
#ifdef RACK_GUI
	ScrollWidget *scrollWidget;
	ZoomWidget *zoomWidget;
#endif /* RACK_GUI */

	RackScene();
	void step() override;
#ifdef RACK_GUI
	void draw(NVGcontext *vg) override;
	void onHoverKey(EventHoverKey &e) override;
	void onPathDrop(EventPathDrop &e) override;
#endif /* RACK_GUI */
};

////////////////////
// globals
////////////////////

extern std::string gApplicationName;
extern std::string gApplicationVersion;
extern std::string gApiHost;

#ifdef RACK_GUI
// Easy access to "singleton" widgets
extern RackScene *gRackScene;
#endif /* RACK_GUI */
extern RackWidget *gRackWidget;
#ifdef RACK_GUI
extern Toolbar *gToolbar;
#endif /* RACK_GUI */

void sceneInit();
void sceneDestroy();

} // namespace rack

