#ifndef WATERFRAMEBUFFER_H
#define WATERFRAMEBUFFER_H

#include "GlewObject.h"
#include "../Texture/TextureHandler.h"

class WaterFrameBuffer : public GlewObject {
public:
	WaterFrameBuffer();
	~WaterFrameBuffer();

	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void Bind() const;
	void UnBindFrameBuffer() const;
	virtual void Process(bool afterProcess_ = false);

	void AttachTextureToColorBuffer(int colorAttacNum_, const TextureHandler& texture_);
	void AttachTextureToDepthBuffer(const TextureHandler& texture_);
	void AttachTextureToStencilBuffer(const TextureHandler& texture_);
	void AttachTextureToDepthAndStencilBuffer(const TextureHandler& texture_);


private:
	int colorAtachmentCount;
};

#endif // !WATERFRAMEBUFFER_H
