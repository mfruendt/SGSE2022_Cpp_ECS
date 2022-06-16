#pragma once

#include "World.hpp"
#include "components/Health.hpp"
#include "ecs/Scene.hpp"
#include "ecs/SceneView.hpp"

/**
 * @brief System that the death of entities
 *
 */
struct HealthSystem
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
		for (auto entity : SceneView<Health>(scene))
		{
			auto health = scene.Get<Health>(entity);

			// Delete entity if it has no health left
			if (health->health <= 0)
			{
				scene.DestroyEntity(entity);
			}
		}
	}
};