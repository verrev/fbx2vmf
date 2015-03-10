#ifndef _FILESTRUCTURE_H_
#define _FILESTRUCTURE_H_
#include <Windows.h>
#include <DirectXMath.h>
using namespace DirectX;

struct ModelHeader
{
	ModelHeader(){}
	ModelHeader(const UINT &meshCount) :
		mMeshCount(meshCount){}
	UINT mMeshCount;
};
enum VERTEX_LAYOUT
{
	POSITION = 1,
	NORMAL = 2,
	UVS = 4,
	MAX_LAYOUT_ITEMS = 3
};
struct MeshHeader
{
	MeshHeader(){}
	MeshHeader(const UINT &verticeCount) :
		mVerticeCount(verticeCount) {}
	UINT mVerticeCount;
	VERTEX_LAYOUT mVertexLayout;
};
struct Vertex1P
{
	XMFLOAT3 mPos;
};
struct Vertex1P1N
{
	XMFLOAT3 mPos;
	XMFLOAT3 mNormal;
};
struct Vertex1P1N1UV
{
	XMFLOAT3 mPos;
	XMFLOAT3 mNormal;
	XMFLOAT2 mUV;
};
#endif