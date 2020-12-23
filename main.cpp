#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
int main(void)
{
	GLFWwindow* window;
	const char* glsl_version = "#version 130";
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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


		const std::vector<float> positions
		{
			 -50.0f, -50.0f, 0.0f, 0.0f,   0.1f,0.0f,1.0f,1.0f,1.0f, // bottom left
			  50.0f, -50.0f, 1.0f, 0.0f,   0.1f,0.0f,1.0f,1.0f,1.0f, // bottom right 
			  50.0f,  50.0f, 1.0f, 1.0f,   0.1f,0.0f,1.0f,1.0f,1.0f, // top right
			 -50.0f,  50.0f, 0.0f, 1.0f,   0.1f,0.0f,1.0f,1.0f,1.0f, // top left

			 -50.0f, 150.0f - 50.0f, 0.0f, 0.0f, 0.0f,1.0f,0.1f,1.0f,2.0f, // bottom left
			  50.0f, 150.0f - 50.0f, 1.0f, 0.0f, 0.0f,1.0f,0.1f,1.0f,2.0f,// bottom right 
			  50.0f, 150.0f + 50.0f, 1.0f, 1.0f, 0.0f,1.0f,0.1f,1.0f,2.0f,// top right
			 -50.0f, 150.0f + 50.0f, 0.0f, 1.0f, 0.0f,1.0f,0.1f,1.0f,2.0f,// top left

		};

		const std::vector<unsigned int> indicies {
			0,1,2, 2,3,0,
			4,5,6, 6,7,4,
		};
		//GLCall(glClearColor(0.4,0.4,0.4,1));
		//GLCall(glEnable(GL_BLEND));
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions.data(), positions.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(4);
		layout.Push<float>(1);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indicies.data(), indicies.size());
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		/* Loop until the user closes the window */
		float r = 0.0f;
		float increment = 0.05f;
		shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


		Texture texture1("res/textures/lena.png");
		Texture texture2("res/textures/cherno_logo.png");

		texture1.Bind(1);
		texture2.Bind(2);
		shader.setUniform1iv("u_Texture", { 0,1,2 });
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);


		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		glm::vec3 translationA{ 100,100,0 };

		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT));

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			shader.Bind();
			
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
				ImGui::SliderFloat3("trans A", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			shader.setUniform4f("u_Color", r, 0.3f, 0.5f, 1.0f);

			{
				glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
				glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 mvp = proj * view * model;
				shader.Bind();
				shader.setUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}
			
			//{
			//	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
			//	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
			//	glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
			//	glm::mat4 mvp = proj * view * model;
			//	shader.Bind();
			//	shader.setUniformMat4f("u_MVP", mvp);
			//	renderer.Draw(va, ib, shader);
			//}

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();

		}
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}