#include "newapp.hpp"


namespace rack {

std::string gApplicationName = "VCV Rack";
std::string gApplicationVersion =
#ifdef VERSION
	TOSTRING(VERSION);
#else
	"";
#endif
std::string gApiHost = "http://api.vcvrack.com";

RackWidget *gRackWidget = NULL;
#ifdef RACK_GUI
Toolbar *gToolbar = NULL;
#endif /* RACK_GUI */
RackScene *gRackScene = NULL;


#ifdef RACK_GUI
void sceneInit() {
	gRackScene = new RackScene();
	gScene = gRackScene;
}

void sceneDestroy() {
	delete gScene;
	gScene = NULL;
}
#endif /* RACK_GUI */


} // namespace rack
