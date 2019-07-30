// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "CommandParser.h"

#include "character/Player.h"
#include "item/Item.h"
#include "map/GameMapManager.h"
#include "ui/notifications/Notifications.h"
#include "util/StringUtil.h"
#include "util/Logger.h"

#define DEFAULT_ERR_MSG "unable to parse this line"

using std::string;
using std::vector;

namespace vigilante {

CommandParser::CmdTable CommandParser::_cmdTable = {
  {"startquest", &CommandParser::startQuest},
  {"additem",    &CommandParser::addItem   },
  {"removeitem", &CommandParser::removeItem},
};

CommandParser::CommandParser() : _success(), _errMsg() {}


void CommandParser::parse(const string& cmd) {
  vector<string> args;
  
  if (cmd.empty() || (args = string_util::split(cmd)).empty()) {
    return;
  }

  _success = false;
  _errMsg = DEFAULT_ERR_MSG;
 
  // Execute the corresponding command handler from _cmdTable.
  // The obtained value from _cmdTable is a class member function pointer.
  CmdTable::iterator it = _cmdTable.find(args[0]);
  if (it != _cmdTable.end()) {
    (this->*((*it).second))(args);
  }

  if (!_success) {
    _errMsg = args[0] + ": " + _errMsg;
    VGLOG(LOG_ERR, "%s", _errMsg.c_str());
  }
  Notifications::getInstance()->show((_success) ? cmd : _errMsg);
}

void CommandParser::setSuccess() {
  _success = true;
}

void CommandParser::setError(const string& errMsg) {
  _success = false;
  _errMsg = errMsg;
}



void CommandParser::startQuest(const vector<string>& args) {
  if (args.size() < 2) {
    setError("missing parameter `quest`");
    return;
  }

  GameMapManager::getInstance()->getPlayer()->getQuestBook().startQuest(args[1]);
  setSuccess();
}


void CommandParser::addItem(const vector<string>& args) {
  if (args.size() < 2) {
    setError("missing parameter `itemName`");
    return;
  }

  int amount = 1;
  if (args.size() >= 3) {
    try {
      amount = std::stoi(args[2]);
    } catch (const std::invalid_argument& ex) {
      setError("invalid argument `amount`");
      return;
    } catch (const std::out_of_range& ex) {
      setError("`amount` is too large");
      return;
    } catch (...) {
      setError("unknown error");
      return;
    }
  }

  if (amount <= 0) {
    setError("`amount` has to be at least 1");
    return;
  }

  GameMapManager::getInstance()->getPlayer()->addItem(Item::create(args[1]), amount);
  setSuccess();
}


void CommandParser::removeItem(const vector<string>& args) {
  if (args.size() < 2) {
    setError("missing parameter `itemName`");
    return;
  }

  int amount = 1;
  if (args.size() >= 3) {
    try {
      amount = std::stoi(args[2]);
    } catch (const std::invalid_argument& ex) {
      setError("invalid argument `amount`");
      return;
    } catch (const std::out_of_range& ex) {
      setError("`amount` is too large");
      return;
    } catch (...) {
      setError("unknown error");
      return;
    }
  }

  if (amount <= 0) {
    setError("`amount` has to be at least 1");
    return;
  }

  GameMapManager::getInstance()->getPlayer()->removeItem(Item::create(args[1]), amount);
  setSuccess();
}

} // namespace vigilante
