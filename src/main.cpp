#include "newrack.hpp"
#include "Fundamental.hpp"
#include <stdlib.h>
#include <unistd.h>

extern rack::Plugin* plugin;
namespace rack
{
	void engineStep();
}
int main()
{
	rack::engineInit();
	plugin = new rack::Plugin();
	init(plugin);

	struct VCOWidget* vco = new struct VCOWidget;
	struct VCFWidget* vcf = new struct VCFWidget;
	struct IoWidget* io = new struct IoWidget;
	Wire* wire = new Wire;

	Port* outputPort = vco->outputs[0];
	// Port* outputPort = vcf->inputs[0];
	Port* inputPort = io->inputs[0];
	// wire.inputModule = vco->outputs[0]->module;
	wire->inputModule = inputPort->module;
	wire->outputModule = outputPort->module;
	wire->inputId = inputPort->portId;
	wire->outputId = outputPort->portId;

	engineAddWire(wire);

	for(int n = 0; n < 1000; ++n)
	{
		usleep(10000);
		engineStep();
	}

	engineRemoveWire(wire);
	delete wire;
	delete vco;
	delete vcf;
	delete io;
	engineDestroy();
	return 0;
}
