#include <iostream>

#include <camera/PerspecCamera.h>
#include <control/FPScontrol.h>
#include <geometry/BoxGeometry.h>
#include <material/CommonMaterial.h>
#include <Renderer.h>
#include <Scene.h>
#include <texture/ImageTexture.h>

#include <Context.h>
using namespace SA;

int main()
{

	Context ctx;
	Renderer renderer;
	FPSControl control;
	PerspecCamera camera(45.0, 1920.0 / 1080.0, 0.01, 100.0);
	camera.setPosition(glm::vec3(0, 0, -1));


	//control.bindObject(&camera);
	//control.bindContext(&ctx);


	Scene scene;

	auto* box = new Object3D(
		new BoxGeometry(),
		new CommonMaterial(
			{
				{"diffuse", new ImageTexture("static/image/awesomeface.png")}
			}
		)
	);

	scene.add(box);

	ctx.loop([&]() {

		//control.inputLoop(&ctx);
		camera.setPosition(glm::vec3(0, 0, ctx.getTime() < 10 ? ctx.getTime():10));
		renderer.render(scene, camera);
	});

	//ctx.loop([]() {});


	return 0;
}