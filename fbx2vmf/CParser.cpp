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
			outUV.y = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);
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
			getUV(m, index, m->GetTextureUVIndex(i, j), 0, v.mUV);
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


