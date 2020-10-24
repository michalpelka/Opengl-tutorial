#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
	{
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[8] =
		{
			-0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f, 0.5f,
			-0.5f, 0.5f,
		};

		unsigned int indicies[] =
		{
			0,1,2,
			2,3,0
		};



		VertexArray va;
		VertexBuffer vb(positions, 8 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indicies, 6);
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		/* Loop until the user closes the window */
		float r = 0.0f;
		float increment = 0.05f;
		shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		GLCall(glBindVertexArray(0));



		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			shader.Bind();
			shader.setUniform4f("u_Color", r, 0.3f, 0.5f, 1.0f);

			va.Bind();
			ib.Bind();
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

		}
	}
	glfwTerminate();
	return 0;
}