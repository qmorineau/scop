#include "GLMesh.hpp"
#include "Mesh.hpp"

GLMesh::GLMesh() {}

GLMesh::~GLMesh() {}

void GLMesh::upload(const Mesh& mesh)
{
	_indexCount = mesh._indices.size();

	glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glGenBuffers(1, &_ebo);

    glBindVertexArray(_vao);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh._vertices.size() * sizeof(Vertex), mesh._vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * sizeof(unsigned int), mesh._indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0);

	// wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GLMesh::draw() const
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
}