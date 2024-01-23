#pragma once

#include "ypi/src/core/ecs/entity/entity.hpp"
#include "ypi/src/core/vector2/Vector2.hpp"

namespace comp {

struct Transform
{
    struct Child {
        exng::Entity entity;
        exng::Vector2f offset = exng::Vector2f(0.0f, 0.0f);
        bool relativePosition = true;
        bool relativeRotation = true;
        bool relativeScale = true;
    };

    void setPosition(exng::Vector2f position) {
        mLocalPosition = position;
        mGlobalPosition = position;
    }
    void setPosition(float x, float y) {
        mLocalPosition.x = x;
        mLocalPosition.y = y;
        mGlobalPosition.x = x;
        mGlobalPosition.y = y;
    }
    exng::Vector2f getPosition() const { return mGlobalPosition; }
    exng::Vector2f getLocalPosition() const { return mLocalPosition; }

    void setOrigin(exng::Vector2f origin) { mOrigin = origin; }
    void setOrigin(float x, float y) { mOrigin.x = x; mOrigin.y = y; }
    exng::Vector2f getOrigin() const { return mOrigin; }

    void setScale(exng::Vector2f scale) {
        mLocalScale = scale;
        mGlobalScale = scale;
    }
    void setScale(float x, float y) {
        mLocalScale.x = x;
        mLocalScale.y = y;
        mGlobalScale.x = x;
        mGlobalScale.y = y;
    }
    exng::Vector2f getScale() const { return mGlobalScale; }
    exng::Vector2f getLocalScale() const { return mLocalScale; }

    void setRotation(float angle) {
        mLocalRotation = angle;
        mGlobalRotation = angle;
    }
    float getRotation() const { return mGlobalRotation; }
    float getLocalRotation() const { return mLocalRotation; }

    void move(exng::Vector2f offset) {
        mLocalPosition += offset;
        mGlobalPosition += offset;
    }

    void move(float offsetX, float offsetY) {
        mLocalPosition.x += offsetX;
        mLocalPosition.y += offsetY;
        mGlobalPosition.x += offsetX;
        mGlobalPosition.y += offsetY;
    }

    void rotate(float angle) {
        mLocalRotation += angle;
        mGlobalRotation += angle;
    }

    void scale(exng::Vector2f factor) {
        mLocalScale.x *= factor.x;
        mLocalScale.y *= factor.y;
        mGlobalScale.x *= factor.x;
        mGlobalScale.y *= factor.y;
    }

    void scale(float factorX, float factorY) {
        mLocalScale.x *= factorX;
        mLocalScale.y *= factorY;
        mGlobalScale.x *= factorX;
        mGlobalScale.y *= factorY;
    }

    std::vector<Child> getChildren() const { return children; }

    std::vector<Child>& getChildren() { return children; }

    void addChild(exng::Entity entity, exng::Vector2f offset, bool relativePosition, bool relativeRotation, bool relativeScale) {
        children.push_back({entity, offset, relativePosition, relativeRotation, relativeScale});
    }

    void removeChild(exng::Entity entity) {
        auto it = std::find_if(children.begin(), children.end(), [entity](auto &child) {
            return child.entity == entity;
        });
        if (it != children.end()) {
            children.erase(it);
        }
    }

    exng::Vector2f mGlobalPosition = exng::Vector2f(0.0f, 0.0f);
    float mGlobalRotation = 0.0f;
    exng::Vector2f mGlobalScale = exng::Vector2f(1.0f, 1.0f);

    private:
        exng::Vector2f mLocalPosition = exng::Vector2f(0.0f, 0.0f);
        exng::Vector2f mOrigin = exng::Vector2f(0.0f, 0.0f);
        exng::Vector2f mLocalScale = exng::Vector2f(1.0f, 1.0f);
        float mLocalRotation = 0.0f;

        std::vector<Child> children;
};

}
