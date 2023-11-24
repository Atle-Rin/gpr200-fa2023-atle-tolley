#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

int numLights = 4;

float ambientK = 0.5f;
float diffuseK = 0.67f;
float specular = 0.12f;
float shininess = 224;

float prevTime;
ew::Vec3 bgColor = ew::Vec3(0.1f);
ew::Vec3 lightPos[4] = {
	ew::Vec3(1.5f, 1.0f, 1.5f),
	ew::Vec3(1.5f, 1.0f, -1.5f),
	ew::Vec3(-1.5f, 1.0f, 1.5f),
	ew::Vec3(-1.5f, 1.0f, -1.5f)
};
ew::Vec3 lightCol[4] = {
	ew::Vec3(1.0f, 0.0f, 0.0f),
	ew::Vec3(0.0f, 1.0f, 0.0f),
	ew::Vec3(0.0f, 0.0f, 1.0f),
	ew::Vec3(1.0f, 1.0f, 0.0f)
};

ew::Camera camera;
ew::CameraController cameraController;

struct Light {
	ew::Vec3 position; //World space
	ew::Vec3 color; //RGB
};

struct Material {
	float ambientK; //Ambient coefficient (0-1)
	float diffuseK; //Diffuse coefficient (0-1)
	float specular; //Specular coefficient (0-1)
	float shininess; //Shininess
};

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

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader lighter("assets/unLit.vert", "assets/unLit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);
	Light lights[4];
	Material bricks;

	//Create cube
	ew::Mesh cubeMesh(ew::createCube(1.0f));
	ew::Mesh planeMesh(ew::createPlane(5.0f, 5.0f, 10));
	ew::Mesh sphereMesh(ew::createSphere(0.5f, 64));
	ew::Mesh cylinderMesh(ew::createCylinder(0.5f, 1.0f, 32));
	ew::Mesh lightMesh(ew::createSphere(0.1f, 16));

	//Initialize transforms
	ew::Transform cubeTransform;
	ew::Transform planeTransform;
	ew::Transform sphereTransform;
	ew::Transform cylinderTransform;
	planeTransform.position = ew::Vec3(0, -1.0, 0);
	sphereTransform.position = ew::Vec3(-1.5f, 0.0f, 0.0f);
	cylinderTransform.position = ew::Vec3(1.5f, 0.0f, 0.0f);
	ew::Transform lightTransform;

	resetCamera(camera,cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		lights[0].position = lightPos[0];
		lights[0].color = lightCol[0];
		lights[1].position = lightPos[1];
		lights[1].color = lightCol[1];
		lights[2].position = lightPos[2];
		lights[2].color = lightCol[2];
		lights[3].position = lightPos[3];
		lights[3].color = lightCol[3];

		bricks.ambientK = ambientK;
		bricks.diffuseK = diffuseK;
		bricks.specular = specular;
		bricks.shininess = shininess;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);

		//RENDER
		glClearColor(bgColor.x, bgColor.y,bgColor.z,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		shader.setVec3("_Lights[0].position", lights[0].position);
		shader.setVec3("_Lights[0].color", lights[0].color);
		shader.setVec3("_Lights[1].position", lights[1].position);
		shader.setVec3("_Lights[1].color", lights[1].color);
		shader.setVec3("_Lights[2].position", lights[2].position);
		shader.setVec3("_Lights[2].color", lights[2].color);
		shader.setVec3("_Lights[3].position", lights[3].position);
		shader.setVec3("_Lights[3].color", lights[3].color);
		shader.setInt("_NumLights", numLights);
		shader.setFloat("_DiffuseK", bricks.diffuseK);
		shader.setFloat("_AmbientK", bricks.ambientK);
		shader.setFloat("_Specular", bricks.specular);
		shader.setFloat("_Shininess", bricks.shininess);
		shader.setVec3("_CamPos", camera.position);
		

		//Draw shapes
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw();

		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw();

		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw();

		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw();

		lighter.use();
		lighter.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
		for (int i = 0; i < numLights; i++) {
			lightTransform.position = lights[i].position;
			lighter.setVec3("_Color", lights[i].color);
			lighter.setMat4("_Model", lightTransform.getModelMatrix());
			lightMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}

			ImGui::SliderInt("NumLights", &numLights, 1, 4);

			for (size_t i = 0; i < 4; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Light")) {
					ImGui::DragFloat3("Position", &lightPos[i].x, 0.05f);
					ImGui::SliderFloat3("Color", &lightCol[i].x, 0.0f, 1.0f);
				}
				ImGui::PopID();
			}

			if (ImGui::CollapsingHeader("Material")) {
				ImGui::SliderFloat("_AmbientK", &ambientK, 0.0f, 1.0f);
				ImGui::SliderFloat("_DiffuseK", &diffuseK, 0.0f, 1.0f);
				ImGui::SliderFloat("_Specular", &specular, 0.0f, 1.0f);
				ImGui::SliderFloat("_Shininess", &shininess, 2.0f, 255.0f);
			}

			ImGui::ColorEdit3("BG color", &bgColor.x);
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
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}


