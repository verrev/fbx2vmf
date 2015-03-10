#ifndef _CCONVERTER_H_
#define _CCONVERTER_H_
#include <string>
#include <vector>
#include <Windows.h>
#include <fbxsdk.h>
#pragma comment(lib,"libfbxsdk-md.lib")
#include "CExporter.h"
//-------------------------------------------------
class CConverter
{
public:
	void convertScene(const std::string &scenePath);
private:
	void convertModel(FbxNode *rootNode);
	void getMeshes(FbxNode *node);
private:
	std::vector<FbxMesh*> mMeshes;
	std::string mScenePath;
	std::string mOutModelFilePath;
};
#endif