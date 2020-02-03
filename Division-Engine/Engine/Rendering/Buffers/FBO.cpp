#include "FBO.h"

FBO::FBO() : colorAtachmentCount(0)
{
	OnCreate();
}

FBO::~FBO()
{
	OnDestroy();
}

void FBO::OnCreate()
{
	glCreateFramebuffers(1, &name);
}

void FBO::OnDestroy()
{
	glDeleteFramebuffers(1, &name);
}

void FBO::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, name);
}

void FBO::BindDefaultFramebuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::LoadGPU(bool afterGPU_)
{
	GLenum* drawBuffers = new GLenum[colorAtachmentCount];
	for (int i = 0; i < colorAtachmentCount; ++i) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glNamedFramebufferDrawBuffers(name, colorAtachmentCount, drawBuffers);
	delete[] drawBuffers;
}

void FBO::AttachTextureToColorBuffer(int colorAttachmentNumber_, const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_COLOR_ATTACHMENT0 + colorAttachmentNumber_, texture_.GetName(), 0);
	colorAtachmentCount++;
}

void FBO::AttachTextureToDepthBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_DEPTH_ATTACHMENT, texture_.GetName(), 0);
}

void FBO::AttachTextureToStencilBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_STENCIL_ATTACHMENT, texture_.GetName(), 0);
}

void FBO::AttachTextureToDepthAndStencilBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_DEPTH_STENCIL_ATTACHMENT, texture_.GetName(), 0);
}
