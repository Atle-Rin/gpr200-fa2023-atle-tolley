#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/ewMath/vec3.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <a-rt/camera.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];
float camPos[3]{ 0.0f, 0.0f, 5.0f };
float camTarget[3]{ 0.0f, 0.0f, 0.0f };
float camFOV = 60.0f;
float camOrthSize = 6.0f;
bool camProject = true;
float camNear = 0.1f;
float camFar = 100.0f;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	artLib::Camera cam;
	artLib::CameraControls camCon;
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		moveCamera(window, &cam, &camCon);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();
		cam.position = ew::Vec3(camPos[0], camPos[1], camPos[2]);
		cam.target = ew::Vec3(camTarget[0], camTarget[1], camTarget[2]);
		cam.fov = camFOV;
		cam.aspectRatio = ((float)SCREEN_WIDTH / SCREEN_HEIGHT);
		cam.orthoSize = camOrthSize;
		cam.nearPlane = camNear;
		cam.farPlane = camFar;
		cam.orthographic = camProject;
		ew::Mat4 view = cam.ViewMatrix();
		ew::Mat4 projection = cam.ProjectionMatrix();
		float posMag = ew::Magnitude(cam.position - cam.target);
		float targetMag = ew::Magnitude(ew::Cross((ew::Vec3)(0, 1, 0), ((cam.position - cam.target) / posMag)));
		shader.setVec3("_Position", camPos[0], camPos[1], camPos[2]);
		shader.setVec3("_Target", camTarget[0], camTarget[1], camTarget[2]);
		shader.setFloat("_FirstMag", posMag);
		shader.setFloat("_SecondMag", targetMag);
		shader.setFloat("_FOV", camFOV);
		shader.setFloat("_Aspect", cam.aspectRatio);
		shader.setFloat("_Ortho", camOrthSize);
		shader.setFloat("_Near", camNear);
		shader.setFloat("_Far", camFar);
		if (camProject) {
			shader.setInt("_Project", 1);
		}
		else {
			shader.setInt("_Project", 0);
		}

		//TODO: Set model matrix uniform
		shader.setMat4("_View", view);
		shader.setMat4("_Projection", projection);
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}
			ImGui::Text("Camera");
			ImGui::DragFloat3("CamPos", camPos);
			ImGui::DragFloat3("CamTarget", camTarget);
			ImGui::Checkbox("CamProjection", &camProject);
			ImGui::DragFloat("CamFOV", &camFOV);
			ImGui::DragFloat("OrthoSize", &camOrthSize);
			ImGui::DragFloat("CamNear", &camNear);
			ImGui::DragFloat("CamFar", &camFar);
			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void moveCamera(GLFWwindow* window, artLib::Camera* camera, artLib::CameraControls* controls) {

}