/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** entityManager
*/

#ifndef EXNG_ENTITYMANAGER_HPP
#define EXNG_ENTITYMANAGER_HPP

#include <array>
#include <queue>
#include <cassert>

#include "entity.hpp"
#include "component.hpp"
#include "signature.hpp"

#include "helper/info/logger.hpp"

namespace exng {

	class EntityManager
	{
	public:
		EntityManager()
		{
			// Initialize the bitset with all entity IDs as not used
			m_usedEntityIds.reset();
		}

		Entity createEntity()
		{
			if (m_livingEntityCount >= MAX_ENTITIES)
			{
				throw std::runtime_error("Too many entities exist: " + std::to_string(m_livingEntityCount) + " >= " + std::to_string(MAX_ENTITIES));
			}

			// Find an unused ID
			for (Entity id = 0; id < MAX_ENTITIES; ++id)
			{
				if (!m_usedEntityIds.test(id))
				{
					m_usedEntityIds.set(id);
					++m_livingEntityCount;
					return id;
				}
			}

			// No available IDs
			throw std::runtime_error("No available entity IDs.");
		}

		bool entityExists(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			return m_usedEntityIds.test(entity);
		}

		void destroyEntity(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Invalidate the destroyed entity's signature
			m_signatures[entity].reset();

			// Mark the entity's ID as not used
			m_usedEntityIds.reset(entity);
			--m_livingEntityCount;
		}

		void setSignature(Entity entity, Signature signature)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Put this entity's signature into the array
			m_signatures[entity] = signature;
		}

		Signature getSignature(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Get this entity's signature from the array
			return m_signatures[entity];
		}

		bool hasComponent(Entity entity, ComponentType componentType)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			return m_signatures[entity].test(componentType);
		}

	private:
		// Bitset of used entity IDs
		std::bitset<MAX_ENTITIES> m_usedEntityIds;

		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> m_signatures{};

		// Total living entities - used to keep limits on how many exist
		uint32_t m_livingEntityCount = 0;
	};

} // namespace exng

#endif //EXNG_ENTITYMANAGER_HPP