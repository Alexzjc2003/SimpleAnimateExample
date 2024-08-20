#include <iostream>

#include <util/Logger.h>

#include <control/FPSControl.h>
#include <core/Window.h>
#include <geometry/BoxGeometry.h>
#include <material/BlinnPhongMaterial>

using namespace SA;

int main()
{
  // environment setting
  Window window;
  Context& ctx = window.context;
  FPSControl control;
  window.bindControl(&control);

  // A box, with blinn-phong material
  Object3D box(
    BoxGeometry(),
  )
  




  //system("pause");

  return 0;
}