#include "Player.h"

#include "map/GameMapManager.h"
#include "util/box2d/b2BodyBuilder.h"
#include "util/CategoryBits.h"
#include "util/Constants.h"

using std::string;
using cocos2d::Vector;
using cocos2d::Director;
using cocos2d::Repeat;
using cocos2d::RepeatForever;
using cocos2d::Animation;
using cocos2d::Animate;
using cocos2d::Action;
using cocos2d::Sprite;
using cocos2d::SpriteFrame;
using cocos2d::SpriteFrameCache;
using cocos2d::SpriteBatchNode;


namespace vigilante {

const float Player::kBaseMovingSpeed = .25f;

Player::Player(float x, float y)
    : _b2body(),
      _bodyFixture(),
      _spritesheet(),
      _sprite(),
      _currentState(State::IDLE_SHEATHED),
      _previousState(State::IDLE_SHEATHED),
      _stateTimer(),
      _isFacingRight(true),
      _isSheathed(true),
      _isSheathing(),
      _isUnsheathing(),
      _isJumping(),
      _isAttacking(),
      _isCrouching(),
      _isKilled(),
      _isSetToKill() {
  defineBody(x, y);
  defineTexture(x, y);
}

Player::~Player() {
  if (_spritesheet) {
    _spritesheet->release();
  }
}


void Player::update(float delta) {
  if (_isKilled) return;

  _previousState = _currentState;
  _currentState = getState();

  // If there's a change in character's state, update its animation accordingly.
  if (_previousState != _currentState) {
    switch(_currentState) {
      case State::RUNNING_SHEATHED:
        runAnimation(State::RUNNING_SHEATHED, true);
        break;
      case State::RUNNING_UNSHEATHED:
        runAnimation(State::RUNNING_UNSHEATHED, true);
        break;
      case State::JUMPING_SHEATHED:
        runAnimation(State::JUMPING_SHEATHED, false);
        break;
      case State::JUMPING_UNSHEATHED:
        runAnimation(State::JUMPING_UNSHEATHED, false);
        break;
      case State::FALLING_SHEATHED:
        runAnimation(State::FALLING_SHEATHED, false);
        break;
      case State::FALLING_UNSHEATHED:
        runAnimation(State::FALLING_UNSHEATHED, false);
        break;
      case State::CROUCHING_SHEATHED:
        runAnimation(State::CROUCHING_SHEATHED, false);
        break;
      case State::CROUCHING_UNSHEATHED:
        runAnimation(State::CROUCHING_UNSHEATHED, false);
        break;
      case State::ATTACKING:
        runAnimation(State::ATTACKING, false);
        break;
      case State::KILLED:
        runAnimation(State::KILLED, false);
        break;
      case State::IDLE_SHEATHED:
        runAnimation(State::IDLE_SHEATHED, true);
        break;
      case State::IDLE_UNSHEATHED: // fall through
      default:
        runAnimation(State::IDLE_UNSHEATHED, true);
        break;
    }
  }

  if (!_isFacingRight && !_sprite->isFlippedX()) {
    _sprite->setFlippedX(true);
    b2CircleShape* shape = static_cast<b2CircleShape*>(_weaponFixture->GetShape());
    shape->m_p = {-15.0f / kPPM, 0};
  } else if (_isFacingRight && _sprite->isFlippedX()) {
    _sprite->setFlippedX(false);
    b2CircleShape* shape = static_cast<b2CircleShape*>(_weaponFixture->GetShape());
    shape->m_p = {15.0f / kPPM, 0};
  }

  _stateTimer = (_currentState != _previousState) ? 0 : _stateTimer + delta;

  //cocos2d::log("[Player::update] x=%f y=%f", _b2body->GetPosition().x, _b2body->GetPosition().y);
  _sprite->setPosition(_b2body->GetPosition().x * kPPM, _b2body->GetPosition().y * kPPM + 5);
}

void Player::defineBody(float x, float y) {
  b2World* world = GameMapManager::getInstance()->getWorld();
  b2BodyBuilder bodyBuilder(world);

  _b2body = bodyBuilder.type(b2BodyType::b2_dynamicBody)
    .position(x, y, 1)
    .buildBody();

  // Create body fixture.
  // Fixture position in box2d is relative to b2body's position.
  float scaleFactor = Director::getInstance()->getContentScaleFactor();
  b2Vec2 vertices[4];
  vertices[0] = {-5 / scaleFactor,  20 / scaleFactor};
  vertices[1] = { 5 / scaleFactor,  20 / scaleFactor};
  vertices[2] = {-5 / scaleFactor, -14 / scaleFactor};
  vertices[3] = { 5 / scaleFactor, -14 / scaleFactor};

  bodyBuilder.newPolygonFixture(vertices, 4, kPPM)
    .categoryBits(category_bits::kPlayer)
    .maskBits(category_bits::kPortal | category_bits::kEnemy | category_bits::kMeleeWeapon | category_bits::kItem)
    .setUserData(this)
    .buildFixture();


  // Create feet fixture.
  b2Vec2 feetVertices[4];
  feetVertices[0] = {(-5 + 1) / scaleFactor, 0};
  feetVertices[1] = {( 5 - 1) / scaleFactor, 0};
  feetVertices[2] = {(-5 + 1) / scaleFactor, (-14 - 1) / scaleFactor};
  feetVertices[3] = {( 5 - 1) / scaleFactor, (-14 - 1) / scaleFactor};

  _feetFixture = bodyBuilder.newPolygonFixture(feetVertices, 4, kPPM)
    .categoryBits(category_bits::kFeet)
    .maskBits(category_bits::kGround | category_bits::kPlatform | category_bits::kWall)
    .setUserData(this)
    .buildFixture();


  // Create weapon fixture.
  _weaponFixture = bodyBuilder.newCircleFixture({15.0f, 0}, 15.0f, kPPM)
    .categoryBits(category_bits::kMeleeWeapon)
    .maskBits(category_bits::kEnemy | category_bits::kObject)
    .setSensor(true)
    .setUserData(this)
    .buildFixture();
}

void Player::defineTexture(float x, float y) {
  cocos2d::log("[Player] loading textures");
  SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

  string location = "Texture/Character/Player/sprites/";
  frameCache->addSpriteFramesWithFile(location + "player.plist");
  _spritesheet = SpriteBatchNode::create(location + "player.png");

  loadAnimation(State::IDLE_SHEATHED, "p_idle_sheathed", 6, 10.0f / kPPM);
  loadAnimation(State::IDLE_UNSHEATHED, "p_idle_unsheathed", 6, 10.0f / kPPM);

  loadAnimation(State::RUNNING_SHEATHED, "p_running_sheathed", 8, 10.0f / kPPM);
  loadAnimation(State::RUNNING_UNSHEATHED, "p_running_unsheathed", 8, 10.0f / kPPM);

  loadAnimation(State::JUMPING_SHEATHED, "p_jumping_sheathed", 5, 10.0f / kPPM);
  loadAnimation(State::JUMPING_UNSHEATHED, "p_jumping_unsheathed", 5, 10.0f / kPPM);

  loadAnimation(State::FALLING_SHEATHED, "p_falling_sheathed", 1, 10.0f / kPPM);
  loadAnimation(State::FALLING_UNSHEATHED, "p_falling_unsheathed", 1, 10.0f / kPPM);

  loadAnimation(State::CROUCHING_SHEATHED, "p_crouching_sheathed", 2, 10.0f / kPPM);
  loadAnimation(State::CROUCHING_UNSHEATHED, "p_crouching_unsheathed", 2, 10.0f / kPPM);

  loadAnimation(State::WEAPON_SHEATHING, "p_weapon_sheathing", 6, 10.0f / kPPM);
  loadAnimation(State::WEAPON_UNSHEATHING, "p_weapon_unsheathing", 6, 10.0f / kPPM);

  loadAnimation(State::ATTACKING, "p_attacking", 8, 12.0f / kPPM);
  loadAnimation(State::KILLED, "p_killed", 6, 24.0f / kPPM);

  // Select a frame as default look of character's sprite.
  _sprite = Sprite::createWithSpriteFrameName("p_idle_sheathed/p_idle_sheathed0.png");
  _sprite->setPosition(x * kPPM, y * kPPM + 7);
  _sprite->setScale(1.3f);

  _spritesheet->addChild(_sprite);
  _spritesheet->getTexture()->setAliasTexParameters(); // disable texture antialiasing
  GameMapManager::getInstance()->getScene()->addChild(_spritesheet, 30);

  runAnimation(State::IDLE_SHEATHED);
}

void Player::loadAnimation(State state, const string& frameName, size_t frameCount, float delay) {
  SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();

  Vector<SpriteFrame*> frames;
  for (size_t i = 0; i < frameCount; i++) {
    string name = frameName + "/" + frameName + std::to_string(i) + ".png";
    frames.pushBack(frameCache->getSpriteFrameByName(name));
  }
  _animations[state] = Animation::createWithSpriteFrames(frames, delay);
  _animations[state]->retain();
}

void Player::runAnimation(State state, bool loop) const {
  _sprite->stopAllActions();

  auto animation = Animate::create(_animations[state]);
  if (loop) {
    _sprite->runAction(RepeatForever::create(animation));
  } else {
    _sprite->runAction(Repeat::create(animation, 1));
  }
}


Player::State Player::getState() const {
  if (_isSetToKill) {
    return State::KILLED;
  } else if (_isAttacking) {
    return State::ATTACKING;
  } else if (_isSheathing) {
    return State::WEAPON_SHEATHING;
  } else if (_isUnsheathing) {
    return State::WEAPON_UNSHEATHING;
  } else if (_isJumping) {
    return (_isSheathed) ? State::JUMPING_SHEATHED : State::JUMPING_UNSHEATHED;
  } else if (_b2body->GetLinearVelocity().y < -.5f) {
    return (_isSheathed) ? State::FALLING_SHEATHED : State::FALLING_UNSHEATHED;
  } else if (_isCrouching) {
    return (_isSheathed) ? State::CROUCHING_SHEATHED : State::CROUCHING_UNSHEATHED;
  } else if (std::abs(_b2body->GetLinearVelocity().x) > .01f) {
    return (_isSheathed) ? State::RUNNING_SHEATHED : State::RUNNING_UNSHEATHED;
  } else {
    return (_isSheathed) ? State::IDLE_SHEATHED : State::IDLE_UNSHEATHED;
  }
}


void Player::moveLeft() {
  _isFacingRight = false;
  if (_b2body->GetLinearVelocity().x >= -kBaseMovingSpeed * 2) {
    _b2body->ApplyLinearImpulse({-kBaseMovingSpeed, 0}, _b2body->GetWorldCenter(), true);
  }
}

void Player::moveRight() {
  _isFacingRight = true;
  if (_b2body->GetLinearVelocity().x <= kBaseMovingSpeed * 2) {
    _b2body->ApplyLinearImpulse({kBaseMovingSpeed, 0}, _b2body->GetWorldCenter(), true);
  }
}

void Player::jump() {
  if (!_isJumping) {
    _isJumping = true;
    _b2body->ApplyLinearImpulse({0, 2.5f}, _b2body->GetWorldCenter(), true);
  }
}

void Player::crouch() {
  _isCrouching = true;
}

void Player::getUp() {
  _isCrouching = false;
}


b2Body* Player::getB2Body() const {
  return _b2body;
}

SpriteBatchNode* Player::getSpritesheet() const {
  return _spritesheet;
}

bool Player::isJumping() const {
  return _isJumping;
}

bool Player::isCrouching() const {
  return _isCrouching;
}

bool Player::isOnPlatform() const {
  return _isOnPlatform;
}


void Player::setIsJumping(bool isJumping) {
  _isJumping = isJumping;
}

void Player::setIsCrouching(bool isCrouching) {
  _isCrouching = isCrouching;
}

void Player::setIsOnPlatform(bool isOnPlatform) {
  _isOnPlatform = isOnPlatform;
}

} // namespace vigilante
