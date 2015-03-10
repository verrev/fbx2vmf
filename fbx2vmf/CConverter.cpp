#include "CConverter.h"
void CConverter::convertScene(const std::string& scenePath)
{
	mScenePath = scenePath;
	mOutModelFilePath = scenePath;
	mOutModelFilePath.replace(mOutModelFilePath.find("FBX"), 3, "vmf");

	FbxManager* manager = FbxManager::Create();
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);
	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(mScenePath.c_str(), -1, manager->GetIOSettings());
	FbxScene* scene = FbxScene::Create(manager, "");
	importer->Import(scene);
	importer->Destroy();
	//---------------------------------------------------
	FbxAxisSystem axisSystem = FbxAxisSystem::eDirectX;
	if (scene->GetGlobalSettings().GetAxisSystem() != axisSystem) axisSystem.ConvertScene(scene);
	//---------------------------------------------------
	convertModel(scene->GetRootNode());
}
void CConverter::convertModel(FbxNode *rootNode)
{
	getMeshes(rootNode);
	CExporter e;
	e.exportModel(mOutModelFilePath, mMeshes);
}
void CConverter::getMeshes(FbxNode *node)
{
	for (UINT i = 0; i < node->GetNodeAttributeCount(); ++i){
		if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::eMesh){
			mMeshes.push_back(static_cast<FbxMesh*>(node->GetNodeAttributeByIndex(i)));
		}
	}
	for (UINT i = 0; i < node->GetChildCount(); ++i){
		getMeshes(node->GetChild(i));
	}
}