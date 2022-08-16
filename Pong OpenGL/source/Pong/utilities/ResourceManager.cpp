#include "ResourceManager.h"

std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture> ResourceManager::Textures;

void ResourceManager::CreateShader(const std::string vertexPath, const std::string fragmentPath, const std::string name) {
	std::ifstream vertexFile{ vertexPath };
	std::ifstream fragmentFile{ fragmentPath };

	std::stringstream vertexStream;
	vertexStream << vertexFile.rdbuf();
	std::stringstream fragmentStream;
	fragmentStream << fragmentFile.rdbuf();

	std::string vertexCode{ vertexStream.str() };
	std::string fragmentCode{ fragmentStream.str() };

	Shader shader{vertexCode.c_str(), fragmentCode.c_str()};
	Shaders[name] = shader;
}

Shader& ResourceManager::GetShader(const std::string name) {
	return Shaders[name];
}

void ResourceManager::CreateTexture(const std::string path, const std::string name) {
	int width;
	int height;
	int nrChannels;
	unsigned char* data{ stbi_load(path.c_str(), &width, &height, &nrChannels, 0)};
	Texture texture{ data, width, height };
	Textures[name] = texture;
}

Texture& ResourceManager::GetTexture(const std::string name) {
	return Textures[name];
}