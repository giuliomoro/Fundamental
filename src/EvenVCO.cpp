#include "Befaco.hpp"
#include "dsp/minblep.hpp"
#include "dsp/filter.hpp"


struct EvenVCO : Module {
	enum ParamIds {
		OCTAVE_PARAM,
		TUNE_PARAM,
		PWM_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PITCH1_INPUT,
		PITCH2_INPUT,
		FM_INPUT,
		SYNC_INPUT,
		PWM_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		TRI_OUTPUT,
		SINE_OUTPUT,
		EVEN_OUTPUT,
		SAW_OUTPUT,
		SQUARE_OUTPUT,
		NUM_OUTPUTS
	};

	float phase = 0.0;
	/** The value of the last sync input */
	float sync = 0.0;
	/** The outputs */
	float tri = 0.0;
	/** Whether we are past the pulse width already */
	bool halfPhase = false;

	MinBLEP<16> triSquareMinBLEP;
	MinBLEP<16> triMinBLEP;
	MinBLEP<16> sineMinBLEP;
	MinBLEP<16> doubleSawMinBLEP;
	MinBLEP<16> sawMinBLEP;
	MinBLEP<16> squareMinBLEP;

	RCFilter triFilter;

	EvenVCO();
	void step() override;
};


EvenVCO::EvenVCO() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {
	triSquareMinBLEP.minblep = minblep_16_32;
	triSquareMinBLEP.oversample = 32;
	triMinBLEP.minblep = minblep_16_32;
	triMinBLEP.oversample = 32;
	sineMinBLEP.minblep = minblep_16_32;
	sineMinBLEP.oversample = 32;
	doubleSawMinBLEP.minblep = minblep_16_32;
	doubleSawMinBLEP.oversample = 32;
	sawMinBLEP.minblep = minblep_16_32;
	sawMinBLEP.oversample = 32;
	squareMinBLEP.minblep = minblep_16_32;
	squareMinBLEP.oversample = 32;
}

void EvenVCO::step() {
	// Compute frequency, pitch is 1V/oct
	float pitch = 1.0 + roundf(params[OCTAVE_PARAM].value) + params[TUNE_PARAM].value / 12.0;
	pitch += inputs[PITCH1_INPUT].value + inputs[PITCH2_INPUT].value;
	pitch += inputs[FM_INPUT].value / 4.0;
	float freq = 261.626 * powf(2.0, pitch);
	freq = clampf(freq, 0.0, 20000.0);

	// Pulse width
	float pw = params[PWM_PARAM].value + inputs[PWM_INPUT].value / 5.0;
	const float minPw = 0.05;
	pw = rescalef(clampf(pw, -1.0, 1.0), -1.0, 1.0, minPw, 1.0-minPw);

	// Advance phase
	float deltaPhase = clampf(freq / engineGetSampleRate(), 1e-6, 0.5);
	float oldPhase = phase;
	phase += deltaPhase;

	if (oldPhase < 0.5 && phase >= 0.5) {
		float crossing = -(phase - 0.5) / deltaPhase;
		triSquareMinBLEP.jump(crossing, 2.0);
		doubleSawMinBLEP.jump(crossing, -2.0);
	}

	if (!halfPhase && phase >= pw) {
		float crossing  = -(phase - pw) / deltaPhase;
		squareMinBLEP.jump(crossing, 2.0);
		halfPhase = true;
	}

	// Reset phase if at end of cycle
	if (phase >= 1.0) {
		phase -= 1.0;
		float crossing = -phase / deltaPhase;
		triSquareMinBLEP.jump(crossing, -2.0);
		doubleSawMinBLEP.jump(crossing, -2.0);
		squareMinBLEP.jump(crossing, -2.0);
		sawMinBLEP.jump(crossing, -2.0);
		halfPhase = false;
	}

	// Outputs
	float triSquare = (phase < 0.5) ? -1.0 : 1.0;
	triSquare += triSquareMinBLEP.shift();

	// Integrate square for triangle
	tri += 4.0 * triSquare * freq / engineGetSampleRate();
	tri *= (1.0 - 40.0 / engineGetSampleRate());

	float sine = -cosf(2*M_PI * phase);
	float doubleSaw = (phase < 0.5) ? (-1.0 + 4.0*phase) : (-1.0 + 4.0*(phase - 0.5));
	doubleSaw += doubleSawMinBLEP.shift();
	float even = 0.55 * (doubleSaw + 1.27 * sine);
	float saw = -1.0 + 2.0*phase;
	saw += sawMinBLEP.shift();
	float square = (phase < pw) ? -1.0 : 1.0;
	square += squareMinBLEP.shift();

	// Set outputs
	outputs[TRI_OUTPUT].value = 5.0*tri;
	outputs[SINE_OUTPUT].value = 5.0*sine;
	outputs[EVEN_OUTPUT].value = 5.0*even;
	outputs[SAW_OUTPUT].value = 5.0*saw;
	outputs[SQUARE_OUTPUT].value = 5.0*square;
}


EvenVCOWidget::EvenVCOWidget() {
	EvenVCO *module = new EvenVCO();
	setModule(module);
	box.size = Vec(15*8, 380);

#ifdef RACK_GUI
	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/EvenVCO.svg")));
		addChild(panel);
	}

	addChild(createScrew<Knurlie>(Vec(15, 0)));
	addChild(createScrew<Knurlie>(Vec(15, 365)));
	addChild(createScrew<Knurlie>(Vec(15*6, 0)));
	addChild(createScrew<Knurlie>(Vec(15*6, 365)));
#endif /* RACK_GUI */

	addParam(createParam<BefacoBigSnapKnob>(Vec(22, 32), module, EvenVCO::OCTAVE_PARAM, -5.0, 4.0, 0.0));
	addParam(createParam<BefacoTinyKnob>(Vec(73, 131), module, EvenVCO::TUNE_PARAM, -7.0, 7.0, 0.0));
	addParam(createParam<Davies1900hRedKnob>(Vec(16, 230), module, EvenVCO::PWM_PARAM, -1.0, 1.0, 0.0));

	addInput(createInput<PJ301MPort>(Vec(8, 120), module, EvenVCO::PITCH1_INPUT));
	addInput(createInput<PJ301MPort>(Vec(19, 157), module, EvenVCO::PITCH2_INPUT));
	addInput(createInput<PJ301MPort>(Vec(48, 183), module, EvenVCO::FM_INPUT));
	addInput(createInput<PJ301MPort>(Vec(86, 189), module, EvenVCO::SYNC_INPUT));

	addInput(createInput<PJ301MPort>(Vec(72, 236), module, EvenVCO::PWM_INPUT));

	addOutput(createOutput<PJ301MPort>(Vec(10, 283), module, EvenVCO::TRI_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(87, 283), module, EvenVCO::SINE_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(48, 306), module, EvenVCO::EVEN_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(10, 327), module, EvenVCO::SAW_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(87, 327), module, EvenVCO::SQUARE_OUTPUT));
}
