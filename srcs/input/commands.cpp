#include "commands.hpp"

#include "Application.hpp"

// Window
void Commands::CloseWindow::execute(Application* app) const
{
    app->window().closeWindow();
};

// Lights
void Commands::EditRedChanel::execute(Application* app) const
{
    app->lights().setColor(LightManager::ActiveColor::Red);
};
void Commands::EditGreenChanel::execute(Application* app) const
{
    app->lights().setColor(LightManager::ActiveColor::Green);   
};
void Commands::EditBlueChanel::execute(Application* app) const
{
    app->lights().setColor(LightManager::ActiveColor::Blue);
};
void Commands::AddLight::execute(Application* app) const
{
    app->lights().add(app->getCamera().getPosition());
};
void Commands::DeleteLight::execute(Application* app) const
{
    app->lights().remove();
};
void Commands::ToggleLightMode::execute(Application* app) const
{
    app->toggleLightEditor();
};
void Commands::PreviousLight::execute(Application* app) const
{
    Light* l = app->lights().prev();
	if (l)
		app->getCamera().changePosition(l->getPosition());
};
void Commands::NextLight::execute(Application* app) const
{
    Light* l = app->lights().next();
	if (l)
		app->getCamera().changePosition(l->getPosition());
};
void Commands::DecreaseIntensity::execute(Application* app) const
{
	app->lights().intensityDown(app->getDelta());
}
void Commands::IncreaseIntensity::execute(Application* app) const
{
	app->lights().intensityUp(app->getDelta());
}
void Commands::DecreaseColorChanel::execute(Application* app) const
{
    LightManager& manager = app->lights();
    switch (manager.getColor())
    {
        case LightManager::ActiveColor::Red:
            manager.colorDown(Color::Red, app->getDelta());
            break;
        case LightManager::ActiveColor::Green:
            manager.colorDown(Color::Green, app->getDelta());
            break;
        case LightManager::ActiveColor::Blue:
            manager.colorDown(Color::Blue, app->getDelta());
            break;
        default:
            break;
    }
};
void Commands::IncreaseColorChanel::execute(Application* app) const
{
   LightManager& manager = app->lights();
    switch (manager.getColor())
    {
        case LightManager::ActiveColor::Red:
            manager.colorUp(Color::Red, app->getDelta());
            break;
        case LightManager::ActiveColor::Green:
            manager.colorUp(Color::Green, app->getDelta());
            break;
        case LightManager::ActiveColor::Blue:
            manager.colorUp(Color::Blue, app->getDelta());
            break;
        default:
            break;
    }
};

// Render Mode
void Commands::PhongMode::execute(Application* app) const
{
    app->renderer().setMode(RenderMode::Phong);
};
void Commands::FaceMode::execute(Application* app) const
{
    app->renderer().setMode(RenderMode::Face);
};
void Commands::MaterialMode::execute(Application* app) const
{
    app->renderer().setMode(RenderMode::Material);
};
void Commands::TextureMode::execute(Application* app) const
{
    app->scene()->model().toggleTexture();
};

// Model
void Commands::ResetModel::execute(Application* app) const
{
    app->scene()->model().resetTranslation();
};

// Rotate
void Commands::RotateX::execute(Application* app) const
{
    app->scene()->model().rotateX();
};
void Commands::RotateY::execute(Application* app) const
{
	app->scene()->model().rotateY();
};
void Commands::RotateZ::execute(Application* app) const
{
	app->scene()->model().rotateZ();
};

// Camera
void Commands::ResetCamera::execute(Application* app) const
{
    app->getCamera().resetPosition();
	app->scene()->model().resetRotation();
};
void Commands::ChangeCameraMode::execute(Application* app) const
{
    app->getCamera().changeMode();
};
void Commands::CameraForward::execute(Application* app) const
{
    app->getCamera().processKeyboard(Camera::FORWARD, app->getDelta());
};
void Commands::CameraBackward::execute(Application* app) const
{
    app->getCamera().processKeyboard(Camera::BACKWARD, app->getDelta());
};
void Commands::CameraLeft::execute(Application* app) const
{
    app->getCamera().processKeyboard(Camera::LEFT, app->getDelta());
};
void Commands::CameraRight::execute(Application* app) const
{
    app->getCamera().processKeyboard(Camera::RIGHT, app->getDelta());
};

void Commands::ChangeRendering::execute(Application* app) const
{
    app->renderer().changeFaceRendering();
};

// Mouse
void Commands::MouseMove::execute(Application* app) const
{
	if (!app->window().getMouse())
	{	
		InputContext& ctx = app->inputContext();
		app->getCamera().onMouseMove(ctx.mousePos.x, ctx.mousePos.y);
	}
};

void Commands::MouseScroll::execute(Application* app) const
{
	InputContext& ctx = app->inputContext();
	app->getCamera().onMouseScroll(ctx.mouseOffset.x, ctx.mouseOffset.y);
};
void Commands::EnableMouse::execute(Application* app) const
{
	app->inputContext().isMouseCaptured = false;
	app->window().enableMouse();
};
void Commands::DisableMouse::execute(Application* app) const
{
	app->inputContext().isMouseCaptured = true;
	app->getCamera().disableMouse();
	glfwSetCursorPos(app->window().getWindow(), SCR_WIDTH / 2, SCR_HEIGHT / 2);
	app->window().disableMouse();
};