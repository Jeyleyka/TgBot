#pragma once

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <sstream>
#include <functional>
#include <any>
#include <unordered_map>
#include <vector>

#include <tgbot/tgbot.h>

using namespace TgBot;

class WinBot
{
private:
	std::string token;
	TgBot::Bot bot;
	TgBot::InlineKeyboardMarkup::Ptr keyboard;
	std::vector<TgBot::InlineKeyboardButton::Ptr> row;
	

	std::unordered_map<std::string, std::any> setUpCommands() noexcept;
	std::unordered_map<std::string, std::any> setUpCallbacks() noexcept;


	void onInfo(TgBot::Message::Ptr message);
	void onCommandStart(TgBot::Message::Ptr message);
	void onCommandKeybord(TgBot::Message::Ptr message);

	void onGithubMenu(TgBot::CallbackQuery::Ptr query);
	void onCurrentTime(TgBot::CallbackQuery::Ptr query);
	//void onGithubMenu(TgBot::CallbackQuery::Ptr query);

public:
	WinBot(const std::string& token);
	void run();
};

