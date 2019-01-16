#include "Fundamental.hpp"
#include "Befaco.hpp"


Plugin *plugin;

void init(rack::Plugin *p) {
	plugin = p;
	p->slug = "Fundamental";
#ifdef VERSION
	p->version = TOSTRING(VERSION);
#endif
	p->addModel(createModel<VCOWidget>("Fundamental", "Fundamental", "VCO", "VCO-1"));
	p->addModel(createModel<VCO2Widget>("Fundamental", "Fundamental", "VCO2", "VCO-2"));
	p->addModel(createModel<VCFWidget>("Fundamental", "Fundamental", "VCF", "VCF"));
	p->addModel(createModel<VCAWidget>("Fundamental", "Fundamental", "VCA", "VCA"));
	p->addModel(createModel<LFOWidget>("Fundamental", "Fundamental", "LFO", "LFO-1"));
	p->addModel(createModel<LFO2Widget>("Fundamental", "Fundamental", "LFO2", "LFO-2"));
#ifdef RACK_GUI
	p->addModel(createModel<DelayWidget>("Fundamental", "Fundamental", "Delay", "Delay"));
#endif /* RACK_GUI */
	p->addModel(createModel<ADSRWidget>("Fundamental", "Fundamental", "ADSR", "ADSR"));
	p->addModel(createModel<VCMixerWidget>("Fundamental", "Fundamental", "VCMixer", "VC Mixer"));
	p->addModel(createModel<_8VERTWidget>("Fundamental", "Fundamental", "8VERT", "8VERT"));
#ifdef RACK_GUI
	p->addModel(createModel<ScopeWidget>("Fundamental", "Fundamental", "Scope", "Scope"));
	p->addModel(createModel<SEQ3Widget>("Fundamental", "Fundamental", "SEQ3", "SEQ-3"));
#endif /* RACK_GUI */
	p->addModel(createModel<EvenVCOWidget>("Befaco", "Befaco", "EvenVCO", "EvenVCO"));
	p->addModel(createModel<RampageWidget>("Befaco", "Befaco", "Rampage", "Rampage"));
	p->addModel(createModel<ABCWidget>("Befaco", "Befaco", "ABC", "A*B+C"));
	p->addModel(createModel<MixerWidget>("Befaco", "Befaco", "Mixer", "Mixer"));
	p->addModel(createModel<SlewLimiterWidget>("Befaco", "Befaco", "SlewLimiter", "Slew Limiter"));
	p->addModel(createModel<DualAtenuverterWidget>("Befaco", "Befaco", "DualAtenuverter", "Dual Atenuverter"));
	p->addModel(createModel<IoWidget>("Fundamental", "Fundamental", "Io", "Io"));
}
