#include "core.h"

std::unordered_map<std::string, std::any> WinBot::setUpCommands() noexcept {
	auto commands = std::unordered_map<std::string, std::any>{};

	commands["info"] = std::bind(&WinBot::onInfo, this, std::placeholders::_1);
	commands["start"] = std::bind(&WinBot::onCommandStart, this, std::placeholders::_1);
	commands["inline"] = std::bind(&WinBot::onCommandKeybord, this, std::placeholders::_1);

	return commands;
}

std::unordered_map<std::string, std::any> WinBot::setUpCallbacks() noexcept {
	auto callbacks = std::unordered_map<std::string, std::any>{};

	callbacks["info"] = std::bind(&WinBot::onInfo, this, std::placeholders::_1);
	callbacks["start"] = std::bind(&WinBot::onCommandStart, this, std::placeholders::_1);
	callbacks["inline"] = std::bind(&WinBot::onCommandKeybord, this, std::placeholders::_1);

	return callbacks;
}

void WinBot::onInfo(TgBot::Message::Ptr message)
{
	bot.getApi().sendMessage(message->chat->id,
		"Available commands:\n"
		"/start - greetings\n"
		"/time - current time");
}

void WinBot::onCommandStart(TgBot::Message::Ptr message)
{
	bot.getApi().sendMessage(
			message->chat->id,
			"Hello user!"
	);
}

void WinBot::onCommandKeybord(TgBot::Message::Ptr message)
{
}

WinBot::WinBot(const std::string& token)
	: token(token), bot(token)
{
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
	});


	bot.getEvents().onCommand("time", [this](TgBot::Message::Ptr message) {
		auto now = boost::asio::chrono::system_clock::now();
		std::time_t now_time = boost::asio::chrono::system_clock::to_time_t(now);

		std::tm local_tm{};
		localtime_s(&local_tm, &now_time);

		std::stringstream ss;
		ss << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S");

		bot.getApi().sendMessage(
			message->chat->id,
			"Current time: " + ss.str()
		);
	});

	bot.getEvents().onCommand("echo", [this](TgBot::Message::Ptr message) {
		bot.getApi().sendMessage(
			message->chat->id,
			message->text.erase(0, 5)
		);
	});
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
