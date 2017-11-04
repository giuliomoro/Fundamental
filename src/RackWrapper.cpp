#include "newrack.hpp"
#include "RackWrapper.h"
#include "Fundamental.hpp"
#include "Befaco.hpp"
#include <stdlib.h>
#include <unistd.h>

extern rack::Plugin* plugin;
namespace rack
{
	void engineStep();
}

static struct ModuleWidget* gen1 = NULL;
static struct ModuleWidget* gen2 = NULL;
static struct ModuleWidget* io = NULL;
static Wire* wire1 = NULL;
static Wire* wire2 = NULL;
static Wire* wire3 = NULL;

void connect(Port* outputPort, Port* inputPort, Wire* wire)
{
	wire->inputModule = inputPort->module;
	wire->outputModule = outputPort->module;
	wire->inputId = inputPort->portId;
	wire->outputId = outputPort->portId;
}

bool RackSetup(unsigned int framesPerBlock, unsigned int channelsToHost, unsigned int channelsFromHost)
{
	rack::engineInit();
	plugin = new rack::Plugin();
	init(plugin);

	gen1 = new struct VCFWidget;
	gen2 = new struct EvenVCOWidget;
	io = new struct IoWidget(framesPerBlock, channelsToHost, channelsFromHost);
	wire1 = new Wire;
	wire2 = new Wire;
	wire3 = new Wire;

	connect(gen1->outputs[1], io->inputs[1], wire3);
	connect(gen1->outputs[0], io->inputs[0], wire1);
	connect(gen2->outputs[2], gen1->inputs[3], wire2);
	engineAddWire(wire1);
	engineAddWire(wire2);
	engineAddWire(wire3);
	return true;
}

void RackRender(float value)
{
	gen1->module->params[0].value = value;
	engineStep();
}

void RackCleanup()
{
	engineRemoveWire(wire1);
	engineRemoveWire(wire2);
	engineRemoveWire(wire3);
	delete wire1;
	delete wire2;
	delete wire3;
	delete gen1;
	delete gen2;
	delete io;
	engineDestroy();
}
