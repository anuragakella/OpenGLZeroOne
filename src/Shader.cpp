#include "Shader.h"

Shader::Shader()
{
	Shader::vertexPath = "";
	Shader::fragmentPath = "";
	shaderProgramID = glCreateProgram();
}

Shader::~Shader(){
	glDeleteProgram(shaderProgramID);
	vertexCode = fragmentCode = "";
}


// sets path
void Shader::setVertexShaderPath(const char* path){
	Shader::vertexPath = path;
}

void Shader::setFragmentShaderPath(const char* path){
	Shader::fragmentPath = path;
}

void Shader::load()
{

	// create file streams
	std::ifstream vShaderFile, fShaderFile;
	std::stringstream vStream, fStream;

	// make sure they can throw exceptiions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


	try {
		//open and read files
		std::cout << vertexPath << std::endl;
		vShaderFile.open(Shader::vertexPath);
		std::cout << "ok" << std::endl;
		
		
		std::cout << fragmentPath << std::endl;
		fShaderFile.open(Shader::fragmentPath);
		std::cout << "ok" << std::endl;
		

		// dump file content into a stringstream
		vStream << vShaderFile.rdbuf();
		fStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		// store the stream in a string object for compiliation
		vertexCode = vStream.str();
		fragmentCode = fStream.str();

	}
	catch (std::ifstream::failure e) {
		std::cout << "error::shader.cpp: couldn't read file" << std::endl;
	}
}

// compiles the vertex shader
void Shader::makeVertexShader(){
	
	int done;
	char err[1024];
	const char* tv = vertexCode.c_str();
	// create shader and compile the source code
	vShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShaderID, 1, &tv, NULL);
	glCompileShader(vShaderID);

	glGetShaderiv(vShaderID, GL_COMPILE_STATUS, &done);
	if (!done) {
		glGetShaderInfoLog(vShaderID, 1024, NULL, err);
		std::cout << "Shader Compilation Failed::vertex: " << err << std::endl;
	}
	glAttachShader(shaderProgramID, vShaderID);

}

void Shader::makeFragmentShader(){

	int done;
	char err[1024];
	const char* fv = fragmentCode.c_str();
	// create shader and compile the source code
	fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShaderID, 1, &fv, NULL);
	glCompileShader(fShaderID);

	glGetShaderiv(fShaderID, GL_COMPILE_STATUS, &done);
	if (!done) {
		glGetShaderInfoLog(fShaderID, 1024, NULL, err);
		std::cout << "Shader Compilation Failed::fragment: " << err << std::endl;
	}
	glAttachShader(shaderProgramID, fShaderID);

}

// link shader and check for errors
void Shader::link() {
	int done;
	char err[1024];
	glLinkProgram(shaderProgramID);
	glGetShaderiv(shaderProgramID, GL_LINK_STATUS, &done);
	glDeleteShader(fShaderID);
	glDeleteShader(vShaderID);
	if (!done) {
		glGetShaderInfoLog(shaderProgramID, 1024, NULL, err);
		std::cout << "Shader Linking Failed: " << err << std::endl;
	}
}

const char* GetGLErrorStr(GLenum err)
{
	switch (err)
	{
	case GL_NO_ERROR:          return "No error";
	case GL_INVALID_ENUM:      return "Invalid enum";
	case GL_INVALID_VALUE:     return "Invalid value";
	case GL_INVALID_OPERATION: return "Invalid operation";
	case GL_STACK_OVERFLOW:    return "Stack overflow";
	case GL_STACK_UNDERFLOW:   return "Stack underflow";
	case GL_OUT_OF_MEMORY:     return "Out of memory";
	default:                   return "Unknown error";
	}
}

void CheckGLError()
{
	while (true)
	{
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;

		std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;
	}
}

void Shader::use(){
	glUseProgram(shaderProgramID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
	CheckGLError();
}

void Shader::setFloat(const std::string& name, float value) const{
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
	CheckGLError();
}

void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
	CheckGLError();
}

