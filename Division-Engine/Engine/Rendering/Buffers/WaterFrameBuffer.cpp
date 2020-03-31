#include "WaterFrameBuffer.h"

WaterFrameBuffer::WaterFrameBuffer() : colorAtachmentCount(0)
{
	OnCreate();
}

WaterFrameBuffer::~WaterFrameBuffer()
{
	OnDestroy();
}

void WaterFrameBuffer::OnCreate()
{
	glCreateFramebuffers(1, &name);
}

void WaterFrameBuffer::OnDestroy()
{
	glDeleteFramebuffers(1, &name);
}

void WaterFrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, name);
}

void WaterFrameBuffer::UnBindFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WaterFrameBuffer::Process(bool afterProcess_)
{
	GLenum* drawBuffers = new GLenum[colorAtachmentCount];
	for (int i = 0; i < colorAtachmentCount; ++i) {
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glNamedFramebufferDrawBuffers(name, colorAtachmentCount, drawBuffers);
	delete[] drawBuffers;
}

void WaterFrameBuffer::AttachTextureToColorBuffer(int colorAttacNum_, const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_COLOR_ATTACHMENT0 + colorAttacNum_, texture_.GetName(), 0);
	colorAtachmentCount++;
}

void WaterFrameBuffer::AttachTextureToDepthBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_DEPTH_ATTACHMENT, texture_.GetName(), 0);
}

void WaterFrameBuffer::AttachTextureToStencilBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_STENCIL_ATTACHMENT, texture_.GetName(), 0);
}

void WaterFrameBuffer::AttachTextureToDepthAndStencilBuffer(const TextureHandler& texture_)
{
	glNamedFramebufferTexture(name, GL_DEPTH_STENCIL_ATTACHMENT, texture_.GetName(), 0);
}
