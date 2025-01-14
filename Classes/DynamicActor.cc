// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "DynamicActor.h"

#include <Box2D/Box2D.h>
#include "Constants.h"
#include "map/GameMapManager.h"

using std::vector;
using cocos2d::Node;

namespace vigilante {

DynamicActor::DynamicActor(size_t numAnimations, size_t numFixtures)
    : StaticActor(numAnimations),
      _body(),
      _fixtures(numFixtures) {}


void DynamicActor::removeFromMap() {
  if (!_isShownOnMap) {
    return;
  }

  _isShownOnMap = false;
  GameMapManager::getInstance()->getGameMap()->getDynamicActors().erase(this);

  // If _bodySpritesheet exists, we should remove it instead of _bodySprite.
  GameMapManager::getInstance()->getLayer()->removeChild(
    (_bodySpritesheet) ? ((Node*) _bodySpritesheet) : ((Node*) _bodySprite)
  );
  _bodySpritesheet = nullptr;
  _bodySprite = nullptr;

  _body->GetWorld()->DestroyBody(_body);
  _body = nullptr;
}

void DynamicActor::setPosition(float x, float y) {
  _body->SetTransform({x, y}, 0);
}

void DynamicActor::update(float delta) {
  // Sync the body sprite with its b2body.
  b2Vec2 b2bodyPos = _body->GetPosition();
  _bodySprite->setPosition(b2bodyPos.x * kPpm, b2bodyPos.y * kPpm);
}


b2Body* DynamicActor::getBody() const {
  return _body;
}

vector<b2Fixture*>& DynamicActor::getFixtures() {
  return _fixtures;
}

} // namespace vigilante
