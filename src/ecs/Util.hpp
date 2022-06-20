#pragma once

#include <bitset>

/**
 * @brief Maximum number of entities
 *
 */
#define NUM_OF_ENTITIES (500)

/**
 * @brief Create an invalid entity
 *
 */
#define INVALID_ENTITY CreateEntityId(EntityIndex(-1), 0)

/**
 * @brief Maximum number of component types
 *
 */
const int MAX_COMPONENTS = 32;

/**
 * @brief Type for entity component masks
 *
 */
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

/**
 * @brief Type for entity indexes
 *
 */
typedef unsigned int EntityIndex;

/**
 * @brief Type for entity versions
 *
 */
typedef unsigned int EntityVersion;

/**
 * @brief Type for entity IDs
 *
 */
typedef unsigned long long EntityID;

/**
 * @brief Counter of components
 *
 */
extern int s_componentCounter;

/**
 * @brief Get a new ID for a component
 *
 * @tparam T Type of the component
 * @return int ID of the component
 */
template <class T>
int GetId()
{
	static int s_componentId = s_componentCounter++;
	return s_componentId;
}

/**
 * @brief Create a new entity ID
 *
 * @param index Index of the entity
 * @param version Version of the entity
 * @return EntityID ID of the entity
 */
inline EntityID CreateEntityId(EntityIndex index, EntityVersion version)
{
	// Shift the index up 32, and put the version in the bottom
	return ((EntityID)index << 32) | ((EntityID)version);
}

/**
 * @brief Get the index of the entity
 *
 * @param id ID of the entity
 * @return EntityIndex Index of the entity
 */
inline EntityIndex GetEntityIndex(EntityID id)
{
	// Shift down 32 so we lose the version and get our index
	return id >> 32;
}

/**
 * @brief Get the Entity Version object
 *
 * @param id ID of the entity
 * @return EntityVersion of the input entity
 */
inline EntityVersion GetEntityVersion(EntityID id)
{
	// Cast to a 32 bit int to get our version number (loosing the top 32 bits)
	return (EntityVersion)id;
}

/**
 * @brief Check if the entity ID is that of a valid entity
 *
 * @param id ID that will be checked
 * @return true True if ID is valid
 * @return false False if ID is not valid
 */
inline bool IsEntityValid(EntityID id)
{
	// Check if the index is our invalid index
	return (id >> 32) != EntityIndex(-1);
}