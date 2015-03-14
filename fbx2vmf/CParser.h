#ifndef _CPARSER_H_
#define _CPARSER_H_
#include "FileStructure.h"
#include <fbxsdk.h>
#include <vector>
class CParser
{
public:
	MeshHeader getMeshHeader();
	void *getVertices(FbxMesh *m);
	bool getMaterials(FbxNode *n, std::vector<Material> &mats, std::vector<std::string> &textures);
private:
	void *getVertices1P(FbxMesh *m); 
	void *getVertices1P1N(FbxMesh *m);
	void *getVertices1P1N1UV(FbxMesh *m);
private:
	MeshHeader mMeshHeader;
	std::vector<Vertex1P> mVertices1P; 
	std::vector<Vertex1P1N> mVertices1P1N;
	std::vector<Vertex1P1N1UV> mVertices1P1N1UV;
};
#endif