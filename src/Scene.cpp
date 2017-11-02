#include "newwidgets.hpp"
#ifdef RACK_GUI
#include "gui.hpp"
#endif /* RACK_GUI */


namespace rack {

#ifdef RACK_GUI
void Scene::setOverlay(Widget *w) {
	if (overlay) {
		removeChild(overlay);
		delete overlay;
		overlay = NULL;
	}
	if (w) {
		addChild(w);
		overlay = w;
		overlay->box.pos = Vec();
	}
}
#endif /* RACK_GUI */

#ifdef RACK_GUI
Menu *Scene::createMenu() {
	// Get relative position of the click
	MenuOverlay *overlay = new MenuOverlay();
	Menu *menu = new Menu();
	menu->box.pos = gMousePos;

	overlay->addChild(menu);
	gScene->setOverlay(overlay);

	return menu;
}
#endif /* RACK_GUI */

void Scene::step() {
#ifdef RACK_GUI
	if (overlay) {
		overlay->box.size = box.size;
	}
#endif /* RACK_GUI */

	Widget::step();
}


} // namespace rack
