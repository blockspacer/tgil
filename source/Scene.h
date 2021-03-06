#ifndef SCENE_H
#define SCENE_H

#include "Types.h"

#include <set>
#include <vector>

class DebugDrawer;
class GameObject;
class MenuLogicComponent;
class PhysicsManager;
class PlayerLogicComponent;
class ShaderProgram;

namespace {

const int NO_WINNER = -1;
const float TIME_TO_NEXT_LEVEL = 5.0f;

} // namespace

class GameState {
protected:
   int winner;

public:
   GameState()
      : winner(NO_WINNER) {}

   bool hasWinner() const {
      return winner != NO_WINNER;
   }

   int getWinner() const {
      return winner;
   }

   void setWinner(int winner) {
      if (!hasWinner()) {
         this->winner = winner;
      }
   }
};

struct GameObjectVectors {
   std::vector<SPtr<GameObject>> objects;
   std::vector<SPtr<GameObject>> toAdd;
   std::vector<SPtr<GameObject>> toRemove;
};

enum class MouseEvent {
   None,
   Enter,
   Exit,
   Click
};

typedef std::function<void(MenuLogicComponent &menuLogic, MouseEvent event)> ClickFunction;

struct ClickableObject {
   WPtr<GameObject> gameObject;
   ClickFunction clickFunction;

   ClickableObject(WPtr<GameObject> gameObject, ClickFunction clickFunction)
      : gameObject(gameObject), clickFunction(clickFunction) {
   }
};

class Scene : public std::enable_shared_from_this<Scene> {
protected:
   GameState gameState;
   bool ended;

   const SPtr<PhysicsManager> physicsManager;
   const UPtr<DebugDrawer> debugDrawer;

   std::set<SPtr<ShaderProgram>> shaderPrograms;

   SPtr<GameObject> sun;
   GameObjectVectors players;
   GameObjectVectors cameras;
   GameObjectVectors lights;
   GameObjectVectors objects;

   std::vector<ClickableObject> clickableObjects;

   bool ticking;

   float timeSinceStart;

   float timeSinceEnd;

   float timeUntilEnd;

   bool addToVectors(GameObjectVectors &vectors, SPtr<GameObject> object);

   bool removeFromVectors(GameObjectVectors &vectors, SPtr<GameObject> object);

   void processPendingObjects();

   void updateWinState();

   void updateAudioAttributes();

   void setWinner(int player);

public:
   Scene();

   virtual ~Scene();

   void tick(const float dt);

   const GameState& getGameState() const {
      return gameState;
   }

   SPtr<PhysicsManager> getPhysicsManager() const {
      return physicsManager;
   }

   DebugDrawer& getDebugDrawer() {
      return *debugDrawer;
   }

   SPtr<GameObject> getSun() const {
      return sun;
   }

   const std::vector<SPtr<GameObject>>& getPlayers() const {
      return players.objects;
   }

   const std::vector<SPtr<GameObject>>& getCameras() const {
      return cameras.objects;
   }

   const std::vector<SPtr<GameObject>>& getLights() const {
      return lights.objects;
   }

   const std::vector<SPtr<GameObject>>& getObjects() const {
      return objects.objects;
   }

   const std::set<SPtr<ShaderProgram>>& getShaderPrograms() const {
      return shaderPrograms;
   }

   void setSun(SPtr<GameObject> sun) {
      this->sun = sun;
   }

   void addPlayer(SPtr<GameObject> player);

   void addCamera(SPtr<GameObject> camera);

   void addLight(SPtr<GameObject> light);

   void addObject(SPtr<GameObject> object);

   void removePlayer(SPtr<GameObject> player);

   void removeCamera(SPtr<GameObject> camera);

   void removeLight(SPtr<GameObject> light);

   void removeObject(SPtr<GameObject> object);

   SPtr<GameObject> getPlayerByNumber(int playerNum) const;

   std::vector<SPtr<GameObject>> getLivingPlayers() const;

   void addClickableObject(ClickableObject clickableObject);

   void cleanUpClickableObjects();

   const std::vector<ClickableObject>& getClickableObjects() const;

   float getTimeSinceStart() const {
      return timeSinceStart;
   }

   float getTimeSinceEnd() const {
      return timeSinceEnd;
   }

   void end() {
      ended = true;
   }

   bool hasEnded() const {
      return ended;
   }

   void endIn(float seconds) {
      timeUntilEnd = seconds;
   }
};

#endif
