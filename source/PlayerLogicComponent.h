#ifndef PLAYER_LOGIC_COMPONENT_H
#define PLAYER_LOGIC_COMPONENT_H

#include "LogicComponent.h"

#include <folly/Optional.h>
#include <glm/glm.hpp>

class Ability;
class btRigidBody;
class PhysicsManager;

struct Ground {
   const float y;
   const float friction;

   Ground(float y, float friction)
      : y(y), friction(friction) {
   }
};

class PlayerLogicComponent : public LogicComponent {
protected:
   bool alive;
   bool wasJumpingLastFrame;
   bool canDoubleJump;
   float jumpTimer;
   float distanceSinceStep;
   float deathTime;
   glm::vec3 color;
   SPtr<Ability> primaryAbility;
   SPtr<Ability> secondaryAbility;

   folly::Optional<Ground> getGround(SPtr<PhysicsManager> physicsManager) const;

   folly::Optional<glm::vec3> calcSpringForce(const Ground &ground, const btRigidBody *rigidBody) const;

   glm::vec3 calcMoveIntention(const InputValues &inputValues) const;

   glm::vec3 calcJumpImpulse(const InputValues &inputValues, const glm::vec3 &velocity, const glm::vec3 &horizontalMoveIntention, bool onGround);

   void handleOrientation(const float dt, const InputValues &inputValues);

   void handleMovement(const float dt, const InputValues &inputValues, SPtr<Scene> scene);

   void handleAttack(const float dt, const InputValues &inputValues, SPtr<Scene> scene);

public:
   PlayerLogicComponent(GameObject &gameObject, const glm::vec3 &color);

   virtual ~PlayerLogicComponent();

   virtual void tick(const float dt);

   const glm::vec3& getColor() const {
      return color;
   }

   bool isAlive() const {
      return alive;
   }

   void setAlive(bool alive);

   float timeSinceDeath() const;

   const Ability &getPrimaryAbility() const;

   const Ability &getSecondaryAbility() const;
};

#endif
