// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "StatusBar.h"

using std::string;
using cocos2d::ui::Layout;
using cocos2d::ui::ImageView;

namespace vigilante {

StatusBar::StatusBar(const string& leftPaddingImgPath,
                     const string& rightPaddingImgPath,
                     const string& statusBarImgPath,
                     float maxLength)
    : _layout(Layout::create()),
      _leftPaddingImg(ImageView::create(leftPaddingImgPath)),
      _rightPaddingImg(ImageView::create(rightPaddingImgPath)),
      _statusBarImg(ImageView::create(statusBarImgPath)),
      _maxLength(maxLength) {
  _leftPaddingImg->setAnchorPoint({0, 0});
  _rightPaddingImg->setAnchorPoint({0, 0});
  _statusBarImg->setAnchorPoint({0, 0});
  _statusBarImg->setScaleX(_maxLength);

  _layout->setLayoutType(Layout::Type::HORIZONTAL);
  _layout->addChild(_leftPaddingImg);
  _layout->addChild(_statusBarImg);
  _layout->addChild(_rightPaddingImg);
}


void StatusBar::update(int currentVal, int fullVal) {
  _statusBarImg->setScaleX(_maxLength * currentVal / fullVal);
  _rightPaddingImg->setPositionX(_statusBarImg->getPositionX() + _maxLength * currentVal / fullVal);
}


Layout* StatusBar::getLayout() const {
  return _layout;
}

} // namespace vigilante
