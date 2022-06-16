#pragma once

#include "World.hpp"
#include "components/Position.hpp"
#include "components/Velocity.hpp"
#include "ecs/Scene.hpp"
#include "ecs/SceneView.hpp"

/**
 * @brief System that handles the movement of entities
 *
 */
struct MovementSystem
{
	/**
	 * @brief Update the system
	 *
	 * @param scene Scene that provides entities and components
	 * @param dt Delta time between two updates
	 * @param world World in which the entities move
	 */
	void update(Scene& scene, float dt, World& world)
	{
		// Iterate over every entity
		for (auto entity : SceneView<Position, Velocity>(scene))
		{
			auto pos = scene.Get<Position>(entity);
			auto velocity = scene.Get<Velocity>(entity);

			// If the horizontal movement is within the world bounds, move
			if (world.inWorld(pos->x + velocity->x, pos->y))
			{
				pos->x += velocity->x;
			}

			// If the vertical movement is within the world bounds, move
			if (world.inWorld(pos->x, pos->y + velocity->y))
			{
				pos->y += velocity->y;
			}
		}
	}
};