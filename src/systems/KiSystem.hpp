#pragma once

#include "World.hpp"
#include "components/Velocity.hpp"
#include "ecs/Scene.hpp"
#include "ecs/SceneView.hpp"

#define MOVEMENT_SPEED (2)

/**
 * @brief System that handles the pathtaking of entities
 *
 */
struct KiSystem
{
	/**
	 * @brief Update the system
	 *
	 * @param scene Scene that provides entities and components
	 * @param dt Delta time between two updates
	 */
	void update(Scene& scene, float dt)
	{
		// Iterate over every entity
		for (auto entity : SceneView<Velocity>(scene))
		{
			auto velocity = scene.Get<Velocity>(entity);

			// Get random movement and apply the velocity
			int randomX = rand() % 3;
			int randomY = rand() % 3;
			unsigned int speed = (unsigned int)(MOVEMENT_SPEED * dt);

			if (randomX == 0)
			{
				velocity->x = speed;
			}
			else if (randomX == 1)
			{
				velocity->x = -speed;
			}
			else if (randomX == 2)
			{
				velocity->x = 0;
			}
			if (randomY == 0)
			{
				velocity->y = speed;
			}
			else if (randomY == 1)
			{
				velocity->y = -speed;
			}
			else if (randomY == 2)
			{
				velocity->y = 0;
			}
		}
	}
};