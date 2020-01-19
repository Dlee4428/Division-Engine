#ifndef OBJLOADER_H
#define OBJLOADER_H

#include "../../../DivisionPCH.h"

class ObjLoader {
	public:

		/// C11 precautions 
		ObjLoader(const ObjLoader&) = delete;  /// Copy constructor
		ObjLoader(ObjLoader&&) = delete;       /// Move constructor
		ObjLoader& operator=(const ObjLoader&) = delete; /// Copy operator
		ObjLoader& operator=(ObjLoader&&) = delete;      /// Move operator

		static std::vector<glm::vec3> vertices; 
		static std::vector<glm::vec2> uvCoords;
		static std::vector<glm::vec3> normals;
		static bool loadOBJ(const char* path);
};

#endif