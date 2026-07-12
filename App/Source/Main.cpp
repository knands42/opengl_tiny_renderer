#include "Application.h"


int main()
{
    App::ApplicationSpecification appSpec = App::ApplicationSpecification();
   	appSpec.Name = "OpenGL Tiny Renderer";
	appSpec.WindowSpec.Width = 1280;
	appSpec.WindowSpec.Height = 720;
	
    App::Application app(appSpec);
    app.Run();

    return 0;
}
