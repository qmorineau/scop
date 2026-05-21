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
		struct InputKey
		{
			int key;
			int mod;
			bool operator==(const InputKey& k) const
			{
				return (k.key == key && k.mod == mod);
			};
		};
		struct InputKeyHash // to put in the hashtable 
		{
			size_t operator()(const InputKey& k) const {
				return k.key | (k.mod << 16);  // bits 0-15 = key, bits 16-31 = mod
			}    
		};

		struct CommandMap
		{
			std::unordered_map<InputKey, std::unique_ptr<ICommand>, InputKeyHash> event;
			std::unordered_map<InputKey, std::unique_ptr<ICommand>, InputKeyHash> continuous;
		};


		std::unordered_map<InputMode, CommandMap>			_commands;
		std::unordered_map<int, std::unique_ptr<ICommand>>	_eventMouseCommand;

		void initDefaultMod();
		void initLightEditorMod();
		bool executeCommand(Application * app, std::unordered_map<InputKey, std::unique_ptr<ICommand>, InputKeyHash>& map, InputKey key);
};

 #endif