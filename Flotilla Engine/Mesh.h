#pragma once
#include "Terminology.h"
#include "Vertex.h"
#include "Shader.h"
class Mesh
{
public:
	Mesh(Vertex* vertList, int vertCount, glIndex* indList, int indCount);
	~Mesh();
	void Draw(Shader shader) const;
private:
	void primeMesh();

	Vertex* vertexList;
	glIndex* indexList;

	glID vertexArray;
	glID vertexBuffer;
	glID indexBuffer;

	int vertexCount;
	int indexCount;

};

