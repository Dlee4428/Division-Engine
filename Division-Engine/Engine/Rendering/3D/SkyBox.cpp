#include "SkyBox.h"

SkyBox::SkyBox(GLuint shaderProgram_) : cubemapTexture(0), 
modelLoc(0), projeLoc(0), skyboxVAO(0), skyboxVBO(0), viewLoc(0), textureID(0)
{
	shaderProgram = shaderProgram_;
}

SkyBox::~SkyBox()
{
	OnDestroy();
}

bool SkyBox::OnCreate()
{
	
	return true;
}

void SkyBox::OnDestroy()
{
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);
}

void SkyBox::Update(const float deltaTime_)
{
}

void SkyBox::Render(Camera* camera_) const
{
	glm::mat4 view = camera_->GetView();

	glDepthFunc(GL_LEQUAL);
	view = glm::mat4(glm::mat3(camera_->GetView())); // remove translation
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projeLoc, 1, GL_FALSE, glm::value_ptr(camera_->GetPerspective()));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

unsigned int SkyBox::LoadCubemap(std::vector<std::string> faces)
{
	SDL_Surface* textureSurface = nullptr;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	for (GLuint i = 0; i < faces.size(); i++) {
		textureSurface = IMG_Load(faces[i].c_str());
		if (textureSurface == nullptr) {
			return false;
		}
		int mode = (textureSurface->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, textureSurface->w, textureSurface->h, 0, mode, GL_UNSIGNED_BYTE, textureSurface->pixels);
		SDL_FreeSurface(textureSurface);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
