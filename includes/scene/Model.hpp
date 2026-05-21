#ifndef MODEL_HPP
#define MODEL_HPP

struct	Mesh;
class 	ModelState;

#include "ModelState.hpp"
#include "MeshGPU.hpp"

class Model
{
    public:
        Model(Mesh& mesh, std::string path);
        ~Model();

        void update(float deltaTime);
        mat4 matrix()           {return _state.matrix();};

        void resetModel() 		{_state.resetTranslation(); _state.resetScale();};
		void translate(vec3 v)  {_state.translate(v);};
		void resetRotation()    {_state.resetRotation();};
		void rotateX()          {_state.rotateX();};
		void rotateY()          {_state.rotateY();};
		void rotateZ()          {_state.rotateZ();};
        void toggleTexture()    {_state.toggleTexture();};

        void scaleUp()    {_state.scaleUp();};
        void scaleDown()    {_state.scaleDown();};
        // getter
        MeshGPU&    mesh() {return _meshGPU;};
        ModelState& state() {return _state;};
        
        float       getBlend() {return _state.getBlend();};
    private:
        MeshGPU     _meshGPU;
        ModelState  _state;
};

#endif