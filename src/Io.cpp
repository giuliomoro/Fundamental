#include "Fundamental.hpp"
#include "Io.hpp"
#include <string.h>

struct rackIo* gRackIo = NULL;

struct Io : Module {
	enum InputIds {
		CH0_INPUT,
		CH1_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};

	Io(unsigned int framesPerBlock, unsigned int channelsFromHost, unsigned int channelsToHost) : Module(0, channelsFromHost, channelsToHost) {
		gRackIo = new struct rackIo;
		memset(gRackIo, 0, sizeof(struct rackIo));
		gRackIo->audioFrames = framesPerBlock;
		gRackIo->audioOutChannels = channelsFromHost;
		gRackIo->audioInChannels = channelsToHost;
		gRackIo->audioIn = new float[gRackIo->audioInChannels * gRackIo->audioFrames];
		gRackIo->audioOut = new float[gRackIo->audioOutChannels * gRackIo->audioFrames];
	}
	void step() override;
};

void Io::step() {
	for(unsigned int channel = 0; channel < gRackIo->audioOutChannels; ++channel)
	{
		float value = inputs[channel].value;
		unsigned int element = gRackIo->currentFrame * gRackIo->audioOutChannels + channel;
#ifndef NDEBUG
		printf("[%3d]: %4.10f     ", element, value);
#endif
		gRackIo->audioOut[element] = value * 0.2f;
	}
	++gRackIo->currentFrame;
	if(gRackIo->currentFrame == gRackIo->audioFrames)
		gRackIo->currentFrame = 0;
#ifndef NDEBUG
	printf("\n");
#endif
}

IoWidget::IoWidget(unsigned int framesPerBlock, unsigned int channelsToHost, unsigned int channelsFromHost) {
	Io *module = new Io(framesPerBlock, channelsToHost, channelsFromHost);
	setModule(module);
	for(unsigned int n = 0; n < module->inputs.size(); ++n)
		addInput(createInput<PJ301MPort>(Vec(16, 69), module, n));
	for(unsigned int n = 0; n < module->outputs.size(); ++n)
		addOutput(createOutput<PJ301MPort>(Vec(16, 69), module, n));
}
