#ifndef GLEWOBJECT_H
#define GLEWOBJECT_H

#include "../../../DivisionPCH.h"

// Interface class of Glew Object
class GlewObject {
public:
	GlewObject();
	virtual ~GlewObject();

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Bind() const = 0;
	virtual void LoadGPU(bool afterGPU_ = false) = 0;

	inline GLuint GetName() const { return name; };
protected:
	GLuint name;
};

#endif // !GLEWOBJECT_H
