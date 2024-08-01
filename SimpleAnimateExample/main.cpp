#include <iostream>

#include <camera/PerspecCamera.h>
#include <Context.h>
#include <control/FPScontrol.h>
#include <geometry/BoxGeometry.h>
#include <material/CommonMaterial.h>
#include <Renderer.h>
#include <Scene.h>
#include <texture/ImageTexture.h>

using namespace SA;

int main()
{
	std::cout << "Hello, world" << std::endl;

	Context ctx;
	Renderer renderer;
	FPSControl control;
	PerspecCamera camera(45.0, 1920.0 / 1080.0, 0.1, 100.0);

	control.bindObject(&camera);
	control.bindContext(&ctx);


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

	ctx.loop([&renderer, &scene, &camera, &control, &ctx]() {
		
		renderer.render(scene, camera);
		control.inputLoop(nullptr);
		//std::cout << "Hello, world" << std::endl;
		std::cout << ctx.getTime() << std::endl;


	});

	return 0;
}