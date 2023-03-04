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

	uniformProjection = glGetUniformLocation(id, "projection");
	uniformModel = glGetUniformLocation(id, "model");
	uniformView = glGetUniformLocation(id, "view");
	uniformDirectionLight.colour = glGetUniformLocation(id, "directionalLight.base.colour");
	uniformDirectionLight.ambientIntensity = glGetUniformLocation(id, "directionalLight.base.ambientIntensity");
	uniformDirectionLight.direction = glGetUniformLocation(id, "directionalLight.direction");
	uniformDirectionLight.diffuseIntensity = glGetUniformLocation(id, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(id, "material.specularIntensity");
	uniformShininess= glGetUniformLocation(id, "material.shininess");
	uniformEyePos = glGetUniformLocation(id, "eyePosition");

	uniform_n_pointLights = glGetUniformLocation(id,"n_pointLights");

	for(size_t i = 0; i < MAX_N_POINT_LIGHTS; i++){
		uniformPointLight[i].colour = glGetUniformLocation(id,("pointLights["+std::to_string(i)+"].base.colour").c_str());
		uniformPointLight[i].ambientIntensity = glGetUniformLocation(id,("pointLights["+std::to_string(i)+"].base.ambientIntensity").c_str());
		uniformPointLight[i].diffuseIntensity = glGetUniformLocation(id,("pointLights["+std::to_string(i)+"].base.diffuseIntensity").c_str());
		uniformPointLight[i].position = glGetUniformLocation(id,("pointLights["+std::to_string(i)+"].position").c_str());
		uniformPointLight[i].constant = glGetUniformLocation(id,("pointLights["+std::to_string(i)+"].constant").c_str());
		uniformPointLight[i].linear = glGetUniformLocation(id,("pointLights["+std::to_string(i)+"].linear").c_str());
		uniformPointLight[i].exponent = glGetUniformLocation(id,("pointLights["+std::to_string(i)+"].exponent").c_str());
	}
	uniform_n_spotLights = glGetUniformLocation(id,"n_spotLights");
	for(size_t i = 0; i < MAX_N_SPOT_LIGHTS; i++){
		uniformSpotLight[i].colour = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].base.base.colour").c_str());
		uniformSpotLight[i].ambientIntensity = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].base.base.ambientIntensity").c_str());
		uniformSpotLight[i].diffuseIntensity = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].base.base.diffuseIntensity").c_str());
		uniformSpotLight[i].position = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].base.position").c_str());
		uniformSpotLight[i].constant = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].base.constant").c_str());
		uniformSpotLight[i].linear = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].base.linear").c_str());
		uniformSpotLight[i].exponent = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].base.exponent").c_str());
		uniformSpotLight[i].direction = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].direction").c_str());
		uniformSpotLight[i].edge = glGetUniformLocation(id,("spotLights["+std::to_string(i)+"].edge").c_str());
	}
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
GLuint Shader::getViewLocation(){return uniformView;}
GLuint Shader::getAmbientIntensityLocation(){return uniformDirectionLight.ambientIntensity;}
GLuint Shader::getAmbientColourLocation(){return uniformDirectionLight.colour;}
GLuint Shader::getDiffuseIntensityLocation(){return uniformDirectionLight.diffuseIntensity;}
GLuint Shader::getDirectionLocation(){return uniformDirectionLight.direction;}
GLuint Shader::getSpecularIntensityLocation(){return uniformSpecularIntensity;}
GLuint Shader::getShininessLocation(){return uniformShininess;}
GLuint Shader::getEyePositionLocation(){return uniformEyePos;}

void Shader::use(){glUseProgram(id);}
void Shader::clear(){
    if(id != 0) glDeleteProgram(id),id = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::setDirectionalLight(DirectionalLight* dLight){
	sLightLocation lightloc;
	lightloc.ambientIntensityLocation = uniformDirectionLight.ambientIntensity;
	lightloc.ambientCoulourLocation = uniformDirectionLight.colour;
	lightloc.diffuseIntensityLocation = uniformDirectionLight.diffuseIntensity;
	lightloc.directionLocation = uniformDirectionLight.direction;
	dLight->use(lightloc);	
}

void Shader::setPointLights(PointLight* pLights, unsigned int n){
	n_pointLights = std::min((unsigned int)MAX_N_POINT_LIGHTS, n);
	sLightLocation lightloc;
	glUniform1i(uniform_n_pointLights,n_pointLights);

	for(size_t i = 0; i < n_pointLights;i++){
		sLightLocation lightloc;
		lightloc.ambientIntensityLocation = uniformPointLight[i].ambientIntensity;
		lightloc.ambientCoulourLocation = uniformPointLight[i].colour;
		lightloc.diffuseIntensityLocation = uniformPointLight[i].diffuseIntensity;
		lightloc.positionLocation = uniformPointLight[i].position;
		lightloc.constantLocation = uniformPointLight[i].constant;
		lightloc.linearLocation = uniformPointLight[i].linear;
		lightloc.exponentialLocation = uniformPointLight[i].exponent;
		pLights[i].use(lightloc);
	}

}

void Shader::setSpotLights(SpotLight* sLights, unsigned int n){
	n_spotLights = std::min((unsigned int)MAX_N_SPOT_LIGHTS, n);
	sLightLocation lightloc;
	glUniform1i(uniform_n_spotLights,n_spotLights);

	for(size_t i = 0; i < n_spotLights;i++){
		sLightLocation lightloc;
		lightloc.ambientIntensityLocation = uniformSpotLight[i].ambientIntensity;
		lightloc.ambientCoulourLocation = uniformSpotLight[i].colour;
		lightloc.diffuseIntensityLocation = uniformSpotLight[i].diffuseIntensity;
		lightloc.positionLocation = uniformSpotLight[i].position;
		lightloc.constantLocation = uniformSpotLight[i].constant;
		lightloc.linearLocation = uniformSpotLight[i].linear;
		lightloc.exponentialLocation = uniformSpotLight[i].exponent;
		lightloc.directionLocation = uniformSpotLight[i].direction;
		lightloc.edgeLocation = uniformSpotLight[i].edge;
		sLights[i].use(lightloc);
	}
}
