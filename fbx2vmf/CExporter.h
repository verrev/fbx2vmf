#ifndef _CEXPORTER_H_
#define _CEXPORTER_H_
#include <vector>
#include <string>
#include <fbxsdk.h>
#include <fstream>
#include "CParser.h"
class CExporter
{
public:
	void exportModel(const std::string &outFilePath, std::vector<FbxMesh*> &meshes, std::vector<FbxNode*>&nodes);
private:
	UINT getVertexSize(const VERTEX_LAYOUT &vl);
	void exportMesh(FbxMesh *mesh);
	void exportMaterials(std::vector<FbxNode*> nodes);
	bool write(void *data, const UINT &len, std::ios_base::openmode mode = std::ios::app);
private:
	std::string mModelFilePath;
	CParser mParser;
};
#endif