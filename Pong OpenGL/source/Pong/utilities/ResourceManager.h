#pragma once
#include <fstream>
#include <sstream>
#include <map>
#include <string>

#include "../vendor/stb_image.h"

#include "Shader.h"
#include "Texture.h"

class ResourceManager {
public:
	static void CreateShader(std::string vertexPath, std::string fragmentPath, std::string name);
	static Shader& GetShader(std::string name);
	static void CreateTexture(std::string path, std::string name);
	static Texture& GetTexture(std::string name);
private:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture> Textures;
};