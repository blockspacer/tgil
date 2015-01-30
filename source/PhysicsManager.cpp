#include "FancyAssert.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "PhysicsManager.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace {

const btVector3 DEFAULT_GRAVITY(0.0f, -9.8f, 0.0f);

} // namespace

PhysicsManager::PhysicsManager() {
   broadphase = UPtr<btDbvtBroadphase>(new btDbvtBroadphase);

   // Add support for ghost objects
   ghostPairCallback = UPtr<btGhostPairCallback>(new btGhostPairCallback);
   broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(ghostPairCallback.get());

   collisionConfiguration = UPtr<btCollisionConfiguration>(new btDefaultCollisionConfiguration);

   collisionDispatcher = UPtr<btCollisionDispatcher>(new btCollisionDispatcher(collisionConfiguration.get()));

   constraintSolver = UPtr<btConstraintSolver>(new btSequentialImpulseConstraintSolver);

   dynamicsWorld = UPtr<btDynamicsWorld>(new btDiscreteDynamicsWorld(collisionDispatcher.get(), broadphase.get(), constraintSolver.get(), collisionConfiguration.get()));
   dynamicsWorld->setGravity(DEFAULT_GRAVITY);
}

PhysicsManager::~PhysicsManager() {
   // Clean up in reverse order
   dynamicsWorld.reset();
   constraintSolver.reset();
   collisionDispatcher.reset();
   collisionConfiguration.reset();
   ghostPairCallback.reset();
   broadphase.reset();
}

void PhysicsManager::setDebugDrawer(btIDebugDraw *debugDrawer) {
   dynamicsWorld->setDebugDrawer(debugDrawer);
}

void PhysicsManager::debugDraw() {
   dynamicsWorld->debugDrawWorld();
}

void PhysicsManager::tick(const float dt) {
   dynamicsWorld->stepSimulation(dt, 15);
}

void PhysicsManager::addObject(PhysicsComponent &physicsComponent) {
   btCollisionObject* collisionObject = physicsComponent.getCollisionObject();
   if (!collisionObject) {
      return;
   }

   btRigidBody *rigidBody = dynamic_cast<btRigidBody*>(collisionObject);
   if (rigidBody) {
      dynamicsWorld->addRigidBody(rigidBody, physicsComponent.getCollisionGroup(), physicsComponent.getCollisionMask());
   } else {
      dynamicsWorld->addCollisionObject(collisionObject, physicsComponent.getCollisionGroup(), physicsComponent.getCollisionMask());
   }
}

void PhysicsManager::removeObject(PhysicsComponent &physicsComponent) {
   btCollisionObject* collisionObject = physicsComponent.getCollisionObject();
   if (!collisionObject) {
      return;
   }

   btRigidBody *rigidBody = dynamic_cast<btRigidBody*>(collisionObject);
   if (rigidBody) {
      dynamicsWorld->removeRigidBody(rigidBody);
   } else {
      dynamicsWorld->removeCollisionObject(collisionObject);
   }
}

btDynamicsWorld& PhysicsManager::getDynamicsWorld() const {
   return *dynamicsWorld;
}
