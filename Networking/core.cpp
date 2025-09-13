#include "core.h"

std::unordered_map<std::string, std::any> WinBot::setUpCommands() noexcept {
	auto commands = std::unordered_map<std::string, std::any>{};

	commands["start"] = std::bind(&WinBot::onCommandStart, this, std::placeholders::_1);
	commands["info"] = std::bind(&WinBot::onInfo, this, std::placeholders::_1);
	commands["inline"] = std::bind(&WinBot::onCommandKeybord, this, std::placeholders::_1);

	return commands;
}

std::unordered_map<std::string, std::any> WinBot::setUpCallbacks() noexcept {
	auto callbacks = std::unordered_map<std::string, std::any>{};

	callbacks["Current time"] = std::bind(&WinBot::onCurrentTime, this, std::placeholders::_1);
	callbacks["GitHub"] = std::bind(&WinBot::onGithubMenu, this, std::placeholders::_1);
	callbacks["C++"] = std::bind(&WinBot::onCommandKeybord, this, std::placeholders::_1);

	return callbacks;
}

void WinBot::onInfo(TgBot::Message::Ptr message)
{
	bot.getApi().sendMessage(message->chat->id,
		"/start  Start\n"
		"/info Ways\n"
		"/inline NaVi\n"
	);
}

void WinBot::onCommandStart(TgBot::Message::Ptr message)
{
	bot.getApi().sendMessage(
			message->chat->id,
			"Hello user, Type /info for help."
	);
}

void WinBot::onCommandKeybord(TgBot::Message::Ptr message)
{
	bot.getApi().sendMessage(
		message->chat->id,
		"Choosing action",
		false,
		0,
		keyboard
	);
}

void WinBot::onGithubMenu(TgBot::CallbackQuery::Ptr query)
{
	bot.getApi().sendMessage(
		query->message->chat->id,
		std::string("Repository:\n https://github.com/Jeyleyka/TgBot")
	);
}

void WinBot::onCurrentTime(TgBot::CallbackQuery::Ptr query)
{
	auto now = boost::asio::chrono::system_clock::now();
		std::time_t now_time = boost::asio::chrono::system_clock::to_time_t(now);

		std::tm local_tm{};
		localtime_s(&local_tm, &now_time);

		std::stringstream ss;
		ss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");

	bot.getApi().sendMessage(query->message->chat->id, std::string("Current time: ") + ss.str());
}

WinBot::WinBot(const std::string& token)
	: token(token), bot(token), keyboard(new TgBot::InlineKeyboardMarkup)
{
	// Processing all commands
	for (auto& c : setUpCommands()) {
		auto func = std::any_cast<std::_Binder<
			std::_Unforced,
			void (WinBot::*)(std::shared_ptr<TgBot::Message>),
			WinBot*,
			std::_Ph<1> const&
		>>(c.second);

		bot.getEvents().onCommand(c.first, [func](TgBot::Message::Ptr message) {
			func(message);
		});
	}

	// Processing all buttons
	bot.getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
		for (auto& c : setUpCallbacks()) {
			if (c.first == query->data) {
				auto func = std::any_cast<std::_Binder<
					std::_Unforced,
					void (WinBot::*)(std::shared_ptr<TgBot::CallbackQuery>),
					WinBot*,
					std::_Ph<1> const&
					>>(c.second);

				func(query);
				return;
			}
		}

		bot.getApi().sendMessage(query->message->chat->id, "Unknown control!!!");
	});

	
	// Creating btns
	TgBot::InlineKeyboardButton::Ptr btn1(new TgBot::InlineKeyboardButton);
	btn1->text = "Current time";
	btn1->callbackData = "Current time";

	TgBot::InlineKeyboardButton::Ptr btn2(new TgBot::InlineKeyboardButton);
	btn2->text = "GitHub";
	btn2->callbackData = "GitHub";

	TgBot::InlineKeyboardButton::Ptr btn3(new TgBot::InlineKeyboardButton);
	btn3->text = "C++";
	btn3->callbackData = "C++";

	row.push_back(btn1);
	row.push_back(btn2);
	row.push_back(btn3);

	// Add buttons to the screen
	keyboard->inlineKeyboard.push_back(row);
}

void WinBot::run() {
	try
	{
		std::cout << "Bot username: " << bot.getApi().getMe()->username << "\n";
		TgBot::TgLongPoll longPoll(bot);

		while (true) {
			std::cout << "Long poll started\n";
			longPoll.start();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "error: " << e.what() << "\n";
	}
}
