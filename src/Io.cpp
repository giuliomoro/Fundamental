#include "Fundamental.hpp"
#include "Io.hpp"
#include <string.h>

//#define NDEBUG
#undef RACK_GUI

struct rackIo* gRackIo = NULL;

struct Io : Module {

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
	for(unsigned int channel = 0; channel < gRackIo->audioInChannels; ++channel)
	{
		unsigned int element = gRackIo->currentFrame * gRackIo->audioInChannels + channel;
		float value = gRackIo->audioIn[element];
		outputs[channel].value = value * 5.f;
		//if(channel == 2)
			//printf("inputs: %.3f\n", value);
	}
	for(unsigned int channel = 0; channel < gRackIo->audioOutChannels; ++channel)
	{
		float value = inputs[channel].value;
		unsigned int element = gRackIo->currentFrame * gRackIo->audioOutChannels + channel;
#ifndef NDEBUG
		//printf("[%3d]: %2.4f     ", element, value);
#endif
		gRackIo->audioOut[element] = value * 0.2f;
	}
#ifndef NDEBUG
	//printf("\n");
#endif
	++gRackIo->currentFrame;
	if(gRackIo->currentFrame == gRackIo->audioFrames)
		gRackIo->currentFrame = 0;
}

IoWidget::IoWidget(
#ifndef RACK_GUI
	unsigned int framesPerBlock, unsigned int channelsToHost, unsigned int channelsFromHost
#endif /* ndef RACK_GUI */
	) {
#ifdef RACK_GUI
	unsigned int framesPerBlock = 64;
	unsigned int channelsToHost = 10;
	unsigned int channelsFromHost = 10;
#endif /* RACK_GUI */
	Io *module = new Io(framesPerBlock, channelsToHost, channelsFromHost);
	setModule(module);
#ifndef RACK_GUI
	for(unsigned int n = 0; n < module->inputs.size(); ++n)
		addInput(createInput<PJ301MPort>(Vec(0, 0), module, n));
	for(unsigned int n = 0; n < module->outputs.size(); ++n)
		addOutput(createOutput<PJ301MPort>(Vec(0, 0), module, n));
#else /* ndef RACK_GUI */
	// This code mostly from Rack/src/core/AudioInterface.cpp
	box.size = Vec(15*14, 380);

	{
		Panel *panel = new LightPanel();
		panel->box.size = box.size;
		addChild(panel);
	}

	float margin = 5;
	float labelHeight = 15;
	float yPos = margin;
	float xPos;

	{
		Label *label = new Label();
		label->box.pos = Vec(margin, yPos);
		label->text = "Bela";
		addChild(label);
		yPos += labelHeight + margin;
	}

	/*
	{
		Label *label = new Label();
		label->box.pos = Vec(margin, yPos);
		label->text = "Sample rate";
		addChild(label);
		yPos += labelHeight + margin;

		SampleRateChoice *choice = new SampleRateChoice();
		choice->audioInterface = dynamic_cast<AudioInterface*>(module);
		choice->box.pos = Vec(margin, yPos);
		choice->box.size.x = box.size.x - 2*margin;
		addChild(choice);
		yPos += choice->box.size.y + margin;
	}
	*/

	/*
	{
		Label *label = new Label();
		label->box.pos = Vec(margin, yPos);
		label->text = "Block size";
		addChild(label);
		yPos += labelHeight + margin;

		BlockSizeChoice *choice = new BlockSizeChoice();
		choice->audioInterface = dynamic_cast<AudioInterface*>(module);
		choice->box.pos = Vec(margin, yPos);
		choice->box.size.x = box.size.x - 2*margin;
		addChild(choice);
		yPos += choice->box.size.y + margin;
	}
	*/

	{
		Label *label = new Label();
		label->box.pos = Vec(margin, yPos);
		label->text = "Outputs";
		addChild(label);
		yPos += labelHeight + margin;
	}

	yPos += 5;
	xPos = 10;
	for (int i = 0; i < 5; i++) {
		addInput(createInput<PJ3410Port>(Vec(xPos, yPos), module, i));
		Label *label = new Label();
		label->box.pos = Vec(xPos + 4, yPos + 28);
		label->text = stringf("%d", i + 1);
		addChild(label);

		xPos += 37 + margin;
	}
	yPos += 35 + margin;

	yPos += 5;
	xPos = 10;
	for (int i = 5; i < 10; i++) {
		addInput(createInput<PJ3410Port>(Vec(xPos, yPos), module, i));
		Label *label = new Label();
		label->box.pos = Vec(xPos + 4, yPos + 28);
		label->text = stringf("%d", i + 1);
		addChild(label);

		xPos += 37 + margin;
	}
	yPos += 35 + margin;

	{
		Label *label = new Label();
		label->box.pos = Vec(margin, yPos);
		label->text = "Inputs";
		addChild(label);
		yPos += labelHeight + margin;
	}

	yPos += 5;
	xPos = 10;
	for (int i = 0; i < 5; i++) {
		addOutput(createOutput<PJ3410Port>(Vec(xPos, yPos), module, i));
		Label *label = new Label();
		label->box.pos = Vec(xPos + 4, yPos + 28);
		label->text = stringf("%d", i + 1);
		addChild(label);

		xPos += 37 + margin;
	}
	yPos += 35 + margin;

	yPos += 5;
	xPos = 10;
	for (int i = 5; i < 10; i++) {
		addOutput(createOutput<PJ3410Port>(Vec(xPos, yPos), module, i));
		Label *label = new Label();
		label->box.pos = Vec(xPos + 4, yPos + 28);
		label->text = stringf("%d", i + 1);
		addChild(label);

		xPos += 37 + margin;
	}
	yPos += 35 + margin;
#endif /* RACK_GUI */
}
