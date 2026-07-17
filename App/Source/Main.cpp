#include "AppLayer.h"
#include "Application.h"

int main()
{
    Core::ApplicationSpecification appSpec = Core::ApplicationSpecification();
    appSpec.Name = "OpenGL Tiny Renderer";
    appSpec.WindowSpec.Width = 1280;
    appSpec.WindowSpec.Height = 720;

    Core::Application app(appSpec);
    app.PushLayer<App::AppLayer>();
    app.Run();

    return 0;
}
