#include "Conversions.h"
#include "GameObject.h"
#include "GraphicsComponent.h"
#include "Mesh.h"
#include "Model.h"
#include "MeshPhysicsComponent.h"
#include "GameObjectMotionState.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btShapeHull.h>
#include <glm/glm.hpp>

MeshPhysicsComponent::MeshPhysicsComponent(GameObject &gameObject, float mass, const CollisionGroup::Group collisionGroup, const short collisionMask)
: PhysicsComponent(gameObject, collisionGroup, collisionMask) {
   SPtr<Model> model = gameObject.getGraphicsComponent().getModel();
   if (model) {
      const Mesh &mesh = model->getMesh();
      collisionShape = UPtr<btCollisionShape>(new btConvexHullShape(mesh.getVertices(), mesh.getNumVertices(), sizeof(float) * 3));

      // TODO Handle mesh simplification
   } else {
      // If there is no model, use a unit cube as the default collision shape
      collisionShape = UPtr<btCollisionShape>(new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));
   }

   collisionShape->setLocalScaling(toBt(gameObject.getScale()));

   motionState = UPtr<btMotionState>(new GameObjectMotionState(gameObject));

   btVector3 inertia(0.0f, 0.0f, 0.0f);
   collisionShape->calculateLocalInertia(mass, inertia);
   btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState.get(), collisionShape.get(), inertia);

   collisionObject = UPtr<btRigidBody>(new btRigidBody(constructionInfo));
}

MeshPhysicsComponent::~MeshPhysicsComponent() {
}
