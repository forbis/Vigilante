// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#ifndef VIGILANTE_DIALOGUE_MENU_H_
#define VIGILANTE_DIALOGUE_MENU_H_

#include <memory>

#include <cocos2d.h>
#include <2d/CCLabel.h>
#include <ui/UIImageView.h>
#include "Controllable.h"
#include "ui/dialogue/DialogueListView.h"

namespace vigilante {

class Npc;

class DialogueMenu : public Controllable {
 public:
  DialogueMenu();
  virtual ~DialogueMenu() = default;

  virtual void handleInput() override; // Controllable

  cocos2d::Layer* getLayer() const;
  DialogueListView* getDialogueListView() const;

 private:
  cocos2d::Layer* _layer;
  std::unique_ptr<DialogueListView> _dialogueListView;
};

} // namespace vigilante

#endif // VIGILANTE_DIALOGUE_MENU_H_
