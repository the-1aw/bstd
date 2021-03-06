/*
** EPITECH PROJECT, 2018
** Network
** File description:
** PathFinder
*/

#pragma once

#include <queue>

namespace bstd {
	class PathFinder {
		public:
			enum class Cardinal {
				NORT = 0,
				SOUTH,
				EAST,
				WEST,
				NORTH_EAST,
				NORTH_WEST,
				SOUTH_WEST,
				SOUTH_EAST
			};

		public:
			virtual ~PathFinder();
			PathFinder() = default;
			PathFinder(PathFinder &&) = default;
			PathFinder(const PathFinder &) = default;
			PathFinder &operator=(PathFinder &&) = default;
			PathFinder &operator=(const PathFinder &) = default;

		public:
			virtual Cardinal getNextMove() = 0;
			virtual std::queue<Cardinal> getFullPath() = 0;
	};
}
