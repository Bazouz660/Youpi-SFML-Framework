/*
** EPITECH PROJECT, 2024
** B-CPP-500-STG-5-2-rtype-julian.lambert
** File description:
** coordinator
*/

#ifndef EXNG_COORDINATOR_HPP
#define EXNG_COORDINATOR_HPP

#include "componentManager.hpp"
#include "entityManager.hpp"
#include "systemManager.hpp"

#include "helper/SafeDequeue.hpp"
#include "core/event/Observer.hpp"

#include <mutex>

namespace exng {

class Coordinator
{
public:
	Coordinator() = default;

	void init()
	{
		// Create pointers to each manager
		std::lock_guard<std::mutex> lock(m_mutex);
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}

	bool entityExists(Entity entity)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return mEntityManager->entityExists(entity);
	}

	Entity createEntity()
	{
        std::lock_guard<std::mutex> lock(m_mutex);
		auto entity = mEntityManager->createEntity();
		m_entities.push_back(entity);
		return entity;
	}

	void destroyEntity(Entity entity)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		mEntityManager->destroyEntity(entity);

		mComponentManager->entityDestroyed(entity);

		mSystemManager->entityDestroyed(entity);

		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entity), m_entities.end());
	}

	// Component methods
	template<typename T>
	void registerComponent()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		mComponentManager->registerComponent<T>();
	}

	template<typename T>
	bool isComponentRegistered()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return mComponentManager->isComponentRegistered(typeid(T).name());
	}

	template<typename T>
	T& addComponent(Entity entity, T component)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		mComponentManager->addComponent<T>(entity, component);

		auto signature = mEntityManager->getSignature(entity);
		signature.set(mComponentManager->getComponentType<T>(), true);
		mEntityManager->setSignature(entity, signature);

		mSystemManager->entitySignatureChanged(entity, signature);
		return mComponentManager->getComponent<T>(entity);
	}

	template<typename T>
	void removeComponent(Entity entity)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		mComponentManager->removeComponent<T>(entity);

		auto signature = mEntityManager->getSignature(entity);
		signature.set(mComponentManager->getComponentType<T>(), false);
		mEntityManager->setSignature(entity, signature);

		mSystemManager->entitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& getComponent(Entity entity)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return mComponentManager->getComponent<T>(entity);
	}

	template<typename T>
	ComponentType getComponentType()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return mComponentManager->getComponentType<T>();
	}

	template<typename T>
	bool hasComponent(Entity entity)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return mComponentManager->hasComponent<T>(entity, typeid(T).name());
	}

    template <typename SystemType, typename... ComponentTypes>
    std::shared_ptr<SystemType> createSystem()
    {
		std::lock_guard<std::mutex> lock(m_mutex);
        auto system = registerSystem<SystemType>();
        Signature signature;

		(signature.set(mComponentManager->getComponentType<ComponentTypes>(), true), ...);
		setSystemSignature<SystemType>(signature);

		return system;
    }

	std::vector<Entity>& getEntities()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_entities;
	}

	const std::vector<Entity>& getEntities() const
	{
		return m_entities;
	}

	std::mutex& getMutex()
	{
		return m_mutex;
	}

protected:

	template<typename T>
	void setSystemSignature(Signature signature)
	{
		mSystemManager->setSignature<T>(signature);
	}

	template<typename T>
	std::shared_ptr<T> registerSystem()
	{
		return mSystemManager->registerSystem<T>(*this);
	}


private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<SystemManager> mSystemManager;
	std::vector<Entity> m_entities;
	std::mutex m_mutex;
};

}

#endif // !EXNG_COORDINATOR_HPP
