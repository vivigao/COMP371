// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.

#include "Renderer.h"
#include "World.h"
#include "EventManager.h"
#include "Billboard.h"
#include "TextureLoader.h"
#include "BSpline.h"

#if defined(PLATFORM_OSX)
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
//#include <OpenGL/gl.h>


int main(int argc, char*argv[]){
	EventManager::Initialize();
	Renderer::Initialize();
	World world;

	if (argc > 1)
		world.LoadScene(argv[1]);
	else{
#if defined(PLATFORM_OSX)	
		world.LoadScene("Scenes/project.scene");
		//world.LoadScene("Scenes/AnimatedSceneWithParticles.scene");
		//		world.LoadScene("Scenes/AnimatedScene.scene");
		//		world.LoadScene("Scenes/StaticScene.scene");
		//		world.LoadScene("Scenes/CoordinateSystem.scene");
#else
		world.LoadScene("../Assets/Scenes/project.scene");
		//world.LoadScene("../Assets/Scenes/AnimatedSceneWithParticles.scene");
		//		world.LoadScene("../Assets/Scenes/AnimatedScene.scene");
		//		world.LoadScene("../Assets/Scenes/StaticScene.scene");
		//		world.LoadScene("../Assets/Scenes/CoordinateSystem.scene");
#endif
	}

	// Main Loop
	do{
		// Update Event Manager - Frame time / input / events processing 
		EventManager::Update();

		// Update World
		float dt = EventManager::GetFrameTime();
		world.Update(dt);

		// Draw World
		world.Draw();
	} while (EventManager::ExitRequested() == false);

	Renderer::Shutdown();
	EventManager::Shutdown();

	return 0;
}