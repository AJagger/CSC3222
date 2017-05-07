/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class is a heavily modified version of the mesh class that some of the graphics tutorials for the ncgl library guide you through making.
* Included are a few testing functions for quickly generating fixed meshes as well as a function for reading in a mesh from a file - heavily modified
* from those demonstrated in the tutorials so as to allow for floats to be read in.
*/

#pragma once
#include "OGLRenderer.h"
#include "../nclgl/ChunkGenerator.h"
#include <string>
#include <fstream>

using std::ifstream;
using std::string;

enum MeshBuffer {
	VERTEX_BUFFER, COLOUR_BUFFER, TEXTURE_BUFFER, MAX_BUFFER
};

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

	virtual void Draw(GLuint texture = 0);
	static Mesh* GenerateTriangle();
	static Mesh* GenerateTriangleStrip();
	static Mesh* Mesh::LoadMeshFile(const string &fileName);

protected:
	void BufferData();

	GLuint arrayObject;
	GLuint bufferObject[MAX_BUFFER];
	GLuint numVertices;
	GLuint type;

	Vector3* vertices;
	Vector4* colours;

	Vector2* textureCoords;
};

