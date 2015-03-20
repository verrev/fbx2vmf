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
	bool getMaterialIndices(FbxMesh *m, std::vector<MaterialIndex> &matInts);
private:
	void *getVertices1P(FbxMesh *m); 
	void *getVertices1P1N(FbxMesh *m);
	void *getVertices1P1N1UV(FbxMesh *m);
	void getUV(FbxMesh* m, int i, int j, XMFLOAT2& outUV);
	void getNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal);
	void getPosition(FbxVector4 *vertices, UINT index, XMFLOAT3 &pos);
private:
	MeshHeader mMeshHeader;
	std::vector<Vertex1P> mVertices1P; 
	std::vector<Vertex1P1N> mVertices1P1N;
	std::vector<Vertex1P1N1UV> mVertices1P1N1UV;
};
#endif