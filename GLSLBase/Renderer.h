#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Dependencies\glew.h"
#include "Dependencies\wglew.h"
#include "Dependencies\glm/glm.hpp"
#include "Dependencies\glm/gtc/matrix_transform.hpp"
#include "Dependencies\glm/gtx/euler_angles.hpp"

class Renderer
{
public:
	Renderer(int windowSizeX, int windowSizeY);
	~Renderer();

	GLuint CreatePngTexture(char * filePath);
	GLuint CreateBmpTexture(char * filePath);
	   
	void Test();
	void Lecture2();
	void Lecture3();
	void Lecture4();
	void Lecture5();
	void Lecture6();
	void Lecture7();
	void Lecture8();
	void Lecture9(GLuint tex);
	void Lecture10();

	void FillBG(float alpha);

	void GenQuadsVBO(int count);
	void CreateGridMesh();
	void CreateTextureTile();
	void CreateTexture();

private:
	void Initialize(int windowSizeX, int windowSizeY);
	bool ReadFile(char* filename, std::string *target);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(char* filenameVS, char* filenameFS);
	void CreateVertexBufferObjects(); 
	unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight);

	bool m_Initialized = false;
	
	unsigned int m_WindowSizeX = 0;
	unsigned int m_WindowSizeY = 0;

	GLuint m_VBORect = 0;
	GLuint m_VBORectTex = 0;
	
	GLuint m_VBOLecture = 0;
	GLuint m_VBOQuads = 0;
	int m_VBOQuads_vertexCount = 0;

	GLuint m_VBOGridMesh = 0;
	int m_VBOGridMesh_Count = 0;

	GLuint m_VBORectColor = 0;

	// Shaders
	GLuint m_SolidRectShader = 0;
	GLuint m_SimpleVelShader = 0;
	GLuint m_Lecture6Shader = 0;
	GLuint m_Lecture7Shader = 0;
	GLuint m_FillBGShader = 0;
	GLuint m_TextureShader = 0;
	GLuint m_FragShader = 0;


	// Textures
	GLuint m_CheckerboardTexture = 0;
	GLuint m_CometTexture = 0;
	GLuint m_SmileTexture0 = 0;
	GLuint m_SmileTexture1 = 0;
	GLuint m_SmileTexture2 = 0;
	GLuint m_SmileTexture3 = 0;
	GLuint m_SmileSadTexture = 0;
};

