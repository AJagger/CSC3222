/* CSC3224 Code
* Author: Aidan Jagger | 130281034
* Class Description:
* This class deals with loading data (textures/meshes/levels (scenes)/collision meshes) into the game engine from files stored in the game directories.
*/

#include "stdafx.h"
#include "../Frameworks/DataArray.cpp" //Temp fix to Linker Errors
#include "ResouceLoader.h"
#include "../Renderer/SOIL/SOIL.h"
#include <sstream>

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

bool ResourceLoader::LoadTextures(DataArray<GLuint> *textures, const string & texDirPath)
{
	char buffer[BUFFER_SIZE]; //Buffer to store the input
	int prevId = -1;

	ifstream f(texDirPath + "\\IncludeTextures.txt");

	if (!f) {
		return false;
	}

	stringstream ss;

	while (f.getline(buffer, BUFFER_SIZE))
	{
		ss << buffer;

		int fileId = -1;
		string texFile;

		for (int i = 0; i < 2; i++)
		{
			ss.getline(buffer, BUFFER_SIZE, ',');
			if (i == 0)
			{
				fileId = atoi(buffer);
			}
			else
			{
				texFile = buffer;
			}
		}

		//The data file being read must have a complete ordered list of game assets. If this is not the case then stop
		if (fileId != prevId + 1)
		{
			return false;
		}

		prevId = fileId;

		//Import texture and add the corresponding GLuint to the textures DataArray
		string pathString = texDirPath + "\\" + texFile;
		GLuint *tempGLuint = textures->CreateNew();
		*tempGLuint = SOIL_load_OGL_texture(pathString.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

		ss.clear();
	}

	f.close();

	return true;
}

bool ResourceLoader::LoadMeshes(DataArray<Mesh*> *meshes, const string & meshDirPath)
{
	char buffer[BUFFER_SIZE];
	int prevId = -1;

	ifstream f(meshDirPath + "\\IncludeMeshes.txt");

	if (!f) {
		return false;
	}

	stringstream ss;

	while(f.getline(buffer, BUFFER_SIZE))
	{
		ss << buffer;

		int fileId = -1;
		string meshFile;

		for(int i = 0; i < 2; i++)
		{
			ss.getline(buffer, BUFFER_SIZE, ',');
			if(i == 0)
			{
				fileId = atoi(buffer);
			}
			else
			{
				meshFile = buffer;
			}
		}

		//The data file being read must have a complete ordered list of game assets. If this is not the case then stop
		if (fileId != prevId + 1)
		{
			return false;
		}
		
		prevId = fileId;
		
		//Create new mesh* in the DataArray, import the mesh from the file and add it
		Mesh **tempMesh = meshes->CreateNew();
		*tempMesh = Mesh::LoadMeshFile(meshDirPath + "\\" + meshFile);

		ss.clear();
	}

	f.close();
	
	return true;
}

bool ResourceLoader::LoadGridData(vector<char> *tiles, const string & levelDirPath)
{
	ifstream f(levelDirPath + "\\GridData.txt");
	char temp;

	if (!f) {
		return false;
	}

	while(f.get(temp))
	{
		tiles->push_back(temp);
	}

	return true;
}
