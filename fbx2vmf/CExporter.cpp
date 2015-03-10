#include "CExporter.h"
#include "CParser.h"
#include <iostream>
void CExporter::exportModel(const std::string &outFilePath, std::vector<FbxMesh*> &meshes)
{
	mModelFilePath = outFilePath;
	// Write the file header - how many meshes are there.
	ModelHeader mh = ModelHeader(meshes.size());
	write(&mh, sizeof(ModelHeader), 0);

	// Write meshes.
	for (UINT i = 0; i < mh.mMeshCount; ++i){
		exportMesh(meshes[i]);
	}
}
UINT CExporter::getVertexSize(const VERTEX_LAYOUT &vl)
{
		UINT size = 0;
		if (vl & VERTEX_LAYOUT::POSITION){
			size += sizeof(XMFLOAT3);
		}
		if (vl & VERTEX_LAYOUT::NORMAL){
			size += sizeof(XMFLOAT3);
		}
		if (vl & VERTEX_LAYOUT::UVS){
			size += sizeof(XMFLOAT2);
		}
		return size;
}
void CExporter::exportMesh(FbxMesh *mesh)
{
	void *vertices = mParser.getVertices(mesh);
	MeshHeader mh = mParser.getMeshHeader();
	write(&mh, sizeof(MeshHeader));
	write(vertices, getVertexSize(mh.mVertexLayout) * mh.mVerticeCount);
}
bool CExporter::write(void *data, const UINT &len, std::ios_base::openmode mode)
{
	std::ofstream outFile(mModelFilePath.c_str(), std::ios::binary | mode);
	if (outFile.is_open() && outFile.good()){
		outFile.write((char*)data, len);
		outFile.close();
		return 1;
	}
	return 0;
}