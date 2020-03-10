#include "MaterialHandler.h"

MaterialHandler::MaterialHandler() : shaderActive(0)
{
}

MaterialHandler::~MaterialHandler()
{
}

void MaterialHandler::SetTexture(unsigned int idx_, TextureHandler* texHandler_)
{
	// IF texture's index size is greather than and equal to vector
	// Proceed to resize vector by index + 1
	// Lastly Texture array stores in vector allocation
	if (idx_ >= texVector.size()) {
		texVector.resize(idx_ + 1);
	}
	texVector.at(idx_) = texHandler_;
}

void MaterialHandler::SetShaderProgram(unsigned int idx_, ShaderProgram* sProgram_)
{
	// Same step as SetTexture()
	if (idx_ >= sProgramVector.size()) {
		sProgramVector.resize(idx_ + 1);
	}
	sProgramVector.at(idx_) = sProgram_;
}

void MaterialHandler::BindShader() const
{
	sProgramVector[shaderActive]->BindShader();
}

void MaterialHandler::BindTexture() const
{
	for (unsigned int i = 0; i < texVector.size(); i++) {
		texVector[i]->Bind(i);
	}
}

void MaterialHandler::Bind() const
{
	BindShader();
	BindTexture();
}
