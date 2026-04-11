#include "Material.hpp"
#include "Shader.hpp"

void Material::apply(Shader& shader) const
{
	shader.setFloat("material.Ns", _Ns);
	shader.setVec3("material.Ka", _Ka);
	shader.setVec3("material.Kd", _Kd);
	shader.setVec3("material.Ks", _Ks);
	shader.setVec3("material.Ke", _Ke);
	shader.setFloat("material.Ni", _Ni);
	shader.setFloat("material.d", _d);
	shader.setInt("material.illum", _illum);
	shader.setBool("material.hasTexture", _hasTexture);
	if (_hasTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _id);
		shader.linkTexture(0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _triPlanarId);
		shader.setInt("material_triPlanar", 1);
	}
};
