#include "Fundamental.hpp"
#include "Io.hpp"
#include <string.h>

struct rackIo* gRackIo = NULL;

struct Io : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		CH0_INPUT,
		CH1_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};

	Io() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;
};

void Io::step() {
	float ch1 = inputs[CH1_INPUT].value;
	unsigned int channel = 0;
	unsigned int element = gRackIo->currentOutFrame++ * gRackIo->audioOutChannels + channel;
#ifndef NDEBUG
	printf("[%3d]: %4.10f\n", element, ch1);
#endif
	gRackIo->audioOut[element] = ch1 * 0.2f;
	if(gRackIo->currentOutFrame == gRackIo->audioFrames)
		gRackIo->currentOutFrame = 0;
}

IoWidget::IoWidget() {
	gRackIo = new struct rackIo;
	memset(gRackIo, 0, sizeof(struct rackIo));
	gRackIo->audioFrames = 128;
	gRackIo->audioOutChannels = Io::NUM_INPUTS; // this is getting confusing
	gRackIo->audioIn = new float[gRackIo->audioInChannels * gRackIo->audioFrames];
	gRackIo->audioOut = new float[gRackIo->audioOutChannels * gRackIo->audioFrames];
	// TODO: freeme
	Io *module = new Io();
	setModule(module);
	addInput(createInput<PJ301MPort>(Vec(16, 69), module, Io::CH1_INPUT));
}
