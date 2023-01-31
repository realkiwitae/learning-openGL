#include "Shader.h"
#include <sstream>

void Shader::createFromFiles(std::string vShader , std::string fShader){
    std::ifstream t(vShader);
    std::stringstream buffer;
    buffer << t.rdbuf();
    std::string vcode = buffer.str();
    buffer.str("");
    t.close();
    t = std::ifstream(fShader);
    buffer << t.rdbuf();
    std::string fcode = buffer.str();
    buffer.clear();
    t.close();

    compile(vcode,fcode);
};

void Shader::compile(std::string vcode , std::string fcode){
	id = glCreateProgram();

	if (!id) 
	{
		printf("Failed to create shader\n");
		return;
	}

	add(id, vcode, GL_VERTEX_SHADER);
	add(id, fcode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(id);
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(id);
	glGetProgramiv(id, GL_VALIDATE_STATUS, &result);
	if (!result) 
	{
		glGetProgramInfoLog(id, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

    uniformModel = glGetUniformLocation(id,"model");
    uniformProjection = glGetUniformLocation(id,"projection");
}

void Shader::add( GLuint theProgram,const std::string shaderCode, GLenum shaderType){
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode.c_str();
    
    GLint codeLength[1];
    codeLength[0] = shaderCode.size();

    glShaderSource(theShader , 1 , theCode, codeLength);
    glCompileShader(theShader);

    GLint res = 0;
    GLchar eLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &res);
    if(!res){
        glGetShaderInfoLog(theShader,sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n",shaderType, eLog);
        return;
    }
    
    glAttachShader(theProgram,theShader);
}

GLuint Shader::getProjectionLocation(){return uniformProjection;}
GLuint Shader::getModelLocation(){return uniformModel;}
void Shader::use(){glUseProgram(id);}
void Shader::clear(){
    if(id != 0) glDeleteProgram(id),id = 0;
    uniformModel = 0;
    uniformProjection = 0;
}
