#include "CParser.h"
#include <iostream>
#include <algorithm>
MeshHeader CParser::getMeshHeader()
{
	return mMeshHeader;
}
void *CParser::getVertices(FbxMesh *m)
{
	mMeshHeader = MeshHeader(-1);
	return getVertices1P1N1UV(m);
}
void getUV(FbxMesh* m, int i, int j, XMFLOAT2& outUV)
{
	FbxStringList lUVSetNameList;
	m->GetUVSetNames(lUVSetNameList);
	FbxVector2 fbxUV;
	bool unmapped = 0;
	m->GetPolygonVertexUV(i, j, lUVSetNameList.GetStringAt(0), fbxUV, unmapped);
	outUV = XMFLOAT2(fbxUV.mData[0], 1 - fbxUV.mData[1]);
}
void getUV(FbxMesh* inMesh, int inCtrlPointIndex, int inTextureUVIndex, int inUVLayer, XMFLOAT2& outUV)
{
	if (inUVLayer >= 2 || inMesh->GetElementUVCount() <= inUVLayer)
	{
		throw std::exception("Invalid UV Layer Number");
	}
	FbxGeometryElementUV* vertexUV = inMesh->GetElementUV(inUVLayer);

	switch (vertexUV->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(inCtrlPointIndex);
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
			outUV.y = static_cast<float>(1 - vertexUV->GetDirectArray().GetAt(index).mData[1]); //INVERTED!
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			outUV.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}
void getNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal)
{
	if (inMesh->GetElementNormalCount() < 1)
	{
		throw std::exception("Invalid Normal Number");
	}

	FbxGeometryElementNormal* vertexNormal = inMesh->GetElementNormal(0);
	switch (vertexNormal->GetMappingMode())
	{
	case FbxGeometryElement::eByControlPoint:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;

	case FbxGeometryElement::eByPolygonVertex:
		switch (vertexNormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
			outNormal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			outNormal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			outNormal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;

		default:
			throw std::exception("Invalid Reference");
		}
		break;
	}
}
void getPosition(FbxVector4 *vertices, UINT index, XMFLOAT3 &pos)
{
	pos = XMFLOAT3(vertices[index].mData[0], vertices[index].mData[1], vertices[index].mData[2]);
}

void *CParser::getVertices1P1N1UV(FbxMesh *m)
{
	VERTEX_LAYOUT vl = VERTEX_LAYOUT::POSITION;
	vl = static_cast<VERTEX_LAYOUT>(vl | VERTEX_LAYOUT::NORMAL);
	vl = static_cast<VERTEX_LAYOUT>(vl | VERTEX_LAYOUT::UVS);
	FbxVector4 *fbxVertices = m->GetControlPoints();
	UINT vertexCounter = 0;
	for (UINT i = 0; i < m->GetPolygonCount(); i++){
		for (UINT j = 0; j < 3; ++j){
			int index = m->GetPolygonVertex(i, j);
			Vertex1P1N1UV v;
			getPosition(fbxVertices, index, v.mPos);
			getNormal(m, index, vertexCounter, v.mNormal);
			getUV(m, i, j, v.mUV);
			//getUV(m, index, m->GetTextureUVIndex(i, j), 0, v.mUV);
			mVertices1P1N1UV.push_back(v);
			++vertexCounter;
		}
	}
	mMeshHeader.mVerticeCount = mVertices1P1N1UV.size();
	mMeshHeader.mVertexLayout = vl;
	return &mVertices1P1N1UV[0];
}
void *CParser::getVertices1P1N(FbxMesh *m)
{
	VERTEX_LAYOUT vl = VERTEX_LAYOUT::POSITION;
	vl = static_cast<VERTEX_LAYOUT>(vl | VERTEX_LAYOUT::NORMAL);
	FbxVector4 *fbxVertices = m->GetControlPoints();
	UINT vertexCounter = 0;
	for (UINT i = 0; i < m->GetPolygonCount(); i++){
		for (UINT j = 0; j < 3; ++j){
			int index = m->GetPolygonVertex(i, j);
			Vertex1P1N v;
			getPosition(fbxVertices, index, v.mPos);
			getNormal(m, index, vertexCounter, v.mNormal);
			mVertices1P1N.push_back(v);
			++vertexCounter;
		}
	}
	mMeshHeader.mVerticeCount = mVertices1P1N.size();
	mMeshHeader.mVertexLayout = vl;
	return &mVertices1P1N[0];
}
void *CParser::getVertices1P(FbxMesh *m)
{
	VERTEX_LAYOUT vl = VERTEX_LAYOUT::POSITION;
	FbxVector4 *fbxVertices = m->GetControlPoints();
	for (UINT i = 0; i < m->GetPolygonCount(); i++){
		for (UINT j = 0; j < 3; ++j){
			int index = m->GetPolygonVertex(i, j);
			Vertex1P v;
			getPosition(fbxVertices, index, v.mPos);
			mVertices1P.push_back(v);
		}
	}
	mMeshHeader.mVerticeCount = mVertices1P.size();
	mMeshHeader.mVertexLayout = vl;
	return &mVertices1P[0];
}
bool CParser::getMaterials(FbxNode *n, std::vector<Material> &mats, std::vector<std::string> &textures)
{
	int matCount = 0;
	if (n)
		matCount = n->GetMaterialCount();
	if (matCount > 0){
		for (int i = 0; i < matCount; ++i){
			FbxSurfaceMaterial *material = n->GetMaterial(i);
			if (material->GetClassId().Is(FbxSurfacePhong::ClassId)){
				auto ambient = ((FbxSurfacePhong *)material)->Ambient;
				XMFLOAT3 amb = XMFLOAT3(ambient.Get()[0], ambient.Get()[1], ambient.Get()[2]);
				auto diffuse = ((FbxSurfacePhong *)material)->Diffuse;
				XMFLOAT3 dif = XMFLOAT3(diffuse.Get()[0], diffuse.Get()[1], diffuse.Get()[2]);
				auto specular = ((FbxSurfacePhong *)material)->Specular;
				XMFLOAT3 spec = XMFLOAT3(specular.Get()[0], specular.Get()[1], specular.Get()[2]);
				auto emissive = ((FbxSurfacePhong *)material)->Emissive;
				XMFLOAT3 emi = XMFLOAT3(emissive.Get()[0], emissive.Get()[1], emissive.Get()[2]);
				auto alpha = 1.0 - ((FbxSurfacePhong *)material)->TransparencyFactor;
				auto shininess = ((FbxSurfacePhong *)material)->Shininess;
				auto reflectivity = ((FbxSurfacePhong *)material)->ReflectionFactor;
				Material m(MATERIAL_TYPE::PHONG, 1, amb, dif, spec, emi, alpha, shininess, reflectivity); // 1 - has mats
				mats.push_back(m);
			}
			else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)){
				auto ambient = ((FbxSurfacePhong *)material)->Ambient;
				XMFLOAT3 amb = XMFLOAT3(ambient.Get()[0], ambient.Get()[1], ambient.Get()[2]);
				auto diffuse = ((FbxSurfacePhong *)material)->Diffuse;
				XMFLOAT3 dif = XMFLOAT3(diffuse.Get()[0], diffuse.Get()[1], diffuse.Get()[2]);
				auto emissive = ((FbxSurfacePhong *)material)->Emissive;
				XMFLOAT3 emi = XMFLOAT3(emissive.Get()[0], emissive.Get()[1], emissive.Get()[2]);
				auto alpha = 1.0 - ((FbxSurfacePhong *)material)->TransparencyFactor;
				Material m(MATERIAL_TYPE::LAMBERT, 1, amb, dif, XMFLOAT3(0, 0, 0), emi, alpha, 0, 0); // 1 - has mats
				mats.push_back(m);
			}
			else{
				// unk mat // THOW ERROR! ? 
			}
			//====================================================================== TEXTURES
			FbxProperty property = material->FindProperty(FbxLayerElement::sTextureChannelNames[0]);
			if (property.IsValid()){
				unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
				for (unsigned int j = 0; j < textureCount; ++j){
					FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(j);
					if (layeredTexture) { throw std::exception("Layered Texture is currently unsupported\n"); return 0; }
					else{
						FbxTexture* texture = property.GetSrcObject<FbxTexture>(j);
						if (texture){
							std::string textureType = property.GetNameAsCStr();
							FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);
							if (fileTexture){
								if (textureType == "DiffuseColor"){
									textures.push_back(fileTexture->GetRelativeFileName());
								}
								else if (textureType == "SpecularColor"){
									//textures.push_back(fileTexture->GetFileName()); UNSUPPORTED AS OF YET
								}
								else if (textureType == "Bump"){
									//textures.push_back(fileTexture->GetFileName()); UNSUPPORTED AS OF YET
								}
							}
						}
					}
				}
			}
		}
	}
	else{
		Material m(MATERIAL_TYPE::PHONG, 0, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), 1, 0, 0);
		mats.push_back(m);
	}
	if (mats.size() > 0) return 1;
	return 0;
}