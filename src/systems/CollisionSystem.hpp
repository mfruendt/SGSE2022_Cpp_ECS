#pragma once

#include "World.hpp"
#include "components/Collision.hpp"
#include "components/Position.hpp"
#include "ecs/Scene.hpp"
#include "ecs/SceneView.hpp"

/**
 * @brief System that handles entity collision
 *
 */
struct CollisionSystem
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
		for (auto entity : SceneView<Position>(scene))
		{
			auto position = scene.Get<Position>(entity);

			// Iterate over every other entity
			for (auto otherEntity : SceneView<Position>(scene, GetEntityIndex(entity)))
			{
				auto otherPosition = scene.Get<Position>(otherEntity);

				// If the entities are not the same and in the same position, add a collision component
				if (entity != otherEntity)
				{
					if (position->x == otherPosition->x && position->y == otherPosition->y)
					{
						auto collision = scene.Assign<Collision>(entity);
						collision->damage = 1;
					}
				}
			}
		}
	}
};