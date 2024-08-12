#include <iostream>

#include <camera/PerspecCamera.h>
#include <control/FPScontrol.h>
#include <geometry/BoxGeometry.h>
#include <material/CommonMaterial.h>
#include <core/Renderer.h>
#include <core/Scene.h>
#include <core/Window.h>
#include <core/Context.h>
#include <texture/ImageTexture.h>


#include <util/Logger.h>
using namespace SA;

int main()
{
  Window window(1920, 1080, 400, 200);
  Context& ctx = window.context;
  Renderer renderer;
  FPSControl control;
  PerspecCamera camera(45.0, 1920.0 / 1080.0, 0.01, 100.0);


  camera.setPosition(glm::vec3(0, 0, 5));
  control.bindObject(&camera);
  control.bindWindow(&window);
  control.mouse_sensitivity = 0.002;


  Scene scene;

  auto* box = new Object3D(new BoxGeometry(),
    new CommonMaterial(
      {
        {"diffuse", new ImageTexture("static/image/awesomeface.png")}
      }
    )
  );

  scene.add(box);

  ctx.loop = [&](double delta) {


    logger.log(camera.getDirection(), "main()::cam dir");
    renderer.render(scene, camera);
  };

  ctx.loop.get()(0.0);



  return 0;
}