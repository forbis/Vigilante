#ifndef VIGILANTE_MAGICAL_MISSILE_H_
#define VIGILANTE_MAGICAL_MISSILE_H_

#include <string>
#include <functional>

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#include "DynamicActor.h"
#include "Skill.h"
#include "Projectile.h"

namespace vigilante {

class Character;

class MagicalMissile : public DynamicActor, public Skill, public Projectile {
 public:
  MagicalMissile(const std::string& jsonFileName, Character* user);
  virtual ~MagicalMissile() = default;

  virtual void update(float delta) override; // DynamicActor
  virtual void setPosition(float x, float y) override; // DynamicActor
  virtual void showOnMap(float x, float y) override; // DynamicActor
  virtual void removeFromMap() override; // DynamicActor

  virtual Character* getUser() const override; // Projectile
  virtual int getDamage() const override; // Projectile
  virtual void onHit(Character* target) override; // Projectile

  virtual void import(const std::string& jsonFileName) override; // Skill
  virtual void activate() override; // Skill
  virtual Skill::Profile& getSkillProfile() override; // Skill
  
 private:
  virtual void defineBody(b2BodyType bodyType, short categoryBits, short maskBits, float x, float y);
  virtual void defineTexture(const std::string& textureResPath, float x, float y);

  Skill::Profile _skillProfile;
  Character* _user;

  float _flyingSpeed;
  bool _hasActivated;
  bool _hasHit;

  cocos2d::Sprite* _launchFxSprite; // sprite of launching fx

  enum AnimationType {
    LAUNCH_FX,
    FLYING,
    ON_HIT,
    SIZE
  };
};

} // namespace vigilante

#endif // VIGILANTE_MAGICAL_MISSILE_H_