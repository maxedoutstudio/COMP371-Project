//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//


#include "Renderer.h"
#include "World.h"
#include "EventManager.h"
#include "Billboard.h"
#include "TextureLoader.h"

#include <GLFW/glfw3.h>


int main(int argc, char*argv[])
{

	int level = 0;
	while (level < 2){

	EventManager::Initialize();
	Renderer::Initialize();

	World world = World(level);    
    
	if (argc > 1)
	{
		world.LoadScene(argv[1]);
	}
	else
	{
		// TODO - You can alternate between different scenes for testing different things
		// Static Scene contains no animation
		// Animated Scene does
#if defined(PLATFORM_OSX)		
		world.LoadScene("Scenes/AnimatedSceneWithParticles.scene");
//		world.LoadScene("Scenes/AnimatedScene.scene");
//		world.LoadScene("Scenes/StaticScene.scene");
//		world.LoadScene("Scenes/CoordinateSystem.scene");
#else
		if (level == 0) { world.LoadScene("../Assets/Scenes/AnimatedSceneWithParticles.scene"); }
		if (level == 1) { world.LoadScene("../Assets/Scenes/MarsAnimatedSceneWithParticles.scene"); }
//		world.LoadScene("../Assets/Scenes/AnimatedScene.scene");
//		world.LoadScene("../Assets/Scenes/StaticScene.scene");
//		world.LoadScene("../Assets/Scenes/CoordinateSystem.scene");
#endif
	}

	// Main Loop


	do
	{
		// Update Event Manager - Frame time / input / events processing 
		EventManager::Update();

		// Update World
		float dt = EventManager::GetFrameTime();

		//Different level testing

		if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_L) == GLFW_PRESS)
		{
			break;
		}

		world.Update(dt);

		// Draw World
		world.Draw();        
	}
	while(EventManager::ExitRequested() == false);

	Renderer::Shutdown();
	EventManager::Shutdown();
	level += 1;
	}
	return 0;
}
