#include "Mesh.h"


Mesh::Mesh() : computeTangents(false), instanceCount(0)
{
}

Mesh::~Mesh()
{
	OnDestroy();
}

void Mesh::OnDestroy()
{
	// If error from Mesh check here
	if (iboVector.size() > 0) {
		for (auto ibo : iboVector) {
			delete ibo;
			ibo = nullptr;
		}
		iboVector.clear();
	}

	if (vboVector.size() > 0) {
		for (auto vbo : vboVector) {
			delete vbo;
			vbo = nullptr;
		}
		vboVector.clear();
	}
}

void Mesh::InitFromFile(const std::string& path_)
{
	// Assimp FileReader
	Assimp::Importer importer;

	unsigned int flags = aiProcess_GenSmoothNormals | aiProcess_PreTransformVertices |
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;

	if (computeTangents) {
		flags |= aiProcess_CalcTangentSpace;
	}

	const aiScene* scene = importer.ReadFile(path_, flags);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Failed to load mesh from file: " << path_ << ": " << importer.GetErrorString() << std::endl;
		return;
	}
	else {
		InitAssimpScene(scene);
	}
}

void Mesh::InitAssimpScene(const aiScene* scene_)
{
	// Get number of meshes from each framebuffer vector
	vboVector.resize(scene_->mNumMeshes);
	iboVector.resize(scene_->mNumMeshes);

	// Loop through each stage
	for (unsigned int meshI = 0; meshI < scene_->mNumMeshes; ++meshI) {
		aiMesh* mesh = scene_->mMeshes[meshI];
		unsigned int nVertices = mesh->mNumVertices;
		unsigned int nIndices = mesh->mNumFaces * mesh->mFaces->mNumIndices;

		IBO* ibo = new IBO(nIndices);
		VBO* vbo = new VBO(nVertices);

		// VBO Attributes set up here
		vbo->SetAttributeData(0, (void*)mesh->mVertices);

		// Normal Attributes
		int normalAttribute = vbo->AddAttribute(3, sizeof(float));
		vbo->SetAttributeData(normalAttribute, (void*)mesh->mNormals);

		// Tangents Attributes
		if (computeTangents) {
			int tangentAttribute = vbo->AddAttribute(3, sizeof(float));
			vbo->SetAttributeData(tangentAttribute, (void*)mesh->mTangents);
		}

		// TexCoords Attributes
		if (mesh->HasTextureCoords(0)) {
			int texCoordAttribute = vbo->AddAttribute(2, sizeof(float));

			for (unsigned int i = 0; i < nVertices; ++i) {
				vbo->SetVertexAttributeData(texCoordAttribute, i, (void*)&mesh->mTextureCoords[0][i]);
			}
		}

		// Color Attributes
		if (mesh->HasVertexColors(0)) {
			int colorAttribute = vbo->AddAttribute(3, sizeof(float));
			vbo->SetAttributeData(colorAttribute, (void*)mesh->mColors[0]);
		}

		// SubMesh Index
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
			for (unsigned int j = 0; j < mesh->mFaces->mNumIndices; ++j) {
				ibo->AddIndex(mesh->mFaces[i].mIndices[j]);
			}
		}

		vbo->LoadGPU();
		ibo->LoadGPU();

		vboVector[meshI] = vbo;
		iboVector[meshI] = ibo;
	}
}

void Mesh::InitFromRawData(const int verticesNum_, const int indicesNum_)
{
	// Push back data from all vectors
	vboVector.push_back(new VBO(verticesNum_));

	if (indicesNum_ > 0) {
		iboVector.push_back(new IBO(indicesNum_));
	}
}

void Mesh::SetAttribute(int attributeID_, const void* data_)
{
	// VBO Attribute Set
	vboVector[0]->SetAttributeData(attributeID_, data_);
}

void Mesh::VertexAttribute(int attributeID_, int vertexIndex_, const void* data_)
{
	// VBO Vertex Attribute data
	vboVector[0]->SetVertexAttributeData(attributeID_, vertexIndex_, data_);
}

int Mesh::AddAttribute(unsigned int elementNum_, unsigned int elementByteSize_, GLenum type_, GLboolean normalized_)
{
	return vboVector[0]->AddAttribute(elementNum_, elementByteSize_, type_, normalized_);
}


void Mesh::SetIndices(GLuint* indices_)
{
	// IBO indices
	iboVector[0]->SetIndices(indices_);
}

void Mesh::AddIndex(GLuint index_)
{
	iboVector[0]->AddIndex(index_);
}

void Mesh::EndFromRawData()
{
	// After Init RawDatas, either VBO and IBO LoadGPU() 
	vboVector[0]->LoadGPU();

	if (!iboVector.empty()) {
		iboVector[0]->LoadGPU();
	}
}

void Mesh::RenderIndex(GLenum mode_) const
{
	for (unsigned int i = 0; i < vboVector.size(); ++i) {
		vboVector[i]->GetVAO().Bind();
		iboVector[i]->Bind();
		glDrawElements(mode_, iboVector[i]->GetIndicesNumber(), GL_UNSIGNED_INT, 0);
	}
}

void Mesh::Render(GLenum mode_) const
{
	for (unsigned int i = 0; i < vboVector.size(); ++i) {
		vboVector[i]->GetVAO().Bind();
		glDrawArrays(mode_, 0, vboVector[i]->GetVerticesNumber());
	}
}

void Mesh::RenderInstance(GLenum mode_) const
{
	for (unsigned int i = 0; i < vboVector.size(); ++i) {
		vboVector[i]->GetVAO().Bind();
		iboVector[i]->Bind();
		glDrawElementsInstanced(mode_, iboVector[i]->GetIndicesNumber(), 
			GL_UNSIGNED_INT, 0, instanceCount);
	}
}

void Mesh::SetRenderInstanceProp(int instanceCount_, int index_, int divisor_)
{
	// Mesh instanceCount = instanceCount;
	this->instanceCount = instanceCount;
	for (unsigned int i = 0; i < vboVector.size(); ++i) {
		vboVector[i]->GetVAO().Bind();
		glVertexAttribDivisor(index_, divisor_);
	}
}