#pragma once

#include "ecs/ComponentPool.hpp"

struct Entity
{
	EntityID id;
	ComponentMask mask;

	/**
	 * @brief Pools for component types in this scene
	 *
	 */
	std::vector<ComponentPool*> componentPools;
};