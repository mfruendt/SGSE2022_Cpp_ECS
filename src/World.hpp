#pragma once

#include "components/Position.hpp"

/**
 * @brief World in which the entities move
 *
 */
struct World
{
	/**
	 * @brief Construct a new World object
	 *
	 * @param x Horizontal size of the world
	 * @param y Vertical size of the world
	 */
	World(int x, int y) :
		sizeX(x),
		sizeY(y)
	{
	}

	/**
	 * @brief Get a new random position in the world
	 *
	 * @return Position Random position
	 */
	Position getRandomPos(void)
	{
		int spawnX = rand() % sizeX;
		int spawnY = rand() % sizeY;

		Position position;
		position.x = spawnX;
		position.y = spawnY;

		return position;
	}

	/**
	 * @brief Check if the input position is within the bounds of the world
	 *
	 * @param pos Position that will be checked
	 * @return true True if the position is within the bounds of the world
	 * @return false False if the position is not within the bounds of the world
	 */
	bool inWorld(Position pos)
	{
		return pos.x >= 0 && pos.x < sizeX && pos.y >= 0 && pos.y < sizeY;
	}

	/**
	 * @brief Check if the input position is within the bounds of the world
	 *
	 * @param x Horizontal component of the position that will be checked
	 * @param y Vertical component of the position that will be checked
	 * @return true True if the position is within the bounds of the world
	 * @return false False if the position is not within the bounds of the world
	 */
	bool inWorld(int x, int y)
	{
		return x >= 0 && x < sizeX && y >= 0 && y < sizeY;
	}

	/**
	 * @brief Horizontal size of the world
	 *
	 */
	int sizeX;

	/**
	 * @brief Vertical size of the world
	 *
	 */
	int sizeY;
};