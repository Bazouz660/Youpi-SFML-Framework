#pragma once

#include "entity.hpp"
#include <set>

namespace exng
{

	class Coordinator;

	namespace sys
	{

		class System
		{
		public:
			System(Coordinator &coordinator) : mCoordinator(coordinator) {}

			std::set<Entity> &getEntities() { return mEntities; }

		protected:
			std::set<Entity> mEntities;
			Coordinator &mCoordinator;
		};

	}
}