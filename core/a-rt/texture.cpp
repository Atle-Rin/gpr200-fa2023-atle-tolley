#include "texture.h"
#include "C:\Users\atle.tolley\GPR-200 Repo\gpr200-fa2023-atle-tolley\core\ew\external\glad.h"
#include "C:\Users\atle.tolley\GPR-200 Repo\gpr200-fa2023-atle-tolley\core\ew\external\stb_image.h"

unsigned int loadTexture(const char* filePath) {
	stbi_set_flip_vertically_on_load(true);

	int width, height, numComponents;
	unsigned char* data = stbi_load(filePath, &width, &height, &numComponents, 0);
	if (data == NULL) {
		printf("Failed to load image %s", filePath);
		stbi_image_free(data);
		return 0;
	}

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}