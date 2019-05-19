#include "AbstractPane.h"

using cocos2d::ui::Layout;

namespace vigilante {

AbstractPane::AbstractPane(PauseMenu* pauseMenu)
    : _pauseMenu(pauseMenu), _layout(Layout::create()) {}

AbstractPane::AbstractPane(PauseMenu* pauseMenu, Layout* layout)
    : _pauseMenu(pauseMenu), _layout(layout) {}


bool AbstractPane::isVisible() const {
  if (_layout) {
    return _layout->isVisible();
  }
  return false;
}

void AbstractPane::setVisible(bool visible) const {
  if (_layout) {
    _layout->setVisible(visible);
  }
}


Layout* AbstractPane::getLayout() const {
  return _layout;
}

} // namespace vigilante