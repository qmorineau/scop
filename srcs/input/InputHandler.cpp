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
    _commands[InputMode::Default].event[GLFW_KEY_SPACE] = std::make_unique<Commands::ResetModel>(); // todo
    _commands[InputMode::Default].event[GLFW_KEY_X] = std::make_unique<Commands::RotateX>();
    _commands[InputMode::Default].event[GLFW_KEY_Y] = std::make_unique<Commands::RotateY>();
    _commands[InputMode::Default].event[GLFW_KEY_Z] = std::make_unique<Commands::RotateZ>();
    _commands[InputMode::Default].event[GLFW_KEY_SPACE] = std::make_unique<Commands::TransformX>(); // todo
    _commands[InputMode::Default].event[GLFW_KEY_SPACE] = std::make_unique<Commands::TransformY>(); // todo
    _commands[InputMode::Default].event[GLFW_KEY_SPACE] = std::make_unique<Commands::TransformZ>(); // todo
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
	_commands[InputMode::Default].continuous[GLFW_KEY_W] = std::make_unique<Commands::CameraForward>();
	_commands[InputMode::Default].continuous[GLFW_KEY_S] = std::make_unique<Commands::CameraBackward>();
	_commands[InputMode::Default].continuous[GLFW_KEY_A] = std::make_unique<Commands::CameraLeft>();
	_commands[InputMode::Default].continuous[GLFW_KEY_D] = std::make_unique<Commands::CameraRight>();
    _commands[InputMode::LightEditor].continuous[GLFW_KEY_DOWN] = std::make_unique<Commands::DecreaseIntensity>();
    _commands[InputMode::LightEditor].continuous[GLFW_KEY_UP] = std::make_unique<Commands::IncreaseIntensity>();
}

InputHandler::~InputHandler() {};

void InputHandler::handleKeysCallback(Application* app, int key)
{
    bool isLightEditor = app->isLightEditor();
    InputMode mode = isLightEditor ? InputMode::LightEditor : InputMode::Default;

    if (mode != InputMode::Default)
    {
        auto& map = _commands[mode].event;
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->execute(app);
            return;
        }
    }

    auto& def = _commands[InputMode::Default].event;
    auto it = def.find(key);
    if (it == def.end())
        return;
    it->second->execute(app);
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

    if (mode != InputMode::Default)
    {
        for (auto& [key, command] : _commands[mode].continuous)
        {
            if (ctx.keys[key])
                command->execute(app);
        }
        return;
    }
    for (auto& [key, command] : _commands[InputMode::Default].continuous)
    {
        if (ctx.keys[key])
            command->execute(app);
    }
}