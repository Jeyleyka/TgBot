#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <sstream>
#include <functional>
#include <any>
#include <unordered_map>

#include <tgbot/tgbot.h>

using namespace TgBot;

class WinBot
{
private:
	std::string token;
	TgBot::Bot bot;

	std::unordered_map<std::string, std::any> setUpCommands() noexcept;
	std::unordered_map<std::string, std::any> setUpCallbacks() noexcept;

	void onInfo(TgBot::Message::Ptr message);
	void onCommandStart(TgBot::Message::Ptr message);
	void onCommandKeybord(TgBot::Message::Ptr message);

public:
	WinBot(const std::string& token);
	void run();
};

