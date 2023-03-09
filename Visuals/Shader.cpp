#include "Shader.h"
#include <sstream>

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;

	pointLightCount = 0;
	spotLightCount = 0;
}

void Shader::CreateFromFiles(std::string vShader , std::string fShader)
{
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
	CompileShader(vcode,fcode);
}

void Shader::CompileShader(std::string vertexCode , std::string fragmentCode)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		printf("Error creating shader program!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID,("pointLights["+std::to_string(i)+"].base.colour").c_str());
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID,("pointLights["+std::to_string(i)+"].base.ambientIntensity").c_str());
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID,("pointLights["+std::to_string(i)+"].base.diffuseIntensity").c_str());
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID,("pointLights["+std::to_string(i)+"].position").c_str());
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID,("pointLights["+std::to_string(i)+"].constant").c_str());
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID,("pointLights["+std::to_string(i)+"].linear").c_str());
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID,("pointLights["+std::to_string(i)+"].exponent").c_str());
	}

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].base.base.colour").c_str());
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].base.base.ambientIntensity").c_str());
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].base.base.diffuseIntensity").c_str());
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].base.position").c_str());
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].base.constant").c_str());
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].base.linear").c_str());
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].base.exponent").c_str());
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].direction").c_str());
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID,("spotLights["+std::to_string(i)+"].edge").c_str());
	}

	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");
}

GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}
GLuint Shader::GetModelLocation()
{
	return uniformModel;
}
GLuint Shader::GetViewLocation()
{
	return uniformView;
}
GLuint Shader::GetAmbientColourLocation()
{
	return uniformDirectionalLight.uniformColour;
}
GLuint Shader::GetAmbientIntensityLocation()
{
	return uniformDirectionalLight.uniformAmbientIntensity;
}
GLuint Shader::GetDiffuseIntensityLocation()
{
	return uniformDirectionalLight.uniformDiffuseIntensity;
}
GLuint Shader::GetDirectionLocation()
{
	return uniformDirectionalLight.uniformDirection;
}
GLuint Shader::GetSpecularIntensityLocation()
{
	return uniformSpecularIntensity;
}
GLuint Shader::GetShininessLocation()
{
	return uniformShininess;
}
GLuint Shader::GetEyePositionLocation()
{
	return uniformEyePosition;
}

void Shader::SetDirectionalLight(DirectionalLight * dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight * pLight, unsigned int lightCount)
{
	if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
			uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

void Shader::SetSpotLights(SpotLight * sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) lightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, lightCount);

	for (size_t i = 0; i < lightCount; i++)
	{
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent,
			uniformSpotLight[i].uniformEdge);
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4 lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(lTransform));
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = 0;
	uniformProjection = 0;
}


void Shader::AddShader(GLuint theProgram, std::string shaderCode, GLenum shaderType)
{
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

Shader::~Shader()
{
	ClearShader();
}
