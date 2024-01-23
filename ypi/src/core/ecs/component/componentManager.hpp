#pragma once

#include "component.hpp"

namespace exng {

class ComponentManager
{
public:
	template<typename T>
	void registerComponent()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		// Add this component type to the component type map
		mComponentTypes.insert({typeName, mNextComponentType});

		// Create a ComponentArray pointer and add it to the component arrays map
		mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		// Increment the value so that the next component registered will be different
		++mNextComponentType;
	}

	template<typename T>
	ComponentType getComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		// Return this component's type - used for creating signatures
		return mComponentTypes[typeName];
	}

	template<typename T>
	void addComponent(Entity entity, T component)
	{
		// Add a component to the array for an entity
		getComponentArray<T>()->insertData(entity, component);
	}

	template<typename T>
	void removeComponent(Entity entity)
	{
		// Remove a component from the array for an entity
		getComponentArray<T>()->removeData(entity);
	}

	template<typename T>
	T& getComponent(Entity entity)
	{
		// Get a reference to a component from the array for an entity
		return getComponentArray<T>()->getData(entity);
	}

	void entityDestroyed(Entity entity)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->entityDestroyed(entity);
		}
	}

	bool isComponentRegistered(const char* typeName)
	{
		return mComponentTypes.find(typeName) != mComponentTypes.end();
	}

	template<typename T>
	bool hasComponent(Entity entity, const char* typeName)
	{
		return mComponentTypes.find(typeName) != mComponentTypes.end() && getComponentArray<T>()->hasData(entity);
	}

private:
	// Map from type string pointer to a component type
	std::unordered_map<const char*, ComponentType> mComponentTypes{};

	// Map from type string pointer to a component array
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

	// The component type to be assigned to the next registered component - starting at 0
	ComponentType mNextComponentType{};

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray()
	{
		const char* typeName = typeid(T).name();

		auto it = mComponentTypes.find(typeName);
		assert(it != mComponentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[it->first]);
	}
};

} // namespace exng