#include "ModelLoader.hpp"
#include "ObjImporter.hpp"
#include "MeshComponent.hpp"
#include "MaterialComponent.hpp"

void ModelLoader::loadModel(std::shared_ptr<GameObject> obj, std::string objName, std::string mtlName, std::string textureNameWithExt) {
	std::shared_ptr<sre::Mesh> mesh;
	std::shared_ptr<sre::Material> material;
	std::string texture = textureNameWithExt == "" ? "" : texturePath + textureNameWithExt;

	std::string mtlNameFromObj;
	ObjImporter::loadObj(mesh, objectPath + objName + ".obj", mtlNameFromObj);

	std::string chosenMtlPath = materialPath + (mtlName == "" ? mtlNameFromObj : mtlName) + ".mtl";
	ObjImporter::loadMaterial(material, chosenMtlPath, mesh->getUVs(), texture);

	//meshes.push_back(mesh);
	//TowerDefense::materials.push_back(material);

	obj->addComponent<MeshComponent>()->setMesh(mesh);
	obj->addComponent<MaterialComponent>()->setMaterial(material);

}