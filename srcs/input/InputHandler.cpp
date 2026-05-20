#include "InputHandler.hpp"
#include "Application.hpp"
#include "commands.hpp"

InputHandler::InputHandler()
{
	// Key Press
	_eventMouseCommand[CMD_MOUSE_MOVE] = std::make_unique<Commands::MouseMove>();
	_eventMouseCommand[CMD_MOUSE_SCROLL] = std::make_unique<Commands::MouseScroll>();
	_commands[InputMode::Default].event[GLFW_KEY_ESCAPE] = std::make_unique<Commands::CloseWindow>();
	_commands[InputMode::Default].event[GLFW_KEY_L] = std::make_unique<Commands::ToggleLightMode>();
	_commands[InputMode::Default].event[GLFW_KEY_1] = std::make_unique<Commands::PhongMode>();
    _commands[InputMode::Default].event[GLFW_KEY_2] = std::make_unique<Commands::FaceMode>();
    _commands[InputMode::Default].event[GLFW_KEY_3] = std::make_unique<Commands::MaterialMode>();
    _commands[InputMode::Default].event[GLFW_KEY_T] = std::make_unique<Commands::TextureMode>();
    _commands[InputMode::Default].event[GLFW_KEY_SPACE] = std::make_unique<Commands::ResetModel>();
    _commands[InputMode::Default].event[GLFW_KEY_X] = std::make_unique<Commands::RotateX>();
    _commands[InputMode::Default].event[GLFW_KEY_Y] = std::make_unique<Commands::RotateY>();
    _commands[InputMode::Default].event[GLFW_KEY_Z] = std::make_unique<Commands::RotateZ>();
    _commands[InputMode::Default].event[GLFW_KEY_R] = std::make_unique<Commands::ResetCamera>();
    _commands[InputMode::Default].event[GLFW_KEY_C] = std::make_unique<Commands::ChangeRendering>();
    _commands[InputMode::Default].event[GLFW_KEY_M] = std::make_unique<Commands::ChangeCameraMode>();
	
    _commands[InputMode::LightEditor].event[GLFW_KEY_1] = std::make_unique<Commands::SetLightColor>(vec3(1, 1, 1));
    _commands[InputMode::LightEditor].event[GLFW_KEY_2] = std::make_unique<Commands::SetLightColor>(vec3(0, 0, 0));
    _commands[InputMode::LightEditor].event[GLFW_KEY_3] = std::make_unique<Commands::SetLightColor>(vec3(1, 0, 0));
    _commands[InputMode::LightEditor].event[GLFW_KEY_4] = std::make_unique<Commands::SetLightColor>(vec3(0, 1, 0));
    _commands[InputMode::LightEditor].event[GLFW_KEY_5] = std::make_unique<Commands::SetLightColor>(vec3(0, 0, 1));
    _commands[InputMode::LightEditor].event[GLFW_KEY_6] = std::make_unique<Commands::SetLightColor>(vec3(1, 1, 0));
    _commands[InputMode::LightEditor].event[GLFW_KEY_7] = std::make_unique<Commands::SetLightColor>(vec3(1, 0, 1));
    _commands[InputMode::LightEditor].event[GLFW_KEY_8] = std::make_unique<Commands::SetLightColor>(vec3(0, 1, 1));
	_commands[InputMode::LightEditor].event[GLFW_KEY_R] = std::make_unique<Commands::EditRedChanel>();
	_commands[InputMode::LightEditor].event[GLFW_KEY_G] = std::make_unique<Commands::EditGreenChanel>();
	_commands[InputMode::LightEditor].event[GLFW_KEY_B] = std::make_unique<Commands::EditBlueChanel>();
	_commands[InputMode::LightEditor].event[GLFW_KEY_N] = std::make_unique<Commands::AddLight>();
	_commands[InputMode::LightEditor].event[GLFW_KEY_E] = std::make_unique<Commands::DeleteLight>();
	_commands[InputMode::LightEditor].event[GLFW_KEY_LEFT] = std::make_unique<Commands::PreviousLight>();
	_commands[InputMode::LightEditor].event[GLFW_KEY_RIGHT] = std::make_unique<Commands::NextLight>();
	
	// Repeat Key
    _commands[InputMode::Default].continuous[GLFW_KEY_LEFT] = std::make_unique<Commands::Translate>(vec3(-1, 0, 0));
    _commands[InputMode::Default].continuous[GLFW_KEY_RIGHT] = std::make_unique<Commands::Translate>(vec3(1, 0, 0));
    _commands[InputMode::Default].continuous[GLFW_KEY_UP] = std::make_unique<Commands::Translate>(vec3(0, 0, -1));
	_commands[InputMode::Default].continuous[GLFW_KEY_DOWN] = std::make_unique<Commands::Translate>(vec3(0, 0, 1));
    _commands[InputMode::Default].continuous[GLFW_KEY_PAGE_UP] = std::make_unique<Commands::Translate>(vec3(0, 1, 0));
    _commands[InputMode::Default].continuous[GLFW_KEY_PAGE_DOWN] = std::make_unique<Commands::Translate>(vec3(0, -1, 0));
	_commands[InputMode::Default].continuous[GLFW_KEY_W] = std::make_unique<Commands::CameraForward>();
	_commands[InputMode::Default].continuous[GLFW_KEY_S] = std::make_unique<Commands::CameraBackward>();
	_commands[InputMode::Default].continuous[GLFW_KEY_A] = std::make_unique<Commands::CameraLeft>();
	_commands[InputMode::Default].continuous[GLFW_KEY_D] = std::make_unique<Commands::CameraRight>();
	_commands[InputMode::Default].continuous[GLFW_KEY_LEFT_ALT] = std::make_unique<Commands::ScaleDown>();
	_commands[InputMode::Default].continuous[GLFW_KEY_RIGHT_ALT] = std::make_unique<Commands::ScaleUp>();
    _commands[InputMode::LightEditor].continuous[GLFW_KEY_DOWN] = std::make_unique<Commands::DecreaseIntensity>();
    _commands[InputMode::LightEditor].continuous[GLFW_KEY_UP] = std::make_unique<Commands::IncreaseIntensity>();
	_commands[InputMode::LightEditor].continuous[GLFW_KEY_9] = std::make_unique<Commands::DecreaseColorChanel>();
    _commands[InputMode::LightEditor].continuous[GLFW_KEY_0] = std::make_unique<Commands::IncreaseColorChanel>();
}

InputHandler::~InputHandler() {};

void InputHandler::handleKeysCallback(Application* app, int key)
{
    bool isLightEditor = app->isLightEditor();
    InputMode mode = isLightEditor ? InputMode::LightEditor : InputMode::Default;

    if (mode != InputMode::Default)
    {
		if (executeCommand(app, _commands[mode].event, key))
			return;
    }
	executeCommand(app, _commands[InputMode::Default].event, key);
};

void InputHandler::handleMouseCallback(Application* app, int key)
{
	auto iterator = _eventMouseCommand.find(key);
	if (iterator == _eventMouseCommand.end())
		return;
	iterator->second->execute(app);
}
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
void InputHandler::handleKeys(Application* app)
{
    bool isLightEditor = app->isLightEditor();
    InputMode mode = isLightEditor ? InputMode::LightEditor : InputMode::Default;
    InputContext ctx = app->inputContext();

	for (auto key = 0; key < 1024; key++)
	{
		if (ctx.keys[key])
		{
			if (mode != InputMode::Default)
			{
				if (!executeCommand(app, _commands[mode].continuous, key))
					executeCommand(app, _commands[InputMode::Default].continuous, key);
			}
			else
				executeCommand(app, _commands[InputMode::Default].continuous, key);
		}
	}
}

bool InputHandler::executeCommand(Application * app, std::unordered_map<int, std::unique_ptr<ICommand>>& map, int key)
{
	const auto& it = map.find(key);
	if (it != map.end())
	{
		it->second->execute(app);
		return true;
	}
	return false;
};