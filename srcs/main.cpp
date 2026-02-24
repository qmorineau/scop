#include "Object.hpp"
#include "scop.hpp"

#include "Shader.hpp"

GLFWwindow* initOpenGL();

void processInput(GLFWwindow *window);

void render(GLFWwindow* window, Object& obj)
{
	Shader ourShader("texture.vs", "texture.fs");

	unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, obj._vertices.size() * sizeof(Vertex), obj._vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj._indices.size() * sizeof(unsigned int), obj._indices.data(), GL_STATIC_DRAW);

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

	// ourShader.use();

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		 // render container
        ourShader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, obj._indices.size(), GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
	}
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		Object *obj = new Object(std::string(argv[1]));
		try
		{
			obj->parse();
			obj->test();
			GLFWwindow* window = initOpenGL();
			if (window)
				render(window, *obj);
			else
				(void) argc;//error
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return (1);
		}
		return (0);
	}
	else
	{
		std::cerr << "Need a .obj file as input" << std::endl;
		return (0);
	}
}