#include "newrack.hpp"


using namespace rack;


extern Plugin *plugin;

////////////////////
// module widgets
////////////////////

struct IoWidget : ModuleWidget {
	//IoWidget(unsigned int framesPerBlock, unsigned int inChannels, unsigned int outChannels);
	IoWidget();
};

struct VCOWidget : ModuleWidget {
	VCOWidget();
};

struct VCO2Widget : ModuleWidget {
	VCO2Widget();
};

struct VCFWidget : ModuleWidget {
	VCFWidget();
};

struct VCAWidget : ModuleWidget {
	VCAWidget();
};

struct LFOWidget : ModuleWidget {
	LFOWidget();
};

struct LFO2Widget : ModuleWidget {
	LFO2Widget();
};

#ifdef RACK_GUI
struct DelayWidget : ModuleWidget {
	DelayWidget();
};
#endif /* RACK_GUI */

struct ADSRWidget : ModuleWidget {
	ADSRWidget();
};

struct VCMixerWidget : ModuleWidget {
	VCMixerWidget();
};

struct _8VERTWidget : ModuleWidget {
	_8VERTWidget();
};

// struct ScopeWidget : ModuleWidget {
	// ScopeWidget();
// };

struct SEQ3Widget : ModuleWidget {
	SEQ3Widget();
#ifdef RACK_GUI
	Menu *createContextMenu() override;
#endif /* RACK_GUI */
};
