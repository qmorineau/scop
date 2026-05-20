#ifndef MODEL_HPP
#define MODEL_HPP

class Mesh;
class ModelState;

#include "ModelState.hpp"
#include "MeshGPU.hpp"

class Model
{
    public:
        Model(Mesh& mesh, std::string path);
        ~Model() = default;

        void update(float deltaTime);
        mat4 matrix()           {return _state.matrix();};

        void resetTranslation() {_state.resetTranslation();};
		void translate(vec3 v)  {_state.translate(v);};
		void resetRotation()    {_state.resetRotation();};
		void rotateX()          {_state.rotateX();};
		void rotateY()          {_state.rotateY();};
		void rotateZ()          {_state.rotateZ();};
        void toggleTexture()    {_state.toggleTexture();};

        // getter
        MeshGPU&    mesh() {return _meshGPU;};
        ModelState& state() {return _state;};
        
        float       getBlend() {return _state.getBlend();};
    private:
        MeshGPU     _meshGPU;
        ModelState  _state;
};

#endif