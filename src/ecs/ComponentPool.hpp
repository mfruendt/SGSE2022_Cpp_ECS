#pragma once

#include "ecs/Util.hpp"

/**
 * @brief Pool used to store components of one type
 *
 */
struct ComponentPool
{
	/**
	 * @brief Construct a new Component Pool object
	 *
	 * @param elementsize Size of a component
	 */
	ComponentPool(size_t elementsize)
	{
		// The storage of the components is of fixed size, enough to hold a component for each entity
		elementSize = elementsize;
		components = new char[elementSize * NUM_OF_ENTITIES];
	}

	/**
	 * @brief Destroy the Component Pool object
	 *
	 */
	~ComponentPool()
	{
		delete[] components;
	}

	/**
 	* @brief Get the component at the specified index
 	*
 	* @param index Index of the component
 	* @return void* Pointer to the component
 	*/
	inline void* get(size_t index)
	{
		return components + index * elementSize;
	}

	/**
	 * @brief Array that holds the components
	 *
	 */
	char* components { nullptr };

	/**
	 * @brief Size of one component
	 *
	 */
	size_t elementSize { 0 };
};