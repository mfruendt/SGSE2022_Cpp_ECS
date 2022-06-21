#include "Platform/Platform.hpp"
#include "World.hpp"
#include "components/Health.hpp"
#include "components/Position.hpp"
#include "components/Sprite.hpp"
#include "components/Velocity.hpp"
#include "ecs/Scene.hpp"
#include "ecs/Util.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/DamageSystem.hpp"
#include "systems/HealthSystem.hpp"
#include "systems/KiSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/RenderSystem.hpp"
#include <bitset>
#include <stdlib.h>
#include <vector>

/**
 * @brief Counter of components
 *
 */
int s_componentCounter = 0;

int frames = 0;

/**
 * @brief Main function of the programm
 *
 * @return int Exit code
 */
int main()
{
	// Create window
	util::Platform platform;
	sf::RenderWindow window;
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	window.create(sf::VideoMode(800.0f * screenScalingFactor, 800.0f * screenScalingFactor), "SGSE_Sim_ECS");
	window.setFramerateLimit(60);
	platform.setIcon(window.getSystemHandle());

	sf::Event event;
	sf::Clock deltaClock;

	// Create world
	World world(1 * window.getSize().x, 1 * window.getSize().y);

	Scene scene;

	// Create entities
	for (int i = 0; i < NUM_OF_ENTITIES; i++)
	{
		Position spawnPos = world.getRandomPos();
		auto id = scene.NewEntity();
		auto pos = scene.Assign<Position>(id);
		pos->x = spawnPos.x;
		pos->y = spawnPos.y;
		auto sprite = scene.Assign<Sprite>(id);
		sprite->shape.setRadius(1);
		sprite->shape.setFillColor(sf::Color::White);
		scene.Assign<Velocity>(id);
		auto health = scene.Assign<Health>(id);
		health->health = 3;
	}

	// Create used systems
	RenderSystem renderSystem;
	MovementSystem movementSystem;
	KiSystem kiSystem;
	DamageSystem damageSystem;
	HealthSystem healthSystem;
	CollisionSystem collisionSystem;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear window and get delta time
		window.clear();
		float dt = deltaClock.restart().asMilliseconds();
		// Update systems
		movementSystem.update(scene, dt, world);
		renderSystem.update(scene, dt, window);
		kiSystem.update(scene, dt);
		damageSystem.update(scene, dt);
		healthSystem.update(scene, dt);
		collisionSystem.update(scene, dt);
		// Display window and print delta time
		window.display();
		std::cout << dt << std::endl;

		if (frames++ >= 600)
		{
			window.close();
		}
	}

	return 0;
}
