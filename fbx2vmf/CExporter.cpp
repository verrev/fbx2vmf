#include "CExporter.h"
#include "CParser.h"
#include <iostream>
#include <set>

void CExporter::exportModel(const std::string &outFilePath, std::vector<FbxMesh*> &meshes, std::vector<FbxNode*>&nodes)
{
	mModelFilePath = outFilePath;
	// Write the file header - how many meshes are there.
	ModelHeader mh = ModelHeader(meshes.size());
	write(&mh, sizeof(ModelHeader), 0);

	// Write meshes.
	for (auto mesh : meshes){
		exportMesh(mesh);
	}
	exportMaterials(nodes);
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
void CExporter::exportMaterials(std::vector<FbxNode*> nodes)
{
	std::vector<Material> materials;
	std::vector<std::string> textures;
	for (auto node : nodes){
		mParser.getMaterials(node,materials,textures);
	}
	std::set<std::string> textureSet(textures.begin(), textures.end());
	std::vector<std::string> texs(textureSet.begin(),textureSet.end()); // only unique ones left
	if (materials.size()){
		std::string s = mModelFilePath;
		mModelFilePath.replace(mModelFilePath.size() - 3, 3, "vma");
		MaterialHeader mh = MaterialHeader(materials.size());
		write(&mh, sizeof(MaterialHeader),0);
		write(&materials[0], mh.mMaterialCount * sizeof(Material));
		TextureHeader th = TextureHeader(texs.size());
		write(&th, sizeof(TextureHeader));
		for (auto tex : texs){
			Texture t;
			sprintf_s(t.mPath, "%.100s", tex.c_str());
			write(&t, sizeof(Texture)); // here we might need to remove some part of the path
		}
		mModelFilePath = s;
	}
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