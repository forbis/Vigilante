// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "PauseMenuDialog.h"

#include "AssetManager.h"
#include "input/InputManager.h"
#include "ui/TableLayout.h"

using std::string;
using std::unique_ptr;
using std::function;
using cocos2d::Label;
using cocos2d::ui::Layout;
using cocos2d::ui::ImageView;
using cocos2d::EventKeyboard;
using vigilante::asset_manager::kBoldFont;
using vigilante::asset_manager::kRegularFontSize;
using vigilante::asset_manager::kDialogueTriangle;

namespace vigilante {

PauseMenuDialog::PauseMenuDialog(PauseMenu* pauseMenu)
    : AbstractPane(pauseMenu, TableLayout::create()),
      _message(Label::createWithTTF("", kBoldFont, kRegularFontSize)),
      _current() {
  _message->setAnchorPoint({0, 1});
  _message->getFontAtlas()->setAliasTexParameters();
  _layout->addChild(_message);

  addOption("option1");
  addOption("option2");
  addOption("option3");
}


void PauseMenuDialog::update() {
  const auto& winSize = cocos2d::Director::getInstance()->getWinSize();
  // Automatically position the options :-)
  int count = 1;
  float optionWidth = 0;

  for (const auto& option : _options) {
    optionWidth = std::max(optionWidth, option->getWidth());
  }

  for (int i = (int) _options.size() - 1; i >= 0; i--) {
    if (!_options[i]->isVisible()) {
      continue;
    }
    Option* option = _options[i].get();
    option->getLayout()->setPositionX(winSize.width - 110 - (optionWidth + 15) * count);
    count++;
  }
}

void PauseMenuDialog::handleInput() {
  auto inputMgr = InputManager::getInstance();

  if (inputMgr->isKeyJustPressed(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
    selectLeft();
  } else if (inputMgr->isKeyJustPressed(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
    selectRight();
  } else if (inputMgr->isKeyJustPressed(EventKeyboard::KeyCode::KEY_ENTER)) {
    confirm();
  }
}

void PauseMenuDialog::selectLeft() {
  if (_current == 0 || !_options[_current - 1]->isVisible()) {
    return;
  }
  _options[_current]->setSelected(false);
  _options[--_current]->setSelected(true);
}

void PauseMenuDialog::selectRight() {
  if (_current == (int) _options.size() - 1 || !_options[_current + 1]->isVisible()) {
    return;
  }
  _options[_current]->setSelected(false);
  _options[++_current]->setSelected(true);
}

void PauseMenuDialog::confirm() {
  if (_options.size() == 0) {
    return;
  }
  setVisible(false);
  _options[0]->setSelected(true);
  _options[_current]->setSelected(false);
  _options[_current]->getHandler()(); // invokes a std::function<void ()>
}


void PauseMenuDialog::reset() {
  setMessage("");
  _options[_current]->setSelected(false);
  for (auto& option : _options) {
    option->setVisible(false);
  }
}

void PauseMenuDialog::setMessage(const string& message) const {
  _message->setString(message);
}

void PauseMenuDialog::setOption(int index, bool visible, const string& text, const function<void ()>& handler) const {
  if (index < 0 || index >= 3) {
    return;
  }
  _options[index]->setVisible(visible);
  _options[index]->setText(text);
  _options[index]->setHandler(handler);
}

void PauseMenuDialog::show() {
  update();
  setVisible(true);

  // Set the first visible option as selected.
  for (int i = 0; i < (int) _options.size(); i++) {
    if (_options[i]->isVisible()) {
      _options[i]->setSelected(true);
      _current = i;
      break;
    }
  }
}


void PauseMenuDialog::addOption(const string& text, const function<void ()>& handler) {
  _options.push_back(unique_ptr<Option>(new Option(text, handler)));
  _layout->addChild(_options.back()->getLayout());
  update();
  _options.front()->setSelected(true);
}

void PauseMenuDialog::clearOptions() {
  for (const auto& option : _options) {
    _layout->removeChild(option->getLayout());
  }
}


PauseMenuDialog::Option::Option(const string& text, const function<void ()>& handler)
    : _layout(Layout::create()),
      _icon(ImageView::create(kDialogueTriangle)),
      _label(Label::createWithTTF(text, kBoldFont, kRegularFontSize)),
      _handler(handler) {
  _icon->setAnchorPoint({0, 1});
  _icon->setVisible(false);

  _label->setAnchorPoint({0, 1});
  _label->getFontAtlas()->setAliasTexParameters();
  _label->setPositionX(_icon->getContentSize().width + 5); // 5 is gap between icon and label

  _layout->setAnchorPoint({0, 1});
  _layout->addChild(_icon);
  _layout->addChild(_label);
}

float PauseMenuDialog::Option::getWidth() const {
  return _icon->getContentSize().width + _label->getContentSize().width;
}

void PauseMenuDialog::Option::setSelected(bool selected) const {
  _icon->setVisible(selected);
}

bool PauseMenuDialog::Option::isVisible() const {
  return _layout->isVisible();
}

void PauseMenuDialog::Option::setVisible(bool visible) const {
  _layout->setVisible(visible);
}

Layout* PauseMenuDialog::Option::getLayout() const {
  return _layout;
}


string PauseMenuDialog::Option::getText() const {
  return _label->getString();
}

void PauseMenuDialog::Option::setText(const string& text) const {
  _label->setString(text);
}

const function<void ()>& PauseMenuDialog::Option::getHandler() const {
  return _handler;
}

void PauseMenuDialog::Option::setHandler(const function<void ()>& handler) {
  _handler = handler;
}

} // namespace vigilante
