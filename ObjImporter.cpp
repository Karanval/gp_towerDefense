#include <iostream>
#include <fstream>
#include "ObjImporter.hpp"
#include <algorithm>

template <class Container>
void split(const std::string& str, Container& cont, char delim = ' ');

void ObjImporter::loadMaterial(std::shared_ptr<sre::Material> &material, std::string filepath, std::vector<glm::vec4>& uvs,
							   std::string texturepath) {
	std::ifstream file;
	file.open(filepath);
	material = sre::Shader::getStandardPhong()->createMaterial();
	std::string texture;

	if (file.is_open()) {
		std::string line;
		int lineNum = 0;
		glm::vec2 metallicRoughness = glm::vec2();
		while (std::getline(file, line)) {
			std::vector<std::string> values;
			split(line, values);
			if (values.empty()) continue;
			std::string type = values[0];
			if (type == "newmtl") material->setName(&type[1]);
			else if (type == "Ns") metallicRoughness.x = std::stof(values[1], nullptr);
			else if (type == "Ni") metallicRoughness.y = std::stof(values[1], nullptr);
			else if (type == "Kd") material->setColor(sre::Color(std::stof(values[1], nullptr), 
																 std::stof(values[2], nullptr), 
																 std::stof(values[3], nullptr)));
			else if (type == "Ks") material->setSpecularity(sre::Color(std::stof(values[1], nullptr),
																	   std::stof(values[2], nullptr),
																	   std::stof(values[3], nullptr)));
			else if (type == "map_Kd") texture = values[1];
		}

		if (texturepath == "" && texture != "") {
			if (uvs.size() == 0) material->setTexture(sre::Texture::create().withFile(texture).withGenerateMipmaps(true).build());
			else material->setTexture(sre::Texture::create().withFile(texture).withWrapUV(sre::Texture::Wrap::ClampToEdge).build());
		}
		else if (texturepath != "") {
			if (uvs.size() == 0) material->setTexture(sre::Texture::create().withFile(texturepath).withGenerateMipmaps(true).build());
			else material->setTexture(sre::Texture::create().withFile(texturepath).withWrapUV(sre::Texture::Wrap::ClampToEdge).build());
		}

		material->setMetallicRoughness(metallicRoughness);


		
	}
	else material->setColor(sre::Color(1.0f, 0.0f, 1.0f));

}

// Load obj-file and generate a simplerenderengine-mesh
// Supports texture coordinates, normals and material
// Caution: This approach does not handle corrupt obj-files (e.g. erroneous face-indexing)
void ObjImporter::loadObj(std::shared_ptr<sre::Mesh> &mesh, std::string filepath, std::string &materialFilename) {
	std::vector<glm::vec3> verticesTemp;
	std::vector<glm::vec2> textureCoordsTemp;
	std::vector<glm::vec3> normalsTemp;
	std::vector<face> faces;
	bool isTextured = false;
	bool defineNormals = false;

	std::ifstream file;
	file.open(filepath);

	if (file.is_open()) {
		std::string line;
		int lineNum = 0;
		while (std::getline(file, line)) {
			std::vector<std::string> values;
			split(line, values);
			if (values.empty()) continue;
			std::string type = values[0];
			if (type == "v") { // vertex
				if (values.size() != 4) { // must contain 3 values and a definition
					std::cerr << "ObjImporter Error when loading object: Could not parse vertex (line " << lineNum << ").";
					return;
				}
				verticesTemp.push_back(glm::vec3(std::stof(values[1], nullptr), 
												 std::stof(values[2], nullptr), 
												 std::stof(values[3], nullptr)));
			}
			else if (type == "vt") { // texture coordinate
				if (values.size() != 3) { // must contain 2 values and a definition
					std::cerr << "ObjImporter Error when loading object: Could not parse texture coordinate (line " << lineNum << ").";
					return;
				}
				textureCoordsTemp.push_back(glm::vec2(std::stof(values[1], nullptr),
													  std::stof(values[2], nullptr)));
				isTextured = true;
			}
			else if (type == "vn") {
				if (values.size() != 4) {
					std::cerr << "ObjImporter Error when loading object: Could not parse texture coordinate (line " << lineNum << ").";
					return;
				}
				normalsTemp.push_back(glm::vec3(std::stof(values[1], nullptr),
												std::stof(values[2], nullptr),
												std::stof(values[3], nullptr)));
				defineNormals = true;
			}
			else if (type == "f") { // face
				if (values.size() < 4) { // too few arguments
					std::cerr << "ObjImporter Error when loading object: Could not parse face (line " << lineNum << ").";
					return;
				}
				std::vector<glm::vec3> verts;
				std::vector<glm::vec2> uvs;
				std::vector<glm::vec3> norms;
				for (int i = 1; i < values.size(); i++) { // loop through each of the face corners
					std::vector<std::string> indices;
					split(values[i], indices, '/'); // vertex OR vertex/uv OR vertex/uv/normal
					// Store vertex referenced by the index (note: obj file indices starts at 1)
					verts.push_back(verticesTemp[std::stoi(indices.at(0)) - 1]);
					// Store texture coordinate, if it exists (note: must be defined if normals are defined as well)
					if (indices.size() >= 2) uvs.push_back(textureCoordsTemp[std::stoi(indices.at(1)) - 1]);
					if (indices.size() >= 3) norms.push_back(normalsTemp[std::stoi(indices.at(2)) - 1]);
				}
				face f = { {verts[0], verts[1], verts[2]},
						   {glm::vec2(), glm::vec2(), glm::vec2()},
						   {glm::vec3(), glm::vec3(), glm::vec3()} };
				if (isTextured) {
					f.texCoords[0] = uvs[0];
					f.texCoords[1] = uvs[1];
					f.texCoords[2] = uvs[2];
				}
				if (defineNormals) {
					f.normals[0] = norms[0];
					f.normals[1] = norms[1];
					f.normals[2] = norms[2];
				}
				faces.push_back(f);
				if (values.size() == 5) { // face is a quad, create second face
					face f2 = { {verts[2], verts[3], verts[0]},
								{glm::vec2(), glm::vec2(), glm::vec2()},
								{glm::vec3(), glm::vec3(), glm::vec3()} };
					if (isTextured) {
						f2.texCoords[0] = uvs[2];
						f2.texCoords[1] = uvs[3];
						f2.texCoords[2] = uvs[0];
					}
					if (defineNormals) {
						f2.normals[0] = norms[2];
						f2.normals[1] = norms[3];
						f2.normals[2] = norms[0];
					}
					faces.push_back(f2);
				}
			}
			else if (type == "mtllib" && &materialFilename) { // material
				materialFilename = values[1];
			}
			lineNum++;
		}
		file.close();

		std::vector<glm::vec3> vertices = std::vector<glm::vec3>();
		std::vector<glm::vec4> uvs = std::vector<glm::vec4>();
		std::vector<glm::vec3> norms = std::vector<glm::vec3>();

		for (int i = 0; i < faces.size(); i++) 
			for (int j = 0; j < 3; j++) {
				face f = faces.at(i);
				vertices.push_back(f.vertices[j]);
				if (isTextured) uvs.push_back(glm::vec4(f.texCoords[j].x, f.texCoords[j].y, 0.0f, 0.0f));
				if (defineNormals) norms.push_back(f.normals[j]);
			}

		if (isTextured) {
			if (defineNormals) mesh = sre::Mesh::create().withPositions(vertices).withUVs(uvs).withNormals(norms).build();
			else mesh = sre::Mesh::create().withPositions(vertices).withUVs(uvs).build();
		}
		else if (defineNormals) mesh = sre::Mesh::create().withPositions(vertices).withNormals(norms).build();
		else mesh = sre::Mesh::create().withPositions(vertices).build();
	}
	else std::cerr << "ObjImporter Error: Could not open file " << filepath;
}

// source: http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
template <class Container>
void split(const std::string& str, Container& cont, char delim)
{
	std::stringstream ss(str);
	std::string token;
	while (std::getline(ss, token, delim)) {
		cont.push_back(token);
	}
}