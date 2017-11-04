#include "newrack.hpp"


using namespace rack;


extern Plugin *plugin;

struct Knurlie : SVGScrew {
	Knurlie() {
#ifdef RACK_GUI
		sw->svg = SVG::load(assetPlugin(plugin, "res/Knurlie.svg"));
		sw->wrap();
		box.size = sw->box.size;
#endif /* RACK_GUI */
	}
};

void springReverbInit();

////////////////////
// module widgets
////////////////////

struct EvenVCOWidget : ModuleWidget {
	EvenVCOWidget();
};

struct RampageWidget : ModuleWidget {
	RampageWidget();
};

struct ABCWidget : ModuleWidget {
	ABCWidget();
};

struct SpringReverbWidget : ModuleWidget {
	SpringReverbWidget();
};

struct MixerWidget : ModuleWidget {
	MixerWidget();
};

struct SlewLimiterWidget : ModuleWidget {
	SlewLimiterWidget();
};

struct DualAtenuverterWidget : ModuleWidget {
	DualAtenuverterWidget();
};
