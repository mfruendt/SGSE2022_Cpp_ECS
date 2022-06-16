#pragma once

#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "ecs/Scene.hpp"
#include "ecs/SceneView.hpp"

/**
 * @brief System that handles the rendering of entities
 *
 */
struct RenderSystem
{
	/**
	 * @brief Update the system
	 *
	 * @param scene Scene that provides entities and components
	 * @param dt Delta time between two updates
	 * @param window Window used to render
	 */
	void update(Scene& scene, float dt, sf::RenderWindow& window)
	{
		// Iterate over every entity
		for (auto entity : SceneView<Position, Sprite>(scene))
		{
			auto pos = scene.Get<Position>(entity);
			auto sprite = scene.Get<Sprite>(entity);

			// Update position of the sprite and draw it
			sprite->shape.setPosition(pos->x, pos->y);
			window.draw(sprite->shape);
		}
	}
};