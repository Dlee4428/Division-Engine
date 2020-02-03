#ifndef FBO_H
#define FBO_H

#include "GlewObject.h"
#include "../Texture/TextureHandler.h"

// Frame Buffer Object
// Brief explanation of FBO
// http://www.songho.ca/opengl/gl_fbo.html
class FBO : public GlewObject {
public:
	FBO();
	~FBO();

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Bind() const;
	void BindDefaultFramebuffer() const;
	virtual void LoadGPU(bool afterGPU_ = false);

	void AttachTextureToColorBuffer(int colorAttachmentNumber_, const TextureHandler&texture_);
	void AttachTextureToDepthBuffer(const TextureHandler& texture_);
	void AttachTextureToStencilBuffer(const TextureHandler& texture_);
	void AttachTextureToDepthAndStencilBuffer(const TextureHandler& texture_);

private:
	int colorAtachmentCount;
};
#endif // !FBO_H
