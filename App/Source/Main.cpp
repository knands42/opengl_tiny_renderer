#include "Application.h"

int main()
{
    Core::ApplicationSpecification appSpec = Core::ApplicationSpecification();
    appSpec.Name = "OpenGL Tiny Renderer";
    appSpec.WindowSpec.Width = 1280;
    appSpec.WindowSpec.Height = 720;

    Core::Application app(appSpec);
    app.Run();

    return 0;
}
