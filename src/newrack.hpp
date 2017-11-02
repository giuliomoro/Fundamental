#pragma once

#include "math.hpp"
#include "plugin.hpp"
#include "engine.hpp"
#include "newapp.hpp"
#include "newcomponents.hpp"

namespace rack {

template <class TModuleWidget>
Model *createModel(std::string manufacturerSlug, std::string manufacturerName, std::string slug, std::string name) {
	struct TModel : Model {
		ModuleWidget *createModuleWidget() override {
			ModuleWidget *moduleWidget = new TModuleWidget();
			moduleWidget->model = this;
			return moduleWidget;
		}
	};
	Model *model = new TModel();
	model->slug = slug;
	model->name = name;
	model->manufacturerSlug = manufacturerSlug;
	model->manufacturerName = manufacturerName;
	return model;
}

template <class TParamWidget>
ParamWidget *createParam(Vec pos, Module *module, int paramId, float minValue, float maxValue, float defaultValue) {
	ParamWidget *param = new TParamWidget();
#ifdef RACK_GUI
	param->box.pos = pos;
#endif /* RACK_GUI */
	param->module = module;
	param->paramId = paramId;
	param->setLimits(minValue, maxValue);
	param->setDefaultValue(defaultValue);
	return param;
}

template <class TPort>
Port *createInput(Vec pos, Module *module, int inputId) {
	Port *port = new TPort();
#ifdef RACK_GUI
	port->box.pos = pos;
#endif /* RACK_GUI */
	port->module = module;
	port->type = Port::INPUT;
	port->portId = inputId;
	return port;
}

template <class TPort>
Port *createOutput(Vec pos, Module *module, int outputId) {
	Port *port = new TPort();
#ifdef RACK_GUI
	port->box.pos = pos;
#endif /* RACK_GUI */
	port->module = module;
	port->type = Port::OUTPUT;
	port->portId = outputId;
	return port;
}

} // namespace rack
