// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef VIGILANTE_SKILL_H_
#define VIGILANTE_SKILL_H_

#include <string>

#include <cocos2d.h>
#include "Importable.h"
#include "input/Keybindable.h"

namespace vigilante {

class Character;

// Skill interface
class Skill : public Importable, public Keybindable {
 public:
  struct Profile {
    explicit Profile(const std::string& jsonFileName);
    virtual ~Profile() = default;

    std::string jsonFileName;
    std::string characterFramesName;
    float framesDuration;
    float frameInterval;

    std::string textureResDir; // the animation of skill itself
    std::string name;
    std::string desc;
    
    int requiredLevel;
    float cooldown;

    int physicalDamage;
    int magicalDamage;

    int deltaHealth;
    int deltaMagicka;
    int deltaStamina;

    cocos2d::EventKeyboard::KeyCode hotkey;
  };

  // Create a skill by automatically deducing its concrete type
  // based on the json passed in.
  static Skill* create(const std::string& jsonFileName, Character* user);

  virtual ~Skill() = default;
  virtual void import(const std::string& jsonFileName) = 0; // Importable

  virtual cocos2d::EventKeyboard::KeyCode getHotkey() const = 0; // Keybindable
  virtual void setHotkey(cocos2d::EventKeyboard::KeyCode hotkey) = 0; // Keybindable

  virtual bool canActivate() = 0;
  virtual void activate() = 0;

  virtual Skill::Profile& getSkillProfile() = 0;
  virtual const std::string& getName() const = 0;
  virtual const std::string& getDesc() const = 0;
  virtual std::string getIconPath() const = 0;
};

} // namespace vigilante

#endif // VIGILANTE_SKILL_H_
