#ifndef MATERIALLOADER_H
#define MATERIALLOADER_H

#include "../../DivisionPCH.h"
#include "../Graphic/MaterialHandler.h"
#include "../Graphic/TextureHandler.h"

class MaterialLoader {

public:
	MaterialLoader(const MaterialLoader&) = delete;
	MaterialLoader(MaterialLoader&&) = delete;
	MaterialLoader& operator=(const MaterialLoader&) = delete;
	MaterialLoader& operator=(MaterialLoader&&) = delete;

	MaterialLoader() = delete;
	~MaterialLoader();

	static void LoadMaterial(std::string file_);
private:
	static GLuint LoadTexture(std::string fileName_);
};
#endif // !MATERIALLOADER_H
