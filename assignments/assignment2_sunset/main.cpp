#include <stdio.h>
#include <math.h>
#include <a-rt/shader.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct Vertex {
	float x, y, z;
	float u, v;
};

unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

Vertex vertexes[4] = {
	//x   //y  //z  //u  //v
	{-0.75, -0.75, 0.0, 0.0, 0.0},
	{ 0.75, -0.75, 0.0, 1.0, 0.0},
	{-0.75,  0.75, 0.0, 0.0, 1.0},
	{ 0.75,  0.75, 0.0, 1.0, 1.0},
};

unsigned int indices[6] = {
	0 , 1 , 2 , //Triangle 1
	1 , 2 , 3   //Triangle 2
};


float sunColor[4] = { 1.0f, 0.5f, 0.0f, 1.0f };
float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;
float screenRes[2] = { 1.0f, 1.0f };
float topColor[3] = { 1.0, 0.0, 0.2 };
float botColor[3] = { 1.0, 0.5, 0.0 };
float sunDef[2] = { -0.3, 0.3 };

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	artLib::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	unsigned int vao = createVAO(vertexes, 4, indices, 6);

	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();

		//Set uniforms
		glUniform1f(glGetUniformLocation(shader.getMID(), "_Brightness"), triangleBrightness);
		glUniform1f(glGetUniformLocation(shader.getMID(), "_Time"), (float)glfwGetTime());
		glUniform2f(glGetUniformLocation(shader.getMID(), "_Resolution"), screenRes[0], screenRes[1]);
		glUniform4f(glGetUniformLocation(shader.getMID(), "_SunColor"), sunColor[0], sunColor[1], sunColor[2], sunColor[3]);
		glUniform3f(glGetUniformLocation(shader.getMID(), "_BGColor1"), topColor[0], topColor[1], topColor[2]);
		glUniform3f(glGetUniformLocation(shader.getMID(), "_BGColor2"), botColor[0], botColor[1], botColor[2]);
		glUniform2f(glGetUniformLocation(shader.getMID(), "_SunDef"), sunDef[0], sunDef[1]);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::ColorEdit4("Sun Color", sunColor);
			ImGui::ColorEdit3("BG Top Color", topColor);
			ImGui::ColorEdit3("BG Bottom Color", botColor);
			ImGui::DragFloat2("Sun Definition", sunDef);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

