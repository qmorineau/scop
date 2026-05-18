#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <memory>
#include <array>
#include <unordered_map>

class ICommand;
class Application;

class InputHandler
{
	public:
		enum CommandID
		{
			CMD_MOUSE_MOVE = 9,
			CMD_MOUSE_SCROLL = 10,
		};
		InputHandler();
		~InputHandler();
		
		void handleKeysCallback(Application* app, int key);
		void handleMouseCallback(Application* app, int key);
		void handleKeys(Application* app);

	private:
		enum class InputMode
		{
			Default,
			LightEditor
		};
		struct CommandMap
		{
			std::unordered_map<int, std::unique_ptr<ICommand>> event;
			std::unordered_map<int, std::unique_ptr<ICommand>> continuous;
		};

		std::unordered_map<InputMode, CommandMap>			_commands;
		std::unordered_map<int, std::unique_ptr<ICommand>>	_eventMouseCommand;
};

 #endif