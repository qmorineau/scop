#include "Model.hpp"

Model::Model(Mesh& mesh, std::string path) :
    _meshGPU(mesh, path)
{};

Model::~Model() = default;

void Model::update(float deltaTime)
{
    _state.applyRotation(deltaTime);
    _state.applyBlending(deltaTime);
};
