#include "GLMesh.hpp"

GLMesh::GLMesh(Mesh& mesh) : _mesh(mesh)
{
	upload();
}

GLMesh::~GLMesh() {}

void GLMesh::upload()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _mesh.vertices.size() * sizeof(Vertex), _mesh.vertices.data(), GL_STATIC_DRAW);

	GLsizei stride = sizeof(Vertex);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, normale));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, texture));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(3);

	for (auto& submesh : _mesh.subMeshes)
	{
		glGenBuffers(1, &submesh.ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, submesh.ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, submesh.indices.size() * sizeof(uint32_t), submesh.indices.data(), GL_STATIC_DRAW);
	}
}

void GLMesh::draw(Shader& shader) const
{
	shader.use();
	glBindVertexArray(_vao);

	for (auto& submesh : _mesh.subMeshes)
	{
		submesh.material->apply(shader);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, submesh.ebo);
		glDrawElements(GL_TRIANGLES, submesh.indices.size(), GL_UNSIGNED_INT, 0);
	}
}