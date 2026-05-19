#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "ICommand.hpp"
#include "Application.hpp"

namespace Commands
{
    // Window
    class CloseWindow : public ICommand {void execute(Application* app) const override;};
    // Lights
    class SetLightColor : public ICommand
    {
        public:
            SetLightColor(vec3 color) : _color(color) {};
            void execute(Application* app) const override {app->lights().changeColor(_color);}
        private:
            vec3 _color;
    };
    class EditRedChanel : public ICommand {void execute(Application* app) const override;};
    class EditGreenChanel : public ICommand {void execute(Application* app) const override;};
    class EditBlueChanel : public ICommand {void execute(Application* app) const override;};
    class AddLight : public ICommand {void execute(Application* app) const override;};
    class DeleteLight : public ICommand {void execute(Application* app) const override;};
    class ToggleLightMode : public ICommand {void execute(Application* app) const override;};
    class PreviousLight : public ICommand {void execute(Application* app) const override;};
    class NextLight : public ICommand {void execute(Application* app) const override;};
    class IncreaseIntensity : public ICommand {void execute(Application* app) const override;};
    class DecreaseIntensity : public ICommand {void execute(Application* app) const override;};
    // Render Mode
    class PhongMode : public ICommand {void execute(Application* app) const override;};
    class FaceMode : public ICommand {void execute(Application* app) const override;};
    class MaterialMode : public ICommand {void execute(Application* app) const override;};
    class TextureMode : public ICommand {void execute(Application* app) const override;};
    // Model
    class ResetModel : public ICommand {void execute(Application* app) const override;};
    // Rotate
    class RotateX : public ICommand {void execute(Application* app) const override;};
    class RotateY : public ICommand {void execute(Application* app) const override;};
    class RotateZ : public ICommand {void execute(Application* app) const override;};
    // Transform
    class TransformX : public ICommand {void execute(Application* app) const override;};
    class TransformY : public ICommand {void execute(Application* app) const override;};
    class TransformZ : public ICommand {void execute(Application* app) const override;};
    // Camera
    class ResetCamera : public ICommand {void execute(Application* app) const override;};
    class ChangeCameraMode : public ICommand {void execute(Application* app) const override;};
    class CameraForward : public ICommand {void execute(Application* app) const override;};
    class CameraBackward : public ICommand {void execute(Application* app) const override;};
    class CameraLeft : public ICommand {void execute(Application* app) const override;};
    class CameraRight : public ICommand {void execute(Application* app) const override;};
    // Mouse
    class MouseScroll : public ICommand {void execute(Application* app) const override;};
    class MouseMove : public ICommand {void execute(Application* app) const override;};
    class EnableMouse : public ICommand {void execute(Application* app) const override;};
    class DisableMouse : public ICommand {void execute(Application* app) const override;};

    class ChangeRendering : public ICommand {void execute(Application* app) const override;};
};

#endif