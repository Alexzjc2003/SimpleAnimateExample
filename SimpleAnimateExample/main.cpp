#include <iostream>

#include <util/Logger.h>

#include <camera/PerspecCamera.h>
#include <control/FPSControl.h>
#include <core/Object3D.h>
#include <core/Renderer.h>
#include <core/Scene.h>
#include <core/Window.h>
#include <geometry/BoxGeometry.h>
#include <light/Pointlight.h>
#include <light/DirectionalLight.h>
#include <light/SpotLight.h>
#include <material/BlinnPhongMaterial.h>
#include <texture/ImageTexture.h>

using namespace SA;

int main()
{
  // environment setting
  Window window = Window(1920, 1080, 400, 200);
  Context& ctx = window.context;

  // A renderer
  Renderer renderer = Renderer();

  // A scene, totally black and empty
  Scene scene = Scene();

  // A cam, very normal cam
  PerspecCamera cam = PerspecCamera(45.0f, 1920.0f / 1080.0f, 0.1f, 50.0f);
  cam.setPos({ 0.0, 0.0, 1.0 });
  scene.add(&cam);

  // Several boxes, with blinn-phong material
  auto geometry = BoxGeometry();
  auto material = BlinnPhongMaterial(
    {
      {"diffuse", new ImageTexture("./static/image/container_diffuse.png")},
      {"specular", new ImageTexture("./static/image/container_specular.png")}
    }, 32.0f);

  //std::vector<Object3D> boxes;
  std::array<Object3D*, 10> boxes;
  for (size_t i = 0; i < 10; i++)
  {
    boxes[i] = new Object3D(&geometry, &material);
    scene.add(boxes[i]);
    boxes[i]->rotateOnWorldAxis({ 1.0f, 0.3f, 0.5f }, glm::radians(i * 20.0f));
  }
  boxes[0]->setPos({ 0.0f, 0.0f, 0.0f });
  boxes[1]->setPos({ 2.0f, 5.0f, -15.0f });
  boxes[2]->setPos({ -1.5f, -2.2f, -2.5f });
  boxes[3]->setPos({ -3.8f, -2.0f, -12.3f });
  boxes[4]->setPos({ 2.4f, -0.4f, -3.5f });
  boxes[5]->setPos({ -1.7f, 3.0f, -7.5f });
  boxes[6]->setPos({ 1.3f, -2.0f, -2.5f });
  boxes[7]->setPos({ 1.5f, 2.0f, -2.5f });
  boxes[8]->setPos({ 1.5f, 0.2f, -1.5f });
  boxes[9]->setPos({ -1.3f, 1.0f, -1.5f });

  // Several lights
  // 4 point lights
  glm::mat3 ads = {
      glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f) };
  glm::vec3 atten = { 1.0f, 0.09f, 0.032f };
  PointLight pointlights[4] = {
      {ads, glm::vec3(0.7f, 0.2f, 2.0f), atten},
      {ads, glm::vec3(2.3f, -3.3f, -4.0f), atten},
      {ads, glm::vec3(-4.0f, 2.0f, -12.0f), atten},
      {ads, glm::vec3(0.0f, 0.0f, -3.0f), atten} };
  for (auto& light : pointlights)
    scene.add(&light);
  ;

  // 1 directional light
  ads = { glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f) };
  DirectionalLight directionallight = {
      ads, glm::vec3(-0.2f, -1.0f, -0.3f) };
  scene.add(&directionallight);

  // 1 spot light
  ads = { glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f) };
  SpotLight spotlight = {
      ads, glm::vec3(0.0f), Object3D::default_front, 15.0f, 12.5f, atten };
  cam.add(&spotlight);

  // A control to control our cam
  FPSControl control;
  control.bindWindow(&window);
  control.bindObject(&cam);
  control.mouse_sensitivity = 0.002f;
  control.move_speed = 2.0f;
  control.fly_speed = 1.6f;

  // render loop
  ctx.loop = [&](double delta)
  {
    //logger.log(cam.getModelLocal(), "mL");
    logger.log(cam.getModelWorld(), "cam Mworld");
    logger.log(spotlight.getModelWorld(), "spot Mworld");
    //logger.log(spotlight.getPosWorld(), "spot.pos");
    //logger.log(spotlight.getPosLocal(), "spot.localpos");
    logger.log(spotlight.getDirLocal(), "spot.localdir");
    logger.log(spotlight.getDirWorld(), "spot.worlddir");
    renderer.render(scene, cam);
  };

  ctx.loop.get()(0.0);
  return 0;
}