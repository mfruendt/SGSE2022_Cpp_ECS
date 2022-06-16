#pragma once

#include "World.hpp"
#include "components/Collision.hpp"
#include "components/Health.hpp"
#include "ecs/Scene.hpp"
#include "ecs/SceneView.hpp"

/**
 * @brief System that handles damage to entities
 *
 */
struct DamageSystem
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
		for (auto entity : SceneView<Health, Collision>(scene))
		{
			auto health = scene.Get<Health>(entity);
			auto collision = scene.Get<Collision>(entity);

			// Apply damage and remove the collision component
			health->health -= collision->damage;
			scene.Remove<Collision>(entity);
		}
	}
};