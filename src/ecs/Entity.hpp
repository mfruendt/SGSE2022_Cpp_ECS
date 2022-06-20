#pragma once

#include "ecs/ComponentPool.hpp"

/**
 * @brief Entity that holds the information about each entity and the applied components
 *
 */
struct Entity
{
	/**
	 * @brief ID of the entity. Contains version and index
	 *
	 */
	EntityID id;

	/**
	 * @brief Bitmask that stores information about assigned components
	 *
	 */
	ComponentMask mask;

	/**
	 * @brief Pools for component types in this scene
	 *
	 */
	std::vector<ComponentPool*> componentPools;
};