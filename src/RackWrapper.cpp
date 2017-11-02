#include "newrack.hpp"
#include "RackWrapper.h"
#include "Fundamental.hpp"
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

void connect(Port* outputPort, Port* inputPort, Wire* wire)
{
	wire->inputModule = inputPort->module;
	wire->outputModule = outputPort->module;
	wire->inputId = inputPort->portId;
	wire->outputId = outputPort->portId;
}

bool RackSetup()
{
	rack::engineInit();
	plugin = new rack::Plugin();
	init(plugin);

	gen1 = new struct VCFWidget;
	gen2 = new struct VCOWidget;
	io = new struct IoWidget;
	wire1 = new Wire;
	wire2 = new Wire;

	connect(gen2->outputs[2], gen1->inputs[3], wire2);
	connect(gen1->outputs[0], io->inputs[0], wire1);
	engineAddWire(wire1);
	engineAddWire(wire2);
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
	delete wire1;
	delete wire2;
	delete gen1;
	delete gen2;
	delete io;
	engineDestroy();
}
