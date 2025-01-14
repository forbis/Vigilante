// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "StatsPane.h"

#include "AssetManager.h"
#include "character/Player.h"
#include "ui/Colorscheme.h"
#include "ui/pause_menu/PauseMenu.h"

using std::string;
using cocos2d::Label;
using cocos2d::Layer;
using cocos2d::ui::Layout;
using cocos2d::ui::ImageView;
using vigilante::asset_manager::kRegularFont;
using vigilante::asset_manager::kRegularFontSize;
using vigilante::asset_manager::kStatsBg;

namespace vigilante {

const float StatsPane::_kPadLeft = 10.0f;
const float StatsPane::_kPadRight = 10.0f;
const float StatsPane::_kRowHeight = 16.0f;
const float StatsPane::_kSectionHeight = 8.0f;

StatsPane::StatsPane(PauseMenu* pauseMenu)
    : AbstractPane(pauseMenu, TableLayout::create()), // install TableLayout to base class
      _background(ImageView::create(kStatsBg)),
      _name(Label::createWithTTF("Aesophor", kRegularFont, kRegularFontSize)),
      _level(Label::createWithTTF("Level 1", kRegularFont, kRegularFontSize)), 
      _health(Label::createWithTTF("100 / 100", kRegularFont, kRegularFontSize)),
      _magicka(Label::createWithTTF("100 / 100", kRegularFont, kRegularFontSize)),
      _stamina(Label::createWithTTF("100 / 100", kRegularFont, kRegularFontSize)),
      _attackRange(Label::createWithTTF("5", kRegularFont, kRegularFontSize)),
      _attackSpeed(Label::createWithTTF("10", kRegularFont, kRegularFontSize)),
      _moveSpeed(Label::createWithTTF("100", kRegularFont, kRegularFontSize)),
      _jumpHeight(Label::createWithTTF("100", kRegularFont, kRegularFontSize)),
      _str(Label::createWithTTF("5", kRegularFont, kRegularFontSize)),
      _dex(Label::createWithTTF("5", kRegularFont, kRegularFontSize)),
      _int(Label::createWithTTF("5", kRegularFont, kRegularFontSize)),
      _luk(Label::createWithTTF("5", kRegularFont, kRegularFontSize)) {
  // AbstractPane::_layout is a cocos2d::ui::Layout,
  // but we know it's a TableLayout in StatsPane
  TableLayout* layout = dynamic_cast<TableLayout*>(_layout);

  layout->setLayoutType(Layout::Type::RELATIVE);
  layout->setAnchorPoint({0, 1}); // Make top-left (0, 0)
  layout->addChild(_background);
  layout->setTableWidth(_background->getContentSize().width);
  layout->setRowHeight(_kRowHeight);

  // Add name and level label at the top.
  layout->addChild(_name);
  layout->align(TableLayout::Alignment::LEFT)->padLeft(_kPadLeft)->padBottom(15.0f);
  _level->setTextColor(colorscheme::kRed);
  layout->addChild(_level);
  layout->align(TableLayout::Alignment::RIGHT)->padRight(_kPadRight)->padBottom(15.0f);
  layout->row(4.0f);

  addEntry("HEALTH", _health);
  addEntry("MAGICKA", _magicka);
  addEntry("STAMINA", _stamina);
  layout->row(_kSectionHeight);

  addEntry("ATTACK RANGE", _attackRange);
  addEntry("ATTACK SPEED", _attackSpeed);
  addEntry("MOVE SPEED", _moveSpeed);
  addEntry("JUMP HEIGHT", _jumpHeight);
  layout->row(_kSectionHeight);

  addEntry("STR", _str);
  addEntry("DEX", _dex);
  addEntry("INT", _int);
  addEntry("LUK", _luk);
}


void StatsPane::update() {
  Character::Profile& profile = _pauseMenu->getPlayer()->getCharacterProfile();

  _level->setString("Level " + std::to_string(profile.level));
  _health->setString(std::to_string(profile.health) + " / " + std::to_string(profile.fullHealth));
  _magicka->setString(std::to_string(profile.magicka) + " / " + std::to_string(profile.fullMagicka));
  _stamina->setString(std::to_string(profile.stamina) + " / " + std::to_string(profile.fullStamina));

  _attackRange->setString(std::to_string(profile.attackRange));
  _attackSpeed->setString(std::to_string(profile.attackTime));
  _moveSpeed->setString(std::to_string(profile.moveSpeed));
  _jumpHeight->setString(std::to_string(profile.jumpHeight));

  _str->setString(std::to_string(profile.strength));
  _dex->setString(std::to_string(profile.dexterity));
  _int->setString(std::to_string(profile.intelligence));
  _luk->setString(std::to_string(profile.luck));
}

void StatsPane::handleInput() {

}

void StatsPane::addEntry(const string& title, Label* label) const {
  TableLayout* layout = dynamic_cast<TableLayout*>(_layout);

  // Create title label and disable antialiasing.
  Label* titleLabel = Label::createWithTTF(title, kRegularFont, kRegularFontSize);
  titleLabel->setTextColor(colorscheme::kGrey);
  titleLabel->getFontAtlas()->setAliasTexParameters();
  label->getFontAtlas()->setAliasTexParameters();

  // Add title (the label on the LHS of the pane).
  layout->addChild(titleLabel);
  layout->align(TableLayout::Alignment::LEFT)->padLeft(_kPadLeft);

  // Add label (on the RHS of the pane)
  layout->addChild(label);
  layout->align(TableLayout::Alignment::RIGHT)->padRight(_kPadRight)->row();
}

} // namespace vigilante
