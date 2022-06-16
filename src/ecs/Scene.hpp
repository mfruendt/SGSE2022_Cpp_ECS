#pragma once

#include "ecs/ComponentPool.hpp"
#include "ecs/Util.hpp"
#include <vector>

/**
 * @brief Scene that holds all entities and components
 *
 */
struct Scene
{
	/**
	 * @brief Information that each entity holds
	 *
	 */
	struct EntityDesc
	{
		EntityID id;
		ComponentMask mask;
	};

	/**
	 * @brief Create a new entity
	 *
	 * @return EntityID ID of the created entity
	 */
	EntityID NewEntity()
	{
		if (!freeEntities.empty())
		{
			EntityIndex newIndex = freeEntities.back();
			freeEntities.pop_back();
			EntityID newID = CreateEntityId(newIndex, GetEntityVersion(entities[newIndex].id));
			entities[newIndex].id = newID;
			return entities[newIndex].id;
		}
		entities.push_back({ CreateEntityId(EntityIndex(entities.size()), 0), ComponentMask() });
		return entities.back().id;
	}

	/**
	 * @brief Get the specified component of the specified entity
	 *
	 * @tparam T Type of component that should be retrieved
	 * @param id ID of the entity
	 * @return T* Pointer to the component
	 */
	template <typename T>
	T* Get(EntityID id)
	{
		int componentId = GetId<T>();
		if (!entities[GetEntityIndex(id)].mask.test(componentId))
		{
			return nullptr;
		}

		T* component = static_cast<T*>(componentPools[componentId]->get(GetEntityIndex(id)));
		return component;
	}

	/**
	 * @brief Assign a component to the specified entity
	 *
	 * @tparam T Type of component that should be assigned
	 * @param id ID of the entity
	 * @return T* Pointer to the component
	 */
	template <typename T>
	T* Assign(EntityID id)
	{
		int componentId = GetId<T>();

		// Add a new component pool if this type is first used
		if (componentPools.size() <= componentId)
		{
			componentPools.resize(componentId + 1, nullptr);
		}
		if (componentPools[componentId] == nullptr)
		{
			componentPools[componentId] = new ComponentPool(sizeof(T));
		}

		// Looks up the component in the pool, and initializes it with placement new
		T* component = new (componentPools[componentId]->get(GetEntityIndex(id))) T();

		// Set the bit for this component to true and return the created component
		entities[GetEntityIndex(id)].mask.set(componentId);
		return component;
	}

	/**
	 * @brief Remove a component from the specified entity
	 *
	 * @tparam T Type of component that should be removed
	 * @param id ID of the entity
	 */
	template <typename T>
	void Remove(EntityID id)
	{
		// Ensures you're not accessing an entity that has been deleted
		if (entities[GetEntityIndex(GetEntityIndex(id))].id != id)
		{
			return;
		}

		int componentId = GetId<T>();
		entities[GetEntityIndex(GetEntityIndex(id))].mask.reset(componentId);
	}

	/**
	 * @brief Destroy an entity
	 *
	 * @param id ID of the entity
	 */
	void DestroyEntity(EntityID id)
	{
		EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
		entities[GetEntityIndex(id)].id = newID;
		entities[GetEntityIndex(id)].mask.reset();
		freeEntities.push_back(GetEntityIndex(id));
	}

	/**
	 * @brief All entities that are contained in this scene
	 *
	 */
	std::vector<EntityDesc> entities;

	/**
	 * @brief Pools for component types in this scene
	 *
	 */
	std::vector<ComponentPool*> componentPools;

	/**
	 * @brief List of free entity IDs
	 *
	 */
	std::vector<EntityIndex> freeEntities;
};