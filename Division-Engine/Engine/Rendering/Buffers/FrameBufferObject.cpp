#include "FrameBufferObject.h"

FrameBufferObject::FrameBufferObject() : colorAtachmentCount(0)
{
	OnCreate();
}

FrameBufferObject::~FrameBufferObject()
{
	OnDestroy();
}

void FrameBufferObject::OnCreate()
{
	glCreateFramebuffers(1, &name);
}

void FrameBufferObject::OnDestroy()
{
	glDeleteFramebuffers(1, &name);
}

void FrameBufferObject::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, name);
}

void FrameBufferObject::BindDefaultFramebuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::LoadGPU(bool afterGPU_)
{
	GLenum* drawBuffers = new GLenum[colorAtachmentCount];
	for (int i = 0; i < colorAtachmentCount; ++i) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glNamedFramebufferDrawBuffers(name, colorAtachmentCount, drawBuffers);
	delete[] drawBuffers;
}

void FrameBufferObject::AttachTextureToColorBuffer(int colorAttachmentNumber_, const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_COLOR_ATTACHMENT0 + colorAttachmentNumber_, texture_.GetName(), 0);
	colorAtachmentCount++;
}

void FrameBufferObject::AttachTextureToDepthBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_DEPTH_ATTACHMENT, texture_.GetName(), 0);
}

void FrameBufferObject::AttachTextureToStencilBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_STENCIL_ATTACHMENT, texture_.GetName(), 0);
}

void FrameBufferObject::AttachTextureToDepthAndStencilBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_DEPTH_STENCIL_ATTACHMENT, texture_.GetName(), 0);
}
