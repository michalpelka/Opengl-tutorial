#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca (length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	//TODO : Error handling
	return id;
}
static unsigned int CreateShader(const std::string &vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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

	unsigned int vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));
	
	VertexBuffer vb(positions, 8 * sizeof(float));
	
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	IndexBuffer ib(indicies, 6);

	std::string vertexShader =
		"#version 330 core\n"
		"layout(location = 0) in vec4 position; \n"
		"\n"
		"void main()\n"
		"{\n"
		"  gl_Position = position;\n"
		"}\n";
	std::string fragmentShader =
		"#version 330 core\n"
		"uniform vec4 u_Color;\n"
		"layout(location = 0) out vec4 color; \n"
		"\n"
		"void main()\n"
		"{\n"
		"  color = u_Color;\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	GLCall(glUseProgram(shader));
	int location = glGetUniformLocation(shader, "u_Color");
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.5f, 1.0f));
	/* Loop until the user closes the window */
	float r = 0.0f;
	float increment = 0.05f;

	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));



	while (!glfwWindowShouldClose(window))
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, r, 0.3f, 0.5f, 1.0f));

	
		GLCall(glBindVertexArray(vao));
		ib.Bind();
		GLCall(glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, nullptr));
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
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}