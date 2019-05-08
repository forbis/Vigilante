#ifndef VIGILANTE_GAMEMAP_MANAGER_H_
#define VIGILANTE_GAMEMAP_MANAGER_H_

#include <string>

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#include "character/Player.h"

namespace vigilante {

class Player;

class GameMapManager {
 public:
  static GameMapManager* getInstance();
  virtual ~GameMapManager();

  void load(const std::string& mapFileName);

  b2World* getWorld() const;
  cocos2d::TMXTiledMap* getMap() const;

 private:
  static GameMapManager* _instance;
  GameMapManager(const b2Vec2& gravity);

  Player* spawnPlayer();

  static void createRectangles(b2World* world, cocos2d::TMXTiledMap* map, const std::string& layerName);
  static void createPolylines(b2World* world, cocos2d::TMXTiledMap* map, const std::string& layerName);

  b2World* _world;  
  cocos2d::TMXTiledMap* _map;
};

} // namespace vigilante

#endif // VIGILANTE_GAMEMAP_MANAGER_H_