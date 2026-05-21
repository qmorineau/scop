#include "InputHandler.hpp"
#include "Application.hpp"
#include "commands.hpp"

InputHandler::InputHandler()
{
	_eventMouseCommand[CMD_MOUSE_MOVE] = std::make_unique<Commands::MouseMove>();
	_eventMouseCommand[CMD_MOUSE_SCROLL] = std::make_unique<Commands::MouseScroll>();	
	initDefaultMod();
	initLightEditorMod();
}

InputHandler::~InputHandler() = default;

void InputHandler::initDefaultMod()
{
	using IK = InputKey;
	auto& event = _commands[InputMode::Default].event;
	event[IK{GLFW_KEY_ESCAPE, 0}] = std::make_unique<Commands::CloseWindow>();
	event[IK{GLFW_KEY_L, 0}] = std::make_unique<Commands::ToggleLightMode>();
	event[IK{GLFW_KEY_1, 0}] = std::make_unique<Commands::PhongMode>();
    event[IK{GLFW_KEY_2, 0}] = std::make_unique<Commands::FaceMode>();
    event[IK{GLFW_KEY_3, 0}] = std::make_unique<Commands::MaterialMode>();
    event[IK{GLFW_KEY_T, 0}] = std::make_unique<Commands::TextureMode>();
    event[IK{GLFW_KEY_SPACE, 0}] = std::make_unique<Commands::ResetModel>();
    event[IK{GLFW_KEY_X, 0}] = std::make_unique<Commands::RotateX>();
    event[IK{GLFW_KEY_Y, 0}] = std::make_unique<Commands::RotateY>();
    event[IK{GLFW_KEY_Z, 0}] = std::make_unique<Commands::RotateZ>();
    event[IK{GLFW_KEY_R, 0}] = std::make_unique<Commands::ResetCamera>();
    event[IK{GLFW_KEY_C, 0}] = std::make_unique<Commands::ChangeRendering>();
    event[IK{GLFW_KEY_M, 0}] = std::make_unique<Commands::ChangeCameraMode>();

	// Repeat key
	auto& continuous = _commands[InputMode::Default].continuous;
	continuous[IK{GLFW_KEY_LEFT, 0}] = std::make_unique<Commands::Translate>(vec3(-1, 0, 0));
    continuous[IK{GLFW_KEY_RIGHT, 0}] = std::make_unique<Commands::Translate>(vec3(1, 0, 0));
    continuous[IK{GLFW_KEY_DOWN, 0}] = std::make_unique<Commands::Translate>(vec3(0, 0, -1));
	continuous[IK{GLFW_KEY_UP, 0}] = std::make_unique<Commands::Translate>(vec3(0, 0, 1));
    continuous[IK{GLFW_KEY_PAGE_UP, 0}] = std::make_unique<Commands::Translate>(vec3(0, 1, 0));
    continuous[IK{GLFW_KEY_PAGE_DOWN, 0}] = std::make_unique<Commands::Translate>(vec3(0, -1, 0));
	continuous[IK{GLFW_KEY_W, 0}] = std::make_unique<Commands::CameraForward>();
	continuous[IK{GLFW_KEY_S, 0}] = std::make_unique<Commands::CameraBackward>();
	continuous[IK{GLFW_KEY_A, 0}] = std::make_unique<Commands::CameraLeft>();
	continuous[IK{GLFW_KEY_D, 0}] = std::make_unique<Commands::CameraRight>();
	continuous[IK{GLFW_KEY_LEFT_ALT, 0}] = std::make_unique<Commands::ScaleDown>();
	continuous[IK{GLFW_KEY_RIGHT_ALT, 0}] = std::make_unique<Commands::ScaleUp>();
};

void InputHandler::initLightEditorMod()
{
	using IK = InputKey;
	auto& event = _commands[InputMode::LightEditor].event;
	event[IK{GLFW_KEY_1, 0}] = std::make_unique<Commands::SetLightColor>(vec3(1, 1, 1));
    event[IK{GLFW_KEY_2, 0}] = std::make_unique<Commands::SetLightColor>(vec3(0, 0, 0));
    event[IK{GLFW_KEY_3, 0}] = std::make_unique<Commands::SetLightColor>(vec3(1, 0, 0));
    event[IK{GLFW_KEY_4, 0}] = std::make_unique<Commands::SetLightColor>(vec3(0, 1, 0));
    event[IK{GLFW_KEY_5, 0}] = std::make_unique<Commands::SetLightColor>(vec3(0, 0, 1));
    event[IK{GLFW_KEY_6, 0}] = std::make_unique<Commands::SetLightColor>(vec3(1, 1, 0));
    event[IK{GLFW_KEY_7, 0}] = std::make_unique<Commands::SetLightColor>(vec3(1, 0, 1));
    event[IK{GLFW_KEY_8, 0}] = std::make_unique<Commands::SetLightColor>(vec3(0, 1, 1));
	event[IK{GLFW_KEY_R, 0}] = std::make_unique<Commands::EditRedChanel>();
	event[IK{GLFW_KEY_G, 0}] = std::make_unique<Commands::EditGreenChanel>();
	event[IK{GLFW_KEY_B, 0}] = std::make_unique<Commands::EditBlueChanel>();
	event[IK{GLFW_KEY_N, 0}] = std::make_unique<Commands::AddLight>();
	event[IK{GLFW_KEY_E, 0}] = std::make_unique<Commands::DeleteLight>();
	event[IK{GLFW_KEY_LEFT, 0}] = std::make_unique<Commands::PreviousLight>();
	event[IK{GLFW_KEY_RIGHT, 0}] = std::make_unique<Commands::NextLight>();
	
	// Repeat key
    auto& continuous = _commands[InputMode::LightEditor].continuous;
    continuous[IK{GLFW_KEY_DOWN, 0}] = std::make_unique<Commands::DecreaseIntensity>();
    continuous[IK{GLFW_KEY_UP, 0}] = std::make_unique<Commands::IncreaseIntensity>();
	continuous[IK{GLFW_KEY_9, 0}] = std::make_unique<Commands::DecreaseColorChanel>();
    continuous[IK{GLFW_KEY_0, 0}] = std::make_unique<Commands::IncreaseColorChanel>();
};

void InputHandler::handleKeysCallback(Application* app, int key)
{
    bool isLightEditor = app->isLightEditor();
    InputMode mode = isLightEditor ? InputMode::LightEditor : InputMode::Default;

	InputKey inputKey = {key, app->inputContext().getMods()};
    if (mode != InputMode::Default)
    {
		if (executeCommand(app, _commands[mode].event, inputKey))
			return;
    }
	executeCommand(app, _commands[InputMode::Default].event, inputKey);
};

void InputHandler::handleMouseCallback(Application* app, int key)
{
	auto iterator = _eventMouseCommand.find(key);
	if (iterator == _eventMouseCommand.end())
		return;
	iterator->second->execute(app);
}

void InputHandler::handleKeys(Application* app)
{
    bool isLightEditor = app->isLightEditor();
    InputMode mode = isLightEditor ? InputMode::LightEditor : InputMode::Default;
    InputContext ctx = app->inputContext();

	for (const auto& key : ctx.activeKeys())
	{
		InputKey inputKey = {key, app->inputContext().getMods()};
		if (mode != InputMode::Default)
		{
			if (!executeCommand(app, _commands[mode].continuous, inputKey))
				executeCommand(app, _commands[InputMode::Default].continuous, inputKey);
		}
		else
			executeCommand(app, _commands[InputMode::Default].continuous, inputKey);
	}
}

bool InputHandler::executeCommand(Application * app, std::unordered_map<InputKey, std::unique_ptr<ICommand>, InputKeyHash>& map, InputKey key)
{
	const auto& it = map.find(key);
	if (it != map.end())
	{
		it->second->execute(app);
		return true;
	}
	return false;
};