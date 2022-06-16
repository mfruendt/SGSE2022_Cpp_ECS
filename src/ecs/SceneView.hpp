#pragma once

#include "ecs/Util.hpp"

/**
 * @brief Scene view used to iterate over entities that have specified components from a scene
 *
 * @tparam ComponentTypes Components that the entities should have
 */
template <typename... ComponentTypes>
struct SceneView
{
	/**
	 * @brief Construct a new Scene View object
	 *
	 * @param scene Scene from which the entities will be gotten
	 */
	SceneView(Scene& scene) :
		scene(&scene)
	{
		start = 0;
		if (sizeof...(ComponentTypes) == 0)
		{
			all = true;
		}
		else
		{
			// Unpack the template parameters into an initializer list
			int componentIds[] = { 0, GetId<ComponentTypes>()... };
			for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
			{
				componentMask.set(componentIds[i]);
			}
		}
	}

	/**
	 * @brief Construct a new Scene View object with specified start index
	 *
	 * @param scene Scene from which the entities will be gotten
	 * @param start Start index for iteration
	 */
	SceneView(Scene& scene, EntityIndex start) :
		scene(&scene),
		start(start)
	{
		if (sizeof...(ComponentTypes) == 0)
		{
			all = true;
		}
		else
		{
			// Unpack the template parameters into an initializer list
			int componentIds[] = { 0, GetId<ComponentTypes>()... };
			for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
				componentMask.set(componentIds[i]);
		}
	}

	/**
	 * @brief Iterator that is used to iterate over the entities
	 *
	 */
	struct Iterator
	{
		/**
		 * @brief Construct a new Iterator object
		 *
		 * @param scene Scene from which the entities will be gotten
		 * @param index Start index for iteration
		 * @param mask Mask that determines which components the entities need to have
		 * @param all Flag if all entities should be iterated, no matter which components they have
		 */
		Iterator(Scene* scene, EntityIndex index, ComponentMask mask, bool all) :
			scene(scene),
			index(index),
			mask(mask),
			all(all)
		{}

		/**
		 * @brief Pointer access operator
		 *
		 * @return EntityID ID of the iterated entity
		 */
		EntityID operator*() const
		{
			return scene->entities[index].id;
		}

		/**
		 * @brief Compare this iterator with another iterator for equality
		 *
		 * @param other Iterator that will be compared
		 * @return true True if equal
		 * @return false False if not equal
		 */
		bool operator==(const Iterator& other) const
		{
			return index == other.index || index == scene->entities.size();
		}

		/**
		 * @brief Compare this iterator with another iterator for inequality
		 *
		 * @param other Iterator that will be compared
		 * @return true True if not equal
		 * @return false False if equal
		 */
		bool operator!=(const Iterator& other) const
		{
			return index != other.index && index != scene->entities.size();
		}

		/**
		 * @brief Check if this index is valid
		 *
		 * @return true True if valid
		 * @return false False if not valid
		 */
		bool ValidIndex()
		{
			return
				// It's a valid entity ID
				IsEntityValid(scene->entities[index].id) &&
				// It has the correct component mask
				(all || mask == (mask & scene->entities[index].mask));
		}

		/**
		 * @brief Increment operator
		 *
		 * @return Iterator& Incremented operator
		 */
		Iterator& operator++()
		{
			do
			{
				index++;
			} while (index < scene->entities.size() && !ValidIndex());
			return *this;
		}

		/**
		 * @brief Index of the current iterated entity
		 *
		 */
		EntityIndex index;

		/**
		 * @brief Scene from which the entities will be gotten
		 *
		 */
		Scene* scene;

		/**
		 * @brief Mask that determines which components the entities need to have
		 *
		 */
		ComponentMask mask;

		/**
		 * @brief Flag if all entities should be iterated, no matter which components they have
		 *
		 */
		bool all { false };
	};

	/**
	 * @brief Get the iterator that starts at the beginning
	 *
	 * @return const Iterator Iterator starting at the beginning
	 */
	const Iterator begin() const
	{
		int firstIndex = start;
		while (firstIndex < scene->entities.size() && (componentMask != (componentMask & scene->entities[firstIndex].mask) || !IsEntityValid(scene->entities[firstIndex].id)))
		{
			firstIndex++;
		}
		return Iterator(scene, firstIndex, componentMask, all);
	}

	/**
	 * @brief Get the iterator that starts at the end
	 *
	 * @return const Iterator Iterator starting at the end
	 */
	const Iterator end() const
	{
		return Iterator(scene, EntityIndex(scene->entities.size()), componentMask, all);
	}

	/**
	 * @brief Scene from which the entities will be gotten
	 *
	 */
	Scene* scene { nullptr };

	/**
	* @brief Mask that determines which components the entities need to have
	*
	*/
	ComponentMask componentMask;

	/**
	 * @brief Flag if all entities should be iterated, no matter which components they have
	 *
	 */
	bool all { false };

	/**
	 * @brief Start index for iteration
	 *
	 */
	EntityIndex start;
};