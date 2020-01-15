#include "MaterialLoader.h"

MaterialLoader::~MaterialLoader()
{
}

void MaterialLoader::LoadMaterial(std::string file_)
{
	std::ifstream in(file_.c_str(), std::ios::in);
	if (!in) {
		Debug::Error("Could not open MTL file: " + file_, "MaterialLoader.cpp", __LINE__);
		return; // Return to stop function if void function
	}
	Material m = Material();
	std::string matName = "";
	std::string line;
	while (std::getline(in, line)) {
		if (line.substr(0, 7) == "newmtl ") {
			if (m.diffuseMap != 0) {
				MaterialHandler::GetInstance()->AddMaterial(matName, m);
				m = Material();
			}
			matName = line.substr(7);
			m.diffuseMap = LoadTexture2D(line.substr(7));
		}
		if (line.substr(0, 4) == "\tNs ") { // Shiness
			float shininess, s;
			std::istringstream Ns(line.substr(4));
			Ns >> s;
			shininess = s;
			m.shininess = shininess;
		}
		if (line.substr(0, 3) == "\td ") { // Transparency
			float transparent, t;
			std::istringstream d(line.substr(3));
			d >> t;
			transparent = t;
			m.transparency = transparent;
		}
		if (line.substr(0, 4) == "\tKa ") { // ambient
			glm::vec3 ambientVec;
			float x, y, z;
			std::istringstream Ka(line.substr(4));
			Ka >> x >> y >> z;
			ambientVec = glm::vec3(x, y, z);
			m.ambient = ambientVec;
		}
		if (line.substr(0, 4) == "\tKd ") { // diffuse
			glm::vec3 diffuseVec;
			float x, y, z;
			std::istringstream Kd(line.substr(4));
			Kd >> x >> y >> z;
			diffuseVec = glm::vec3(x, y, z);
			m.diffuse = diffuseVec;
		}
		if (line.substr(0, 4) == "\tKs ") { // specular
			glm::vec3 specularVec;
			float x, y, z;
			std::istringstream Ks(line.substr(4));
			Ks >> x >> y >> z;
			specularVec = glm::vec3(x, y, z);
			m.specular = specularVec;
		}
	}
	// Material PostProcessing
	if (m.diffuseMap != 0) {
		MaterialHandler::GetInstance()->AddMaterial(matName, m);
	}
}

GLuint MaterialLoader::LoadTexture2D(std::string fileName_)
{
	GLuint currentTexture = TextureHandler::GetInstance()->GetTexture(fileName_);
	if (currentTexture == 0) {
		TextureHandler::GetInstance()->CreateTexture2D(fileName_, "./Resources/Textures/" + fileName_ + ".jpg");
		currentTexture = TextureHandler::GetInstance()->GetTexture(fileName_);
	}
	return currentTexture;
}

