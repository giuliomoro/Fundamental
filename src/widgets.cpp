#include "newwidgets.hpp"

namespace rack {

#ifdef RACK_GUI
Widget *gHoveredWidget = NULL;
Widget *gDraggedWidget = NULL;
Widget *gDragHoveredWidget = NULL;
Widget *gFocusedWidget = NULL;
#endif /* RACK_GUI */

Scene *gScene = NULL;

} // namespace rack
