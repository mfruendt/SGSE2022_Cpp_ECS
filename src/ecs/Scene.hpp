#pragma once

#include "ecs/ComponentPool.hpp"
#include "ecs/Entity.hpp"
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
	//struct EntityDesc
	//{
	//	EntityID id;
	//	ComponentMask mask;
	//};

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
			Entity* entity = &entities[GetEntityIndex(newIndex)];
			EntityID newID = CreateEntityId(newIndex, GetEntityVersion(entity->id));
			entity->id = newID;
			return entity->id;
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
		Entity* entity = &entities[GetEntityIndex(id)];

		int componentId = GetId<T>();
		if (!entity->mask.test(componentId))
		{
			return nullptr;
		}

		T* component = static_cast<T*>(entity->componentPools[componentId]->get());
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
		Entity* entity = &entities[GetEntityIndex(id)];

		// Add a new component pool if this type is first used
		if (entity->componentPools.size() <= componentId)
		{
			entity->componentPools.resize(componentId + 1, nullptr);
		}
		if (entity->componentPools[componentId] == nullptr)
		{
			entity->componentPools[componentId] = new ComponentPool(sizeof(T));
		}

		// Looks up the component in the pool, and initializes it with placement new
		T* component = new (entity->componentPools[componentId]->get()) T();

		// Set the bit for this component to true and return the created component
		entity->mask.set(componentId);
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
		Entity* entity = &entities[GetEntityIndex(GetEntityIndex(id))];

		// Ensures you're not accessing an entity that has been deleted
		if (entity->id != id)
		{
			return;
		}

		int componentId = GetId<T>();
		entity->mask.reset(componentId);
	}

	/**
	 * @brief Destroy an entity
	 *
	 * @param id ID of the entity
	 */
	void DestroyEntity(EntityID id)
	{
		EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
		Entity* entity = &entities[GetEntityIndex(id)];
		entity->id = newID;
		entity->mask.reset();
		freeEntities.push_back(GetEntityIndex(id));
	}

	/**
	 * @brief All entities that are contained in this scene
	 *
	 */
	std::vector<Entity> entities;

	/**
	 * @brief Pools for component types in this scene
	 *
	 */
	//std::vector<ComponentPool*> componentPools;

	/**
	 * @brief List of free entity IDs
	 *
	 */
	std::vector<EntityIndex> freeEntities;
};