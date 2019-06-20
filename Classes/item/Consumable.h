#ifndef VIGILANTE_CONSUMABLE_H_
#define VIGILANTE_CONSUMABLE_H_

#include <string>

#include "item/Item.h"

namespace vigilante {

class Consumable : public Item {
 public:
  struct Profile {
    Profile(const std::string& jsonFileName);
    virtual ~Profile() = default;

    float duration; // sec

    int restoreHealth;
    int restoreMagicka;
    int restoreStamina;

    int bonusPhysicalDamage;
    int bonusMagicalDamage;

    int bonusStr;
    int bonusDex;
    int bonusInt;
    int bonusLuk;

    int bonusMoveSpeed;
    int bonusJumpHeight;
  };

  Consumable(const std::string& jsonFileName);
  virtual ~Consumable() = default;
  virtual void import (const std::string& jsonFileName) override; // Importable

  Consumable::Profile& getConsumableProfile();

 protected:
  Consumable::Profile _consumableProfile;
};

} // namespace vigilante

#endif // VIGILANTE_CONSUMABLE_H_