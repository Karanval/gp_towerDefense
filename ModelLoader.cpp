#include "ModelLoader.hpp"
#include "ObjImporter.hpp"

ModelLoader::ModelLoader() {
	loadedMeshes = std::map<std::string, std::shared_ptr<MeshComponent>>();
	loadedMaterials = std::map<std::string, std::shared_ptr<MaterialComponent>>();
}

void ModelLoader::loadModel(std::shared_ptr<GameObject> obj, std::string objName, std::string mtlName, std::string textureNameWithExt) {
	auto loadedMesh = loadedMeshes.find(objName);
	if (loadedMesh != loadedMeshes.end()) {
		obj->addComponent<MeshComponent>()->setMesh(loadedMesh->second->getMesh());
		auto loadedMat = loadedMaterials.find(mtlName);
		if (loadedMat != loadedMaterials.end()) obj->addComponent<MaterialComponent>()->setMaterial(loadedMat->second->getMaterial());
		return;
	}
	
	std::shared_ptr<sre::Mesh> mesh;
	std::shared_ptr<sre::Material> material;
	std::string texture = textureNameWithExt == "" ? "" : texturePath + textureNameWithExt;

	std::string mtlNameFromObj;
	ObjImporter::loadObj(mesh, objectPath + objName + ".obj", mtlNameFromObj);

	std::string chosenMtlPath = materialPath + (mtlName == "" ? mtlNameFromObj : mtlName) + ".mtl";
	ObjImporter::loadMaterial(material, chosenMtlPath, mesh->getUVs(), texture);


	std::shared_ptr<MeshComponent> meshComponent = obj->addComponent<MeshComponent>();
	std::shared_ptr<MaterialComponent> matComponent = obj->addComponent<MaterialComponent>();
	meshComponent->setMesh(mesh);
	matComponent->setMaterial(material);
	loadedMeshes.insert(std::pair <std::string, std::shared_ptr<MeshComponent>>(objName, meshComponent));
	loadedMaterials.insert(std::pair <std::string, std::shared_ptr<MaterialComponent>>(mtlName, matComponent));
}