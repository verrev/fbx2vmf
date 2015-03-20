#ifndef _FILESTRUCTURE_H_
#define _FILESTRUCTURE_H_
#include <Windows.h>
#include <DirectXMath.h>
using namespace DirectX;
#define MAX_LAYOUT_ITEMS 3
enum MATERIAL_TYPE
{
	PHONG,
	LAMBERT
};
struct MaterialIndexHeader
{
	MaterialIndexHeader(){ /* do not use !*/ }
	MaterialIndexHeader(const UINT &subMeshCount): mSubmeshCount(subMeshCount) {}
	UINT mSubmeshCount;
};
struct MaterialIndex
{
	MaterialIndex(){ /* do not use !*/ }
	MaterialIndex(const UINT &matIndex, const UINT &begin, const UINT &end)
		: mMaterialIndex(matIndex), mBegin(begin), mEnd(end) {}
	UINT mMaterialIndex, mBegin, mEnd;
};
struct MaterialHeader
{
	MaterialHeader() { /* do not use !*/ }
	MaterialHeader(const UINT &matCount) :
		mMaterialCount(matCount){}

	UINT mMaterialCount;
};
struct Material
{
	Material(){ /* do not use !*/ }
	Material(const MATERIAL_TYPE &type,const int &hasTextures,const XMFLOAT3 &ambient, const XMFLOAT3 &diffuse, const XMFLOAT3 &specular,
		const XMFLOAT3 &emissive, const float &alpha,
		const float &shininess, const float &reflectivity) :
		mMaterialType(type),mHasTextures(hasTextures),
		mAmbient(ambient), mDiffuse(diffuse), mSpecular(specular),
		mEmissive(emissive), mAlpha(alpha), mShininess(shininess),
		mReflectivity(reflectivity){}
	int mHasTextures;
	MATERIAL_TYPE mMaterialType;
	XMFLOAT3 mAmbient, mDiffuse, mSpecular, mEmissive;
	float mAlpha, mShininess, mReflectivity;
	int pad[3];
};
struct TextureHeader
{
	TextureHeader() { /* do not use !*/ }
	TextureHeader(const UINT &textureCount) :
	mTextureCount(textureCount){}

	UINT mTextureCount;
};
struct Texture
{
	char mPath[100]; // bad design, but hey, whatcha gonna do?
};
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
	UVS = 4
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