#pragma once

#include "YPI/ecs.hpp"
#include "YPI/graphics.hpp"

namespace comp {

struct Transform
{
    struct Child {
        ypi::ecs::entity entity;
        ypi::Vector2f offset = ypi::Vector2f(0.0f, 0.0f);
        bool relativePosition = true;
        bool relativeRotation = true;
        bool relativeScale = true;
    };

    void setPosition(ypi::Vector2f position) {
        mLocalPosition = position;
        mGlobalPosition = position;
    }

    void setPosition(float x, float y) {
        mLocalPosition.x = x;
        mLocalPosition.y = y;
        mGlobalPosition.x = x;
        mGlobalPosition.y = y;
    }

    ypi::Vector2f getPosition() const { return mGlobalPosition; }
    ypi::Vector2f getLocalPosition() const { return mLocalPosition; }

    void setOrigin(ypi::Vector2f origin) { mOrigin = origin; }
    void setOrigin(float x, float y) { mOrigin.x = x; mOrigin.y = y; }
    ypi::Vector2f getOrigin() const { return mOrigin; }

    void setScale(ypi::Vector2f scale) {
        mLocalScale = scale;
        mGlobalScale = scale;
    }
    void setScale(float x, float y) {
        mLocalScale.x = x;
        mLocalScale.y = y;
        mGlobalScale.x = x;
        mGlobalScale.y = y;
    }
    ypi::Vector2f getScale() const { return mGlobalScale; }
    ypi::Vector2f getLocalScale() const { return mLocalScale; }

    void setRotation(float angle) {
        mLocalRotation = angle;
        mGlobalRotation = angle;
    }
    float getRotation() const { return mGlobalRotation; }
    float getLocalRotation() const { return mLocalRotation; }

    void move(ypi::Vector2f offset) {
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

    void scale(ypi::Vector2f factor) {
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

    void addChild(ypi::ecs::entity entity, ypi::Vector2f offset, bool relativePosition, bool relativeRotation, bool relativeScale) {
        children.push_back({entity, offset, relativePosition, relativeRotation, relativeScale});
    }

    void removeChild(ypi::ecs::entity entity) {
        auto it = std::find_if(children.begin(), children.end(), [entity](auto &child) {
            return child.entity == entity;
        });
        if (it != children.end()) {
            children.erase(it);
        }
    }

    ypi::Vector2f mGlobalPosition = ypi::Vector2f(0.0f, 0.0f);
    float mGlobalRotation = 0.0f;
    ypi::Vector2f mGlobalScale = ypi::Vector2f(1.0f, 1.0f);

    private:
        ypi::Vector2f mLocalPosition = ypi::Vector2f(0.0f, 0.0f);
        ypi::Vector2f mOrigin = ypi::Vector2f(0.0f, 0.0f);
        ypi::Vector2f mLocalScale = ypi::Vector2f(1.0f, 1.0f);
        float mLocalRotation = 0.0f;

        std::vector<Child> children;
};

}
