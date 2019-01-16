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
	gRackWidget = new RackWidget;

	rack::engineInit();
	plugin = new rack::Plugin();
	init(plugin);
	gPlugins.push_back(plugin);
	gRackWidget->loadPatch("rack.vcv");
	return true;
	//io = new struct IoWidget(framesPerBlock, channelsToHost, channelsFromHost);

	gen1 = new struct VCFWidget;
	gen2 = new struct EvenVCOWidget;
	wire1 = new Wire;
	wire2 = new Wire;
	wire3 = new Wire;

	Port* vcf_freq = gen1->inputs[0];
	Port* vco_square = gen2->outputs[2];
	Port* analogIn0 = io->outputs[2];
	Port* audioOut0 = io->inputs[0];
	Port* vcf_in = gen1->inputs[3];
	Port* vcf_out = gen1->outputs[0];
	connect(analogIn0, vcf_freq, wire1);
	connect(vco_square, vcf_in, wire2);
	connect(vcf_out, audioOut0, wire3);

	engineAddWire(wire1);
	engineAddWire(wire2);
	engineAddWire(wire3);

	gen1->module->params[0].value = 0.5;
	gen1->module->params[3].value = 1;

	return true;
}

void RackRender()
{
	engineStep();
}

void RackCleanup()
{
	printf("Cleanup. Any errors after this are \"\"\"OK\"\"\"\n");
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

#ifdef RACK_JSON
// read from JSON!
#include <jansson.h>
#include <map>
#include <algorithm>
#include <thread>
#include <set>
#define OSDIALOG_INFO 0
#define OSDIALOG_WARNING 1
#define OSDIALOG_OK 0
int osdialog_message(int,int, const char* message)
{
	printf("%s\n", message);
	return 0;
}

RackWidget::RackWidget() {
	rails = new FramebufferWidget();
	rails->box.size = Vec();
#ifdef RACK_GUI
	rails->oversample = 1.0;
	{
		RackRail *rail = new RackRail();
		rail->box.size = Vec();
		rails->addChild(rail);
	}
	addChild(rails);
#endif /* RACK_GUI */

	moduleContainer = new Widget();
	addChild(moduleContainer);

	wireContainer = new WireContainer();
	addChild(wireContainer);
}

RackWidget::~RackWidget() {
}
void RackWidget::step() {}

void RackWidget::clear() {
	wireContainer->activeWire = NULL;
	wireContainer->clearChildren();
	moduleContainer->clearChildren();
	lastPath = "";
}
void RackWidget::loadPatch(std::string path) {
	printf("Loading patch %s\n", path.c_str());
	FILE *file = fopen(path.c_str(), "r");
	if (!file) {
		// Exit silently
		return;
	}

	json_error_t error;
	json_t *rootJ = json_loadf(file, 0, &error);
	if (rootJ) {
		clear();
		fromJson(rootJ);
		json_decref(rootJ);
	}
	else {
		std::string message = stringf("JSON parsing error at %s %d:%d %s\n", error.source, error.line, error.column, error.text);
		osdialog_message(OSDIALOG_WARNING, OSDIALOG_OK, message.c_str());
	}

	fclose(file);
}

void RackWidget::fromJson(json_t *rootJ) {
	std::string message;

	// version
	json_t *versionJ = json_object_get(rootJ, "version");
	if (versionJ) {
		std::string version = json_string_value(versionJ);
		if (!version.empty() && gApplicationVersion != version)
			message += stringf("This patch was created with Rack %s. Saving it will convert it to a Rack %s patch.\n\n", version.c_str(), gApplicationVersion.empty() ? "dev" : gApplicationVersion.c_str());
	}

	// modules
	std::map<int, ModuleWidget*> moduleWidgets;
	json_t *modulesJ = json_object_get(rootJ, "modules");
	if (!modulesJ) return;
	size_t moduleId;
	json_t *moduleJ;
	json_array_foreach(modulesJ, moduleId, moduleJ) {
		json_t *manufacturerSlugJ = json_object_get(moduleJ, "manufacturer");
		if (!manufacturerSlugJ) {
			// Backward compatibility with Rack v0.4 and lower
			manufacturerSlugJ = json_object_get(moduleJ, "plugin");
			if (!manufacturerSlugJ) continue;
		}
		json_t *modelSlugJ = json_object_get(moduleJ, "model");
		if (!modelSlugJ) continue;
		std::string manufacturerSlug = json_string_value(manufacturerSlugJ);
		std::string modelSlug = json_string_value(modelSlugJ);

		// Search for model
		Model *model = NULL;
		for (Plugin *plugin : gPlugins) {
			for (Model *m : plugin->models) {
				if (m->manufacturerSlug == manufacturerSlug && m->slug == modelSlug) {
					model = m;
				}
			}
		}

		if (!model) {
			message += stringf("Could not find \"%s %s\" module\n", manufacturerSlug.c_str(), modelSlug.c_str());
			continue;
		}

		// Create ModuleWidget
		ModuleWidget *moduleWidget = model->createModuleWidget();
		assert(moduleWidget);
		moduleWidget->fromJson(moduleJ);
		moduleContainer->addChild(moduleWidget);
		moduleWidgets[moduleId] = moduleWidget;
	}

	// wires
	json_t *wiresJ = json_object_get(rootJ, "wires");
	if (!wiresJ) return;
	size_t wireId;
	json_t *wireJ;
	json_array_foreach(wiresJ, wireId, wireJ) {
		int outputModuleId, outputId;
		int inputModuleId, inputId;
		int err = json_unpack(wireJ, "{s:i, s:i, s:i, s:i}",
			"outputModuleId", &outputModuleId, "outputId", &outputId,
			"inputModuleId", &inputModuleId, "inputId", &inputId);
		if (err) continue;
		// Get ports
		ModuleWidget *outputModuleWidget = moduleWidgets[outputModuleId];
		if (!outputModuleWidget) continue;
		Port *outputPort = outputModuleWidget->outputs[outputId];
		if (!outputPort) continue;
		ModuleWidget *inputModuleWidget = moduleWidgets[inputModuleId];
		if (!inputModuleWidget) continue;
		Port *inputPort = inputModuleWidget->inputs[inputId];
		if (!inputPort) continue;
		// Create WireWidget
		WireWidget *wireWidget = new WireWidget();
		wireWidget->outputPort = outputPort;
		wireWidget->inputPort = inputPort;
		wireWidget->updateWire();
		// Add wire to rack
		wireContainer->addChild(wireWidget);
	}

	// Display a message if we have something to say
	if (!message.empty()) {
		osdialog_message(OSDIALOG_INFO, OSDIALOG_OK, message.c_str());
	}
}
#endif /* RACK_JSON */
