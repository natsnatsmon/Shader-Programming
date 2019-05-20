#include "stdafx.h"
#include "Renderer.h"
#include "LoadPng.h"
#include <Windows.h>
#include <cstdlib>
#include <cassert>

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	//m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	//m_SimpleVelShader = CompileShaders("./Shaders/SimpleVel.vs", "./Shaders/SimpleVel.fs");
	//m_Lecture6Shader = CompileShaders("./Shaders/Lecture3_5.vs", "./Shaders/Lecture3_5.fs");
	//m_Lecture7Shader = CompileShaders("./Shaders/Lecture4.vs", "./Shaders/Lecture4.fs");
	m_FillBGShader = CompileShaders("./Shaders/FillBG.vs", "./Shaders/FillBG.fs");
	m_FragShader = CompileShaders("./Shaders/Lecture6Frag.vs", "./Shaders/Lecture6Frag.fs");
	m_TextureShader = CompileShaders("./Shaders/Lecture8Texture.vs", "./Shaders/Lecture8Texture.fs");
	m_SpriteAnimShader = CompileShaders("./Shaders/SpriteAnim.vs", "./Shaders/SpriteAnim.fs");

	// Load Textures
	m_CometTexture = CreatePngTexture("./Textures/comet.png");
	m_SpriteAnimTexture = CreatePngTexture("./Textures/cutesprite.png");

	//Create VBOs
	CreateVertexBufferObjects();
	GenQuadsVBO(100000);
	CreateTextureTile();
	CreateTexture();
	CreateGridMesh();
}

void Renderer::CreateVertexBufferObjects()
{
	float size = 0.25f;
	float rect[]
		=
	{
		// x, y, z, value, u, v
		-size, -size, 0.f, 0.5f, 1.f, 1.f, 
		-size, size, 0.f, 0.5f, 0.f, 1.f,
		size, size, 0.f, 0.5f, 0.f, 0.f,//Triangle1
		
		-size, -size, 0.f, 0.5f, 1.f, 1.f,
		size, size, 0.f, 0.5f, 0.f, 0.f,
		size, -size, 0.f, 0.5f, 1.f, 0.f,//Triangle2

	}; // ARRAY 형태로 만들었당


	glGenBuffers(1, &m_VBORect); 	// 3개의 버텍스를 GPU에 옮기기 위해서는 ID를 부여해야함~ // ID, ID를 가질 버퍼
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect); // GL_ARRAY_BUFFER 말고도 다른것도 많지만 우린 ARRAY형태로 데이터를 만들었으니 ARRAY다! // 타겟, ID
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW); // 버퍼에 데이터를 올리기 전에 꼭 BindBuffer 하기~~!

	// 여기까지 끝나면 m_VBORect가 GPU에 올라가게 된다~~ rect안쓰고 m_VBORect를 쓰면 댐~~

	float rectColor[]
		=
	{
		// r, g, b, a
		1.f, 0.f, 0.f, 1.f,
		0.5f, 0.5, 0.f, 1.f,
		1.f, 0.5, 0.f, 1.f, //Triangle1

		1.f, 0.f, 0.f, 1.f,
		0.5, 0.5, 0.f, 1.f,
		0.5, 0.3f, 0.f, 1.f,//Triangle2

//		0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f, 0.f,
	}; // ARRAY 형태로 만들었당


	glGenBuffers(1, &m_VBORectColor); 	// 3개의 버텍스를 GPU에 옮기기 위해서는 ID를 부여해야함~ // ID, ID를 가질 버퍼
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor); // GL_ARRAY_BUFFER 말고도 다른것도 많지만 우린 ARRAY형태로 데이터를 만들었으니 ARRAY다! // 타겟, ID
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectColor), rectColor, GL_STATIC_DRAW); // 버퍼에 데이터를 올리기 전에 꼭 BindBuffer 하기~~!


	float rectTex[]
		=
	{
		// x, y, z, value, u, v
		-size, -size, 0.f, 0.5f, 0.f, 0.f,
		-size, size, 0.f, 0.5f, 0.f, 1.f,
		size, size, 0.f, 0.5f, 1.f, 1.f,//Triangle1

		-size, -size, 0.f, 0.5f, 0.f, 0.f,
		size, size, 0.f, 0.5f, 1.f, 1.f,
		size, -size, 0.f, 0.5f, 1.f, 0.f,//Triangle2

	}; // ARRAY 형태로 만들었당

	glGenBuffers(1, &m_VBORectTex); 	// 3개의 버텍스를 GPU에 옮기기 위해서는 ID를 부여해야함~ // ID, ID를 가질 버퍼
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex); // GL_ARRAY_BUFFER 말고도 다른것도 많지만 우린 ARRAY형태로 데이터를 만들었으니 ARRAY다! // 타겟, ID
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectTex), rectTex, GL_STATIC_DRAW); // 버퍼에 데이터를 올리기 전에 꼭 BindBuffer 하기~~!

}

void Renderer::GenQuadsVBO(int count)
{
	float quadSize = 0.005f;

	int verticesPerQuad = 6;
	int floatsPerVertex = 3 + 3 + 2 + 2 + 1 + 4; // x, y, z, vx, vy, vz, s, l, r, a, randValue, R, G, B, A
	int arraySize = count * floatsPerVertex * verticesPerQuad;

	float *vertices = new float[arraySize];

	for (int i = 0; i < count; ++i) {
		float randX, randY;
		float randVX, randVY, randVZ;
		float randST, randLT; // emit(start) time, life time
		float STMax = 5.f, LTMax = 3.f, LTMin = 1.f;
		float ratio, amp;
		float ratioMin = 2.f;
		float ratioThres = 4.f;
		float ampMin = -0.1f;
		float ampThres = 0.2;
		float randVal = 0.f;
		float randThres = 1.f;
		float randR, randG, randB, alpha;

		int index = i * verticesPerQuad * floatsPerVertex;

		randX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);

		randVX = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randVY = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randVZ = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);

		randST = ((float)rand() / (float)RAND_MAX) * STMax;
		randLT = ((float)rand() / (float)RAND_MAX) * LTMax + LTMin;
		
		randX = 0.f;  // Lecture 6을 위해
		randY = 0.f;  // Lecture 6을 위해
		
		ratio = ratioMin + ((float)rand() / (float)RAND_MAX * ratioThres);
		amp = ampMin + ((float)rand() / (float)RAND_MAX * ampThres);
		
		randVal = randVal + ((float)rand() / (float)(RAND_MAX) * randThres);

		randR = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randG = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		randB = 2.f * (((float)rand() / (float)RAND_MAX) - 0.5f);
		alpha = 1.f;

		vertices[index] = randX - quadSize; ++index; // x
		vertices[index] = randY - quadSize; ++index; // y
		vertices[index] = 0.f; ++index;          // z
		vertices[index] = randVX; ++index;        // x방향
		vertices[index] = randVY; ++index;        // y방향
		vertices[index] = 0.f; ++index;          // z방향
		vertices[index] = randST; ++index;        
		vertices[index] = randLT; ++index;        
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randVal; ++index;
		vertices[index] = randR; ++index;
		vertices[index] = randG; ++index;
		vertices[index] = randB; ++index;
		vertices[index] = alpha; ++index;



		vertices[index] = randX - quadSize; ++index;
		vertices[index] = randY + quadSize; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVX; ++index;
		vertices[index] = randVY; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randST; ++index;        
		vertices[index] = randLT; ++index;        
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randVal; ++index;
		vertices[index] = randR; ++index;
		vertices[index] = randG; ++index;
		vertices[index] = randB; ++index;
		vertices[index] = alpha; ++index;


		vertices[index] = randX + quadSize; ++index;
		vertices[index] = randY + quadSize; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVX; ++index;
		vertices[index] = randVY; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randST; ++index;        
		vertices[index] = randLT; ++index;        
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randVal; ++index;
		vertices[index] = randR; ++index;
		vertices[index] = randG; ++index;
		vertices[index] = randB; ++index;
		vertices[index] = alpha; ++index;


		vertices[index] = randX - quadSize; ++index;
		vertices[index] = randY - quadSize; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVX; ++index;
		vertices[index] = randVY; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randST; ++index;        
		vertices[index] = randLT; ++index;        
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randVal; ++index;
		vertices[index] = randR; ++index;
		vertices[index] = randG; ++index;
		vertices[index] = randB; ++index;
		vertices[index] = alpha; ++index;


		vertices[index] = randX + quadSize; ++index;
		vertices[index] = randY - quadSize; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVX; ++index;
		vertices[index] = randVY; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randST; ++index;        
		vertices[index] = randLT; ++index;        
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randVal; ++index;
		vertices[index] = randR; ++index;
		vertices[index] = randG; ++index;
		vertices[index] = randB; ++index;
		vertices[index] = alpha; ++index;


		vertices[index] = randX + quadSize; ++index;
		vertices[index] = randY + quadSize; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randVX; ++index;
		vertices[index] = randVY; ++index;
		vertices[index] = 0.f; ++index;
		vertices[index] = randST; ++index;        
		vertices[index] = randLT; ++index;
		vertices[index] = ratio; ++index;
		vertices[index] = amp; ++index;
		vertices[index] = randVal; ++index;
		vertices[index] = randR; ++index;
		vertices[index] = randG; ++index;
		vertices[index] = randB; ++index;
		vertices[index] = alpha; 
	}

	glGenBuffers(1, &m_VBOQuads); // CPU가 GPU를 바로 ACCESS 못하니까 id 부여해주는거임
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads); // ARRAY_BUFFER로 쓰겠다~
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*arraySize, vertices, GL_STATIC_DRAW);
	m_VBOQuads_vertexCount = count * verticesPerQuad;

	delete vertices;
}

void Renderer::CreateGridMesh()
{
	float basePosX = -0.5f;
	float basePosY = -0.5f;
	float targetPosX = 0.5f;
	float targetPosY = 0.5f;

	int pointCountX = 32;
	int pointCountY = 32;

	float width = targetPosX - basePosX;
	float height = targetPosY - basePosY;

	float* point = new float[pointCountX*pointCountY * 2];
	float* vertices = new float[(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3];
	m_VBOGridMesh_Count = (pointCountX - 1)*(pointCountY - 1) * 2 * 3;

	//Prepare points
	for (int x = 0; x < pointCountX; x++)
	{
		for (int y = 0; y < pointCountY; y++)
		{
			point[(y*pointCountX + x) * 2 + 0] = basePosX + width * (x / (float)(pointCountX - 1));
			point[(y*pointCountX + x) * 2 + 1] = basePosY + height * (y / (float)(pointCountY - 1));
		}
	}

	//Make triangles
	int vertIndex = 0;
	for (int x = 0; x < pointCountX - 1; x++)
	{
		for (int y = 0; y < pointCountY - 1; y++)
		{
			//Triangle part 1
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;

			//Triangle part 2
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + x) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[(y*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 0];
			vertIndex++;
			vertices[vertIndex] = point[((y + 1)*pointCountX + (x + 1)) * 2 + 1];
			vertIndex++;
			vertices[vertIndex] = 0.f;
			vertIndex++;
		}
	}

	glGenBuffers(1, &m_VBOGridMesh);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(pointCountX - 1)*(pointCountY - 1) * 2 * 3 * 3, vertices, GL_STATIC_DRAW);
}

void Renderer::CreateTextureTile() {
	static const GLulong checkerboard[] =
	{
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
	0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};

	glGenTextures(1, &m_CheckerboardTexture);
	glBindTexture(GL_TEXTURE_2D, m_CheckerboardTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkerboard);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Renderer::CreateTexture() {
	GLulong textureSmile[]
		=
	{
	  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	  0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	  0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	  0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	  0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	  0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	glGenTextures(1, &m_SmileTexture0);  // ID만 생성
	glBindTexture(GL_TEXTURE_2D, m_SmileTexture0);  // 요 아이디는 2차원 텍스쳐로 사용할 것이다.. 라고 명시해주는 거
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile);  // 채널당 비트수, RGPA 순서 그런거.. 세팅하고 GPU에 실제적으로 올려주는 API~~!!!!!~!~!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // 샘플링 하는 방식
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLulong textureSmile1[]
		=
	{
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};
	glGenTextures(1, &m_SmileTexture1);
	glBindTexture(GL_TEXTURE_2D, m_SmileTexture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	GLulong textureSmile2[]
		=
	{
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};
	glGenTextures(1, &m_SmileTexture2);
	glBindTexture(GL_TEXTURE_2D, m_SmileTexture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLulong textureSmile3[]
		=
	{
	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};
	glGenTextures(1, &m_SmileTexture3);
	glBindTexture(GL_TEXTURE_2D, m_SmileTexture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmile3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	GLulong textureSmileTotal[]
		=
	{
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,

	0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00,
	0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00,
	0xFFFFFFFF, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFF0000FF, 0xFF0000FF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF0000, 0xFFFF0000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
	};

	glGenTextures(1, &m_SmileSadTexture);
	glBindTexture(GL_TEXTURE_2D, m_SmileSadTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 48, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSmileTotal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//쉐이더 오브젝트 생성
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = (GLint)strlen(pShaderText);
	//쉐이더 코드를 쉐이더 오브젝트에 할당
	glShaderSource(ShaderObj, 1, p, Lengths);

	//할당된 쉐이더 코드를 컴파일
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj 가 성공적으로 컴파일 되었는지 확인
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL 의 shader log 데이터를 가져옴
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram 에 attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //빈 쉐이더 프로그램 생성

	if (ShaderProgram == 0) { //쉐이더 프로그램이 만들어졌는지 확인
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs 가 vs 안으로 로딩됨
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs 가 fs 안으로 로딩됨
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram 에 vs.c_str() 버텍스 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram 에 fs.c_str() 프레그먼트 쉐이더를 컴파일한 결과를 attach함
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach 완료된 shaderProgram 을 링킹함
	glLinkProgram(ShaderProgram);

	//링크가 성공했는지 확인
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program 로그를 받아옴
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done.\n";

	return ShaderProgram;
}
unsigned char * Renderer::loadBMPRaw(const char * imagepath, unsigned int& outWidth, unsigned int& outHeight)
{
	std::cout << "Loading bmp file " << imagepath << " ... " << std::endl;
	outWidth = -1;
	outHeight = -1;
	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = NULL;
	fopen_s(&file, imagepath, "rb");
	if (!file)
	{
		std::cout << "Image could not be opened, " << imagepath << " is missing. " << std::endl;
		return NULL;
	}

	if (fread(header, 1, 54, file) != 54)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (header[0] != 'B' || header[1] != 'M')
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1E]) != 0)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	if (*(int*)&(header[0x1C]) != 24)
	{
		std::cout << imagepath << " is not a correct BMP file. " << std::endl;
		return NULL;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	outWidth = *(int*)&(header[0x12]);
	outHeight = *(int*)&(header[0x16]);

	if (imageSize == 0)
		imageSize = outWidth * outHeight * 3;

	if (dataPos == 0)
		dataPos = 54;

	data = new unsigned char[imageSize];

	fread(data, 1, imageSize, file);

	fclose(file);

	std::cout << imagepath << " is succesfully loaded. " << std::endl;

	return data;
}

GLuint Renderer::CreatePngTexture(char * filePath)
{
	//Load Pngs: Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filePath);
	if (error != 0)
	{
		lodepng_error_text(error);
		assert(error == 0);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

	return temp;
}

GLuint Renderer::CreateBmpTexture(char * filePath)
{
	//Load Bmp: Load file and decode image.
	unsigned int width, height;
	unsigned char * bmp
		= loadBMPRaw(filePath, width, height);

	if (bmp == NULL)
	{
		std::cout << "Error while loading bmp file : " << filePath << std::endl;
		assert(bmp != NULL);
		return -1;
	}

	GLuint temp;
	glGenTextures(1, &temp);

	glBindTexture(GL_TEXTURE_2D, temp);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

	return temp;
}

float time = 0.f;
void Renderer::Test()
{
	glUseProgram(m_SolidRectShader);

	time += .1f;
	if (time > 1.0f) {
		time = 0.f;
	}
	GLuint uTime = glGetUniformLocation(m_SolidRectShader, "u_Time");
	glUniform1f(uTime, time);

	GLuint aPos = glGetAttribLocation(m_SolidRectShader, "a_Position");
	GLuint aCol = glGetAttribLocation(m_SolidRectShader, "a_Color");

//	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);
	// 얘는 ARRAY버퍼(m_VBORect)에 올린 그 내용물을 3개씩 끊어서 하나의 버텍스를 구성함
	// sizeof(float) * 3 = stride인데 이게 뭐냐면 다음꺼 읽으려면 얼만큼 떨어진거 읽어~ 라고 알려주는 거임
	// 읽을 크기 = 3, 읽을 시점 = sizeof(float) * 3
	// 읽을 시점이 왜 필요하냐?? 이거 x, y, z 읽고 바로 다음이 x가 아닐수도 있기 때문임! u, v같은 텍스쳐 좌표 등이 있을수도 있어서 그렇다!

	glEnableVertexAttribArray(aCol);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectColor);
	glVertexAttribPointer(aCol, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	// 버텍스 6개를 그려라...!
	// 버텍스 1개당 3개의 포인트가 필요하니까 총 6 * 3 = 18개의 float point가 필요하게 된다~

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

}

void Renderer::Lecture2() {
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_vertexCount);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture3() {
	glUseProgram(m_SolidRectShader);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOGridMesh);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_LINES, 0, m_VBOGridMesh_Count);

	glDisableVertexAttribArray(0);
}

float time_lec4 = 0.f;
void Renderer::Lecture4() {
	glUseProgram(m_SimpleVelShader);

	time_lec4 += 0.00005f;
	//if (time_lec4 > 1) {
	//	time_lec4 = 0.f;
	//}

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	glUniform1f(uTime, time_lec4);


	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Velocity");
	
	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
									
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	// Shader파일에 내 변수들을 보내주는 역할!!!!!! 드디어 이해함
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 3)); // 시작지점을 밀어줘야겠죵? 맨 끝에 GLvoid해서 하면 댄다
	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_vertexCount);

	glDisableVertexAttribArray(0);
}

void Renderer::Lecture5() {
	glUseProgram(m_SimpleVelShader);

	time_lec4 += 0.0005f;
	//if (time_lec4 > 1) {
	//	time_lec4 = 0.f;
	//}

	GLuint uTime = glGetUniformLocation(m_SimpleVelShader, "u_Time");
	GLuint uRepeat = glGetUniformLocation(m_SimpleVelShader, "u_Repeat");
	glUniform1f(uTime, time_lec4);
//	glUniform1f(uRepeat, uRepeat);


	GLuint aPos = glGetAttribLocation(m_SimpleVelShader, "a_Position");
	GLuint aVel = glGetAttribLocation(m_SimpleVelShader, "a_Velocity");
	GLuint aTime = glGetAttribLocation(m_SimpleVelShader, "a_StartLifeTime");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aTime);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	// (0x, 1y, 2z, 3vx, 4vy, 5vz, 6st, 7lt, 8x, 9y ... )
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 3)); // 시작지점을 밀어줘야겠죵? 맨 끝에 GLvoid해서 하면 댄다
	glVertexAttribPointer(aTime, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (GLvoid*)(sizeof(float) * 6));
	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_vertexCount);

	glDisableVertexAttribArray(0);
}

float time_lec6 = 0.f;
void Renderer::Lecture6() {
	
	// 알파 블렌딩을 켜는 API
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	
	GLuint shader = m_Lecture6Shader;
	glUseProgram(shader);


	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, time_lec4);
	
	time_lec4 += 0.005f;



	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aVel = glGetAttribLocation(shader, "a_Velocity");
	GLuint aTime = glGetAttribLocation(shader, "a_Time");
	GLuint aValue = glGetAttribLocation(shader, "a_Value");
	GLuint aColor = glGetAttribLocation(shader, "a_Color");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aVel);
	glEnableVertexAttribArray(aTime);
	glEnableVertexAttribArray(aValue);
	glEnableVertexAttribArray(aColor);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOQuads);

	// (0x, 1y, 2z, 3vx, 4vy, 5vz, 6st, 7lt, 8x, 9y ... )
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, 0);
	glVertexAttribPointer(aVel, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 3));
	glVertexAttribPointer(aTime, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 6));
	glVertexAttribPointer(aValue, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 10));
	glVertexAttribPointer(aColor, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 15, (GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_VBOQuads_vertexCount);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aVel);
	glDisableVertexAttribArray(aTime);
	glDisableVertexAttribArray(aValue);
	glDisableVertexAttribArray(aColor);
}

float time_lec7 = 0.f;
void Renderer::Lecture7() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint shader = m_Lecture7Shader;
	glUseProgram(shader);

	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, time_lec7);

	time_lec7 += 0.0001f;
	
	GLfloat enemyPoints[] = { 0.f, 0.f, 0.5f, 0.5f, 0.25f, -0.25f, -0.2f, -0.3f, 0.3f, -0.4f };
	GLfloat uPoints = glGetUniformLocation(shader, "u_EnemyPoints");
	glUniform2fv(uPoints, 5, enemyPoints); // uniform 형식으로 주려면 uniform의 location, counts, source 순으로~
	
	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aUV = glGetAttribLocation(shader, "a_UV");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aUV);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aUV, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aUV);

}

void Renderer::FillBG(float alpha) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint shader = m_FillBGShader;
	glUseProgram(shader);

	GLfloat uAlpha = glGetUniformLocation(shader, "u_Alpha");
	glUniform1f(uAlpha, alpha);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");

	glEnableVertexAttribArray(aPos);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
}

float time_lec8 = 0.f;
void Renderer::Lecture8() {

	GLuint shader = m_TextureShader;
	glUseProgram(shader);


	GLuint uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, time_lec8);
	time_lec8 += 0.0001f;

	GLuint uTex = glGetUniformLocation(shader, "u_Texture");
	glUniform1i(uTex, 0); // 0번에 바인드 했다..!
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_CometTexture);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_TexPos");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex);

	// (0x, 1y, 2z, 3vx, 4vy, 5vz, 6st, 7lt, 8x, 9y ... )
	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
}

float time_lec9 = 0;
void Renderer::Lecture9(GLuint tex) {
	GLuint shader = m_FragShader;
	glUseProgram(shader);

	time_lec9 += 0.001f;
	GLfloat uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, time_lec9);

	GLuint aPos = glGetAttribLocation(shader, "a_Position");
	GLuint aTex = glGetAttribLocation(shader, "a_TexPos");

	glEnableVertexAttribArray(aPos);
	glEnableVertexAttribArray(aTex);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex);

	glVertexAttribPointer(aPos, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glVertexAttribPointer(aTex, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(sizeof(float) * 4));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPos);
	glDisableVertexAttribArray(aTex);
}

float g_TimeStamp = 0.f;
void Renderer::Lecture10(int * number) {
	GLuint shader = m_TextureShader;
	glUseProgram(shader);

	GLuint uNumber = glGetUniformLocation(shader, "u_number");
	glUniform1iv(uNumber, 3, number);

	/*
	int uniformTex0 = glGetUniformLocation(shader, "u_TexSampler0");
	glUniform1i(uniformTex0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_SmileTexture0);

	int uniformTex1 = glGetUniformLocation(shader, "u_TexSampler1");
	glUniform1i(uniformTex1, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_SmileTexture1);

	int uniformTex2 = glGetUniformLocation(shader, "u_TexSampler2");
	glUniform1i(uniformTex2, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_SmileTexture2);

	int uniformTex3 = glGetUniformLocation(shader, "u_TexSampler3");
	glUniform1i(uniformTex3, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_SmileTexture3);
	*/

	int uniformAniTex = glGetUniformLocation(shader, "u_aniTexSampler");
	glUniform1i(uniformAniTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_SmileSadTexture);

	g_TimeStamp += 1.f;
	if (g_TimeStamp > 5.f)
		g_TimeStamp = 0.f;

	float uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_TimeStamp);
	

	int aPosition = glGetAttribLocation(shader, "a_Position");
	int aTexPos = glGetAttribLocation(shader, "a_TexPos");

	glEnableVertexAttribArray(aPosition);
	glEnableVertexAttribArray(aTexPos);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex);
	glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glVertexAttribPointer(aTexPos, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(4 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPosition);
	glDisableVertexAttribArray(aTexPos);

}

float g_Time = 0.f;
void Renderer::LecSpriteAnim(float num) {
	GLuint shader = m_SpriteAnimShader;
	glUseProgram(shader);

	//GLuint uNumber = glGetUniformLocation(shader, "u_number");
	//glUniform1iv(uNumber, 3, number);

	int uniformAnimTex = glGetUniformLocation(shader, "u_animTexture");
	glUniform1i(uniformAnimTex, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_SpriteAnimTexture);

	g_TimeStamp += 1.f;
	if (g_TimeStamp > 5.f)
		g_TimeStamp = 0.f;

	float uTime = glGetUniformLocation(shader, "u_Time");
	glUniform1f(uTime, g_TimeStamp);

	int uResolX = glGetUniformLocation(shader, "u_resolX");
	glUniform1f(uResolX, 12.f);
	int uResolY = glGetUniformLocation(shader, "u_resolY");
	glUniform1f(uResolY, 8.f);
	
	int uNum = glGetUniformLocation(shader, "u_number");
	glUniform1f(uNum, num);

	int aPosition = glGetAttribLocation(shader, "a_Position");
	int aTexPos = glGetAttribLocation(shader, "a_TexPos");

	glEnableVertexAttribArray(aPosition);
	glEnableVertexAttribArray(aTexPos);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBORectTex);
	glVertexAttribPointer(aPosition, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glVertexAttribPointer(aTexPos, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(4 * sizeof(float)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(aPosition);
	glDisableVertexAttribArray(aTexPos);

}