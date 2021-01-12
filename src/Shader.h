#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

class Shader {
public:
	// basic global helpers 
	unsigned int shaderProgramID, vShaderID, fShaderID;
	std::string vertexPath, fragmentPath;
	std::string vertexCode, fragmentCode;

	Shader();
	~Shader();

	void setVertexShaderPath(const char* path);
	void setFragmentShaderPath(const char* path);

	// load from disk
	void load();

	// link shader program
	void link();

	// use shader program
	void use();

	// compile and attach shaders
	void makeVertexShader();
	void makeFragmentShader();


	// setter functions to modify uniforms
	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setInt(const std::string& name, int value) const;
};