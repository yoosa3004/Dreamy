////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_minimapCamera = 0;
	m_Shader = 0; 

	m_Light = 0;

	m_Model_Circle = 0;
	m_Model_Plane = 0;
	m_Model_Plane2 = 0;

	m_Model_Cube = 0;
	m_Model_Cube2 = 0;
	m_Model_Cube3 = 0;
	m_Model_CircleList = 0;

	m_Model_Mirror = 0;

	m_2D_Love = 0;
	m_Loading = 0;

	m_Title = 0;

	m_Frustum = 0;

	m_RTT = 0;
	m_RTTTexture = 0;

	m_Terrain = 0;
	m_TerrainShader = 0;
	//m_QuadTree = 0;

	m_Sky = 0;


	F1pressed = false;

	CharacterPos = { 512.5f, 30.0f, 310.0f };
	CharacterRot = { 0.0f, 0.0f, 0.0f };


}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}
GraphicsClass::~GraphicsClass()
{
}


/*----------------------------------------------------------------------------------------
이름 : Initialize()
용도 : D3D객체, 카메라, 메인화면 객체 초기화
------------------------------------------------------------------------------------------*/
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// D3D객체 생성
	//-------------------------------------------------------------------------------------
	m_D3D = new D3DClass;

	if (!m_D3D) { return false; }

	/* Direct3D 객체를 초기화합니다.
	 이 함수에서 화면의 너비, 높이, 윈도우 핸들, Graphicsclass.h에 정의된 네 개의 전역 변수들을 전달한다.
	 D3DCLASS에서 이 변수들을 사용하여 Direct3D 시스템을 설정한다. */
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result) { MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK); return false; }

	//--------------------------------------------------------------------------------------

	// 카메라객체 생성
	//-------------------------------------------------------------------------------------
	m_Camera = new CameraClass;

	if (!m_Camera) { return false; }

	//글꼴의 위치
	m_Camera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -1.0f));
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);
	//-------------------------------------------------------------------------------------


	// 셰이더 객체 생성
	//-------------------------------------------------------------------------------------
	m_Shader = new ShaderManagerClass;
	if (!m_Shader) { return false; }

	result = m_Shader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result) { MessageBox(hwnd, L"Could not initialize the color shader object", L"Error", MB_OK); return false; }
	//-------------------------------------------------------------------------------------


	// 메인이미지 객체 생성
	//-------------------------------------------------------------------------------------
	m_2D_Love = new ImageClass;

	result = m_2D_Love->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Dreamy/Main.jpg", 1600, 900);
	if (!result) { MessageBox(hwnd, L"Could not initialize model object", L"Error", MB_OK); return false; }
	//-------------------------------------------------------------------------------------

	return true;
}


/*----------------------------------------------------------------------------------------
이름 : Loading()
용도 : 실제로 렌더링 되는 애들 초기화
------------------------------------------------------------------------------------------*/
bool GraphicsClass::Loading(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// 로딩이미지 객체 생성
	//-------------------------------------------------------------------------------------
	m_Loading = new ImageClass;

	result = m_Loading->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Dreamy/Loading.jpg", 1600, 900);
	if (!result) { MessageBox(hwnd, L"Could not initialize model object", L"Error", MB_OK); return false; }
	//-------------------------------------------------------------------------------------

	// 로딩이미지 출력
	RenderLoadingScene();


	// 카메라 객체 생성
	//-------------------------------------------------------------------------------------
	m_minimapCamera = new CameraClass;
	if (!m_minimapCamera) { return false; }

	//미니맵 카메라 
	m_minimapCamera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -2.5f));
	m_minimapCamera->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	//-------------------------------------------------------------------------------------



	// Text객체 생성
	//-------------------------------------------------------------------------------------
	m_Title = new TextClass;

	if (!m_Title) { return false; }

	result = m_Title->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result) { MessageBox(hwnd, L"Could not initialize TitleText", L"Error", MB_OK); return false; }
	//-------------------------------------------------------------------------------------


	// 3D모델 객체 생성
	//-------------------------------------------------------------------------------------
	m_Model_Circle = new ModelClass;

	result = m_Model_Circle->Initialize(m_D3D->GetDevice(), "../Dreamy/sphere.txt", L"../Dreamy/seafloor.dds");
	if (!result) { MessageBox(hwnd, L"Could not initialize model object", L"Error", MB_OK); return false; }

	m_Model_Plane = new ModelClass;

	result = m_Model_Plane->InitializeTriple(m_D3D->GetDevice(), "../Dreamy/plane01.txt", L"../Dreamy/dirt01.dds", L"../Dreamy/stone01.dds", L"../Dreamy/alpha01.dds");
	if (!result) { MessageBox(hwnd, L"Could not initialize mo456456del object", L"Error", MB_OK); return false; }


	m_Model_Cube = new ModelClass;

	result = m_Model_Cube->InitializeBumpMap(m_D3D->GetDevice(), "../Dreamy/cube.txt", L"../Dreamy/stone01.dds", L"../Dreamy/bump01.dds");
	if (!result) { MessageBox(hwnd, L"Could not initializddwqdkqwpdkp[l object", L"Error", MB_OK); return false; }

	//result = m_Model_Cube->Initialize(m_D3D->GetDevice(), "../Dreamy/cube.txt", L"../Dreamy/seafloor.dds");
	//if (!result) { MessageBox(hwnd, L"Could not initialize model object", L"Error", MB_OK); return false; }

	m_Model_Cube2 = new ModelClass;

	result = m_Model_Cube2->Initialize(m_D3D->GetDevice(), "../Dreamy/cube.txt", L"../Dreamy/seafloor.dds");
	if (!result) { MessageBox(hwnd, L"Could not initialize model object", L"Error", MB_OK); return false; }

	m_Model_Cube3 = new ModelClass;

	result = m_Model_Cube3->InitializeSpecMap(m_D3D->GetDevice(), "../Dreamy/cube.txt", L"../Dreamy/stone02.dds", L"../Dreamy/bump02.dds", L"../Dreamy/spec02.dds");
	if (!result) { MessageBox(hwnd, L"Could not tranlsmad object", L"Error", MB_OK); return false; }

	m_Model_Plane2 = new ModelClass;

	result = m_Model_Plane2->Initialize(m_D3D->GetDevice(), "../Dreamy/plane01.txt", L"../Dreamy/fog01.png");
	if (!result) { MessageBox(hwnd, L"Could not initialize fogplane object", L"Error", MB_OK); return false; }

	m_Model_Mirror = new ModelClass;

	result = m_Model_Mirror->Initialize(m_D3D->GetDevice(), "../Dreamy/floor.txt", L"../Dreamy/blue01.dds");
	if (!result) { MessageBox(hwnd, L"Could not initialize Mirror object", L"Error", MB_OK); return false; }

	//-------------------------------------------------------------------------------------

	// Frustum 객체 생성
	//-------------------------------------------------------------------------------------
	m_Frustum = new FrustumClass;
	if (!m_Frustum) { return false; }
	//-------------------------------------------------------------------------------------


	//Terrain, sky 객체 생성
	//-------------------------------------------------------------------------------------	

	m_Terrain = new TerrainClass;
	if (!m_Terrain) { return false; }

	result = m_Terrain->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), "../Dreamy/setup.txt", "../Dreamy/dirt01d.tga", "../Dreamy/dirt01n.tga" );
	if (!result) { MessageBox(hwnd, L"Could not initialize Terrain object", L"Error", MB_OK); return false; }


	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader) { return false; }

	m_TerrainShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result) { MessageBox(hwnd, L"Could not initialize TerrainShader object", L"Error", MB_OK); return false; }
	
	//m_QuadTree = new QuadtreeClass;
	//if (!m_QuadTree) { return false; }
	//
	//result = m_QuadTree->Initialize(m_Terrain, m_D3D->GetDevice());
	//if (!result) { MessageBox(hwnd, L"Could not initialize QuadTree object", L"Error", MB_OK); return false; }

	m_Sky = new SkyClass;
	if (!m_Sky) { return false; }

	result = m_Sky->InitializeSky(m_D3D->GetDevice());
	if (!result) { MessageBox(hwnd, L"Could not initialize Sky object", L"Error", MB_OK); return false; }

	result = m_Sky->InitializeCloud(m_D3D->GetDevice(), L"../Dreamy/cloud001.dds", L"../Dreamy/cloud002.dds");
	if (!result) { MessageBox(hwnd, L"Could not initialize Cloud object", L"Error", MB_OK); return false; }
	//-------------------------------------------------------------------------------------

	// 빛 객체 생성
	//-------------------------------------------------------------------------------------
	m_Light = new LightClass;
	if (!m_Light) { return false; }

	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(-0.5f, -1.0f, -0.5f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f); // 반사강도값이 낮을수록 반사광 효과가 커진다!
	//-------------------------------------------------------------------------------------

    // ModelList객체 생성
    //-------------------------------------------------------------------------------------
	m_Model_CircleList = new ModelManagerClass;
	if (!m_Model_CircleList) { return false; }

	// 구 4개를 만들자!
	m_Model_CircleList->Initialize(4);
	if (!result) { MessageBox(hwnd, L"리스트 만드는데 실패함", L"Error", MB_OK); return false; }

	//-------------------------------------------------------------------------------------



	// RTT 객체 생성
	//-------------------------------------------------------------------------------------
	m_RTT = new RTTClass;
	if (!m_RTT) { return false; }

	// 모든 화면을 텍스처에 그리고 싶기 때문에 화면의 너비와 높이를 텍스쳐의 크기로 지정했디.

	result = m_RTT->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result) { MessageBox(hwnd, L"리스트 만드는데 실패함", L"Error", MB_OK); return false; }
	//-------------------------------------------------------------------------------------

	// RTT 텍스처 객체 생성
	//-------------------------------------------------------------------------------------
	m_RTTTexture = new RTTTextureClass;
	if (!m_RTTTexture) { return false; }

	result = m_RTTTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, 400, 225);
	if (!result) { MessageBox(hwnd, L"리스트 만드는데 실패함", L"Error", MB_OK); return false; }
	//-------------------------------------------------------------------------------------

	return true;
}

// 모든 그래픽 객체의 종료가 여기에서 일어난다.
void GraphicsClass::Shutdown()
{
	if (m_Title)	{ m_Title->Shutdown();	delete m_Title;	m_Title = 0;}
	if (m_2D_Love)	{ m_2D_Love->Shutdown(); delete m_2D_Love; m_2D_Love = 0;}
	//if (m_QuadTree) { m_QuadTree->Shutdown(); delete m_QuadTree; m_QuadTree = 0; }
	// Release the texture manager object.
	if (m_TerrainShader) { m_TerrainShader->Shutdown(); delete m_TerrainShader; m_TerrainShader = 0; }
	if (m_Loading) { m_Loading->Shutdown(); delete m_Loading; m_Loading = 0; }
	if (m_RTT) { m_RTT->Shutdown(); delete m_RTT; m_RTT = 0; }
	if (m_RTTTexture) { m_RTTTexture->Shutdown(); delete m_RTTTexture; m_RTTTexture = 0; }
	if (m_Frustum) { delete m_Frustum; m_Frustum = 0; }
	if (m_Terrain) { m_Terrain->Shutdown(); delete m_Terrain; m_Terrain = 0; }
	if (m_Sky) { m_Sky->Shutdown(); delete m_Sky; m_Sky = 0; }
	if (m_Model_CircleList) { m_Model_CircleList->Shutdown(); delete m_Model_CircleList; m_Model_CircleList = 0; }
	if (m_Model_Cube) { m_Model_Cube->Shutdown(); delete m_Model_Cube; m_Model_Cube = 0; }
	if (m_Model_Cube2) { m_Model_Cube2->Shutdown(); delete m_Model_Cube2; m_Model_Cube2 = 0; }
	if (m_Model_Cube3) { m_Model_Cube3->Shutdown(); delete m_Model_Cube3; m_Model_Cube3 = 0; }
	if (m_Model_Mirror) { m_Model_Mirror->Shutdown(); delete m_Model_Mirror; m_Model_Mirror = 0; }
	if (m_Light)	{ delete m_Light; m_Light = 0; }
	if (m_Shader)	{ m_Shader->Shutdown(); delete m_Shader; m_Shader = 0;}
	if (m_Model_Circle)	{ m_Model_Circle->Shutdown(); delete m_Model_Circle;m_Model_Circle = 0;}
	if (m_Model_Plane)	{ m_Model_Plane->Shutdown(); delete m_Model_Plane;m_Model_Plane = 0;}
	if (m_Model_Plane2) { m_Model_Plane2->Shutdown(); delete m_Model_Plane2; m_Model_Plane2 = 0; }
	if (m_Camera)	{ delete m_Camera; m_Camera = 0;}
	if (m_minimapCamera) { delete m_minimapCamera; m_minimapCamera = 0; }
	if (m_D3D){ m_D3D->Shutdown(); delete m_D3D; m_D3D = 0; }
	
	return;
}

/*----------------------------------------------------------------------------------------
이름 : Frame()
용도 : 
- SystemClass에서 프레임 마다 호출된다.
- 마우스포인터 움직이는것 할 수 있음
- fps,cpu,frametime재서 text로 출력.
- 물체 돌릴 수 있음
- 각 프레임 마다 구름의 변환.
------------------------------------------------------------------------------------------*/
// 매 호출마다 Render함수를 부른다.
bool GraphicsClass::Frame(int fps, int cpu, float frameTime, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	bool result, foundHeight;
	D3DXVECTOR3 CameraPos;
	float height, Characterheight;

	// 마우스 포인터위치 잡을 수 있게
	//result = m_Title->SetMousePosition(mouseX, mouseY, m_D3D->GetDeviceContext());

	// fps세팅
	result = m_Title->SetFps(fps, m_D3D->GetDeviceContext());
	if (!result) { return false; }

	// cpu세팅
	result = m_Title->SetCpu(cpu, m_D3D->GetDeviceContext());
	if (!result) { return false; }


	//m_Camera->SetRotation(rot);


	 //높이 기반 카메라 이동을 위함
	//foundHeight = m_Terrain->GetHeightAtPosition(pos.x, pos.z, height);
	//if (foundHeight)
	//{
	//	m_Camera->SetPosition(D3DXVECTOR3(pos.x, height + 5.0f, pos.z));
	//}
	//
	//else
	//{
	//	m_Camera->SetPosition(pos);
	//}

	//캐릭터 위치 초기화
	CharacterPos.x = pos.x;
	CharacterPos.z = pos.z+5.5f;


	//캐릭터 회전 초기화
	CharacterRot = D3DXVECTOR3(rot.x, rot.y, rot.z);

	//높이 기반 캐릭터 이동을 위함
	foundHeight = m_Terrain->GetHeightAtPosition(CharacterPos.x, CharacterPos.z, Characterheight);
	if (foundHeight)
	{
		CharacterPos.y = Characterheight + 1.0f;

	}

	//카메라 위치 초기화
	m_Camera->SetPosition(D3DXVECTOR3(pos.x, Characterheight + 10.0f, pos.z-20.0f));
	//m_Camera->SetPosition(D3DXVECTOR3(512.5f, 30.0f, 300.0f));
	m_Camera->SetRotation(D3DXVECTOR3(16.0f+rot.x, 0, 0));

	//구름의 프레임 처리를 수행한다.
	m_Sky->Frame(frameTime*0.00009f, 0.0f, frameTime*0.00008f, 0.0f);

	//물체 일정 속도로 회전시키고 싶을 때.
	//static float rotation = 0.0f;
	//rotation += (float)D3DX_PI * 0.01f;
	//if (rotation > 360.0f)
	//	rotation -= 360.0f;

	return true;
}


/*----------------------------------------------------------------------------------------
이름 : Render()
용도 : 러닝씬과 RTT씬을 그린다.
- 반사를 할 때는 우선 RTT를 먼저하고 러닝씬을 로드해야 혼합된다.
------------------------------------------------------------------------------------------*/
bool GraphicsClass::Render(bool Pressed)
{
	bool result;

	//RTT시작
	//-------------------------------------------------------------------------------------
	result = RenderToTexture(); //원하는 씬을 텍스처에 그린다.
	if (!result) { return false; }
	//-------------------------------------------------------------------------------------


	// 메인씬 그림
	//-------------------------------------------------------------------------------------
	result = RenderRunningScene(Pressed);
	if (!result) { return false; }
	//-------------------------------------------------------------------------------------
	return true;
}

/*----------------------------------------------------------------------------------------
이름 : RenderRunningScene()
용도 :
- 1. 카메라의 위치를 토대로 뷰 행렬을 만들기 위해 카메라의 render함수를 호출한다.
- 2. 뷰 행렬을 만들고 복사본을 가져온다.
- 3. d3dclass객체로 부터 월드 행렬과 투영 행렬을 복사해온다.
- 4. modelclass의 render()함수를 호출하여 그래픽 파이프라인에 삼각형 모델을 그리도록 한다.
- 5. 여기서 준비한 정점들로 셰이더를 호출하여 셰이더는 모델 정보와 정점을 배치시키기 위한 세 행렬을 사용하여 정점들을 그려낸다.
- 6. 삼각형이 백버퍼에 그려진다.

- ModelClass, ImageClass, TextClass의 Render()은 정점/인덱스 버퍼를 그래픽스 파이프라인에 묶는 I/A단계 까지만 한다.
- 이 기하구조를 그리는(출력하는)건 ShaderClass의 Render()이다!

- 안개 구현 시 백버퍼를 안개의 색으로 지우는것은 매우 중요한 부분이다.
- FogEnd가 멀어질수록(+될수록) 밝아진다 = 물체의 위치(정점셰이더에서 cameraposition)가 FogENd와 가까워질수록(+) FogFactor가 커져서 텍스처가 가려진다.

안개는 ...나중에 ... 높이맵 했을 때 높이맵에만 셰이더를 씌우는게 나을듯.
------------------------------------------------------------------------------------------*/
bool GraphicsClass::RenderRunningScene(bool Pressed)
{
	
	float fogColor, fogStart, fogEnd;
	float pitch = CharacterRot.x * 0.0174532925f;
	float yaw = CharacterRot.y * 0.0174532925f;
	float roll = CharacterRot.z * 0.0174532925f;
	D3DXVECTOR3 cameraPosition;

	D3DXMATRIX TerrainworldMatrix,SkyworldMatrix, PlaneworldMatrix, Plane2worldMatrix, CircleworldMatrix, Cube1worldMatrix, Cube2worldMatrix, Cube3worldMatrix, MirrorworldMatrix;
	D3DXMATRIX TranslationMatrix, TranslationMatrix2, RotationMatrix;
	D3DXMATRIX viewMatrix, projectionMatrix, orthoMatrix, reflectionMatrix;

	D3DXMATRIX TextworldMatrix;

	int CircleCount, Circleindex;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color; // CircleDiffuseColor
	bool Circlerender, result;

	// 텍스처 이동
	static float textureTranslation = 0.0f;
	textureTranslation += 0.0001f;
	if (textureTranslation > 1.0f) { textureTranslation -= 1.0f; }

	//RTT시작
	//-------------------------------------------------------------------------------------
	//result = RenderToTexture(); //원하는 씬을 텍스처에 그린다.
	//if (!result) { return false; }

	//안개 객체 초기화
	//fogEnd가 멀어질수록 밝아짐
	//-------------------------------------------------------------------------------------
	fogColor = 0.0f;
	fogStart = 0.0f;
	fogEnd = 10.0f;
	//-------------------------------------------------------------------------------------

	//시작
	//-------------------------------------------------------------------------------------
	m_D3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	//카메라 위치
	//-------------------------------------------------------------------------------------
	m_Camera->Render();
	cameraPosition = m_Camera->GetPosition();

	//기본 월드,뷰,투영,정사영 행렬 획득
	//-------------------------------------------------------------------------------------
	m_Camera->GetViewMatrix(viewMatrix);
	reflectionMatrix = m_Camera->GetReflectionViewMatrix();

	m_D3D->GetWorldMatrix(TerrainworldMatrix);
	m_D3D->GetWorldMatrix(SkyworldMatrix);
	m_D3D->GetWorldMatrix(PlaneworldMatrix);
	m_D3D->GetWorldMatrix(Plane2worldMatrix);
	m_D3D->GetWorldMatrix(CircleworldMatrix);
	m_D3D->GetWorldMatrix(Cube1worldMatrix);
	m_D3D->GetWorldMatrix(Cube2worldMatrix);
	m_D3D->GetWorldMatrix(Cube3worldMatrix);
	m_D3D->GetWorldMatrix(MirrorworldMatrix);

	m_D3D->GetWorldMatrix(TranslationMatrix);
	m_D3D->GetWorldMatrix(TranslationMatrix2);
	m_Camera->GetViewMatrix(RotationMatrix);
	m_D3D->GetWorldMatrix(TextworldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	//기본 행렬 변환
	//-------------------------------------------------------------------------------------
	// 스케일링->회전->이동 순으로 합쳐야 한다.
	D3DXMatrixTranslation(&SkyworldMatrix, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	//큐브3 텍스처이동하는애
	D3DXMatrixTranslation(&Cube3worldMatrix, CharacterPos.x, CharacterPos.y, CharacterPos.z);
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix, yaw, pitch, roll);
	D3DXMatrixMultiply(&Cube3worldMatrix, &RotationMatrix, &Cube3worldMatrix);
	//-------------------------------------------------------------------------------------

	//시야 절두체 생성
	//-------------------------------------------------------------------------------------
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	//-------------------------------------------------------------------------------------

	// Sky
	//-------------------------------------------------------------------------------------
	m_D3D->TurnOffCulling();
	m_D3D->TurnZBufferOff();

	m_Sky->RenderSky(m_D3D->GetDeviceContext());
	m_Shader->RenderSkydomeShader(m_D3D->GetDeviceContext(), m_Sky->GetIndexCount(), SkyworldMatrix, viewMatrix, projectionMatrix,
		m_Sky->GetApexColor(), m_Sky->GetCenterColor());

	// Turn back face culling back on.
	m_D3D->TurnOnCulling();

	//구름
	m_D3D->EnableSecondBlendState();

	m_Sky->RenderCloud(m_D3D->GetDeviceContext());

	m_Shader->RenderCloudShader(m_D3D->GetDeviceContext(), m_Sky->GetCloudIndexCount(), SkyworldMatrix, viewMatrix, projectionMatrix,
		m_Sky->GetCloudTexture1(), m_Sky->GetCloudTexture2(), m_Sky->GetTranslation(0), m_Sky->GetTranslation(1),
		m_Sky->GetTranslation(2), m_Sky->GetTranslation(3), m_Sky->GetBrightness());

	
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on.
	m_D3D->TurnZBufferOn();
	//-------------------------------------------------------------------------------------


	// Terrain
	//-------------------------------------------------------------------------------------
	
	// Terrain에서 정한 셀 갯수만큼 셀을 로드한다.
	for (int i = 0; i < m_Terrain->GetCellCount(); i++)
	{
		// Put the terrain cell buffers on the pipeline.
		result = m_Terrain->RenderCell(m_D3D->GetDeviceContext(), i, m_Frustum);
		if (result)
		{

			result = m_TerrainShader->Render(m_D3D->GetDeviceContext(), m_Terrain->GetCellIndexCount(i), TerrainworldMatrix, viewMatrix,
				projectionMatrix, m_Terrain->GetColorTexture(), m_Terrain->GetNormalMapTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
			if (!result) { return false; }

			//m_Terrain->RenderCellLines(m_D3D->GetDeviceContext(), i);
			//m_Shader->RenderColorShader(m_D3D->GetDeviceContext(), m_Terrain->GetCellLinesIndexCount(i), TerrainworldMatrix, viewMatrix, projectionMatrix);
			//if (!result) { return false; }
		}
	}

	//큐브3 텍스처 이동+spec맵
	m_Model_Cube3->Render(m_D3D->GetDeviceContext());

	result = m_Shader->RenderTranslateShader(m_D3D->GetDeviceContext(), m_Model_Cube3->GetIndexCount(), Cube3worldMatrix, viewMatrix, projectionMatrix
		, m_Model_Cube3->GetTripleTextureArray(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), textureTranslation);
	if (!result) { return false; }

	//-------------------------------------------------------------------------------------

	/*
	//기본 행렬 변환
	//-------------------------------------------------------------------------------------
	// 스케일링->회전->이동 순으로 합쳐야 한다.

	//큐브1
	D3DXMatrixRotationY(&Cube1worldMatrix, 0.6f);
	D3DXMatrixTranslation(&TranslationMatrix, -3, 4.5, -8);
	D3DXMatrixMultiply(&Cube1worldMatrix, &Cube1worldMatrix, &TranslationMatrix);

	//큐브2 움직이는애
	D3DXMatrixRotationY(&RotationMatrix, 0.6f);
	//D3DXMatrixTranslation(&TranslationMatrix2, Speed2*0.01f, 4.5, Speed*0.01f - 6);
	D3DXMatrixMultiply(&Cube2worldMatrix, &RotationMatrix, &TranslationMatrix2);



	//plane2
	m_D3D->GetWorldMatrix(TranslationMatrix);
	D3DXMatrixRotationX(&Plane2worldMatrix, D3DXToRadian(-90));
	D3DXMatrixTranslation(&TranslationMatrix, 0, 4, 0);
	D3DXMatrixMultiply(&Plane2worldMatrix, &Plane2worldMatrix, &TranslationMatrix);

	//거울
	D3DXMatrixTranslation(&MirrorworldMatrix, -2.7, 3.8, -8.9);
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale, 0.2f, 1.0f, 0.2f);
	D3DXMatrixMultiply(&MirrorworldMatrix, &Scale, &MirrorworldMatrix);


	//구 리스트 변수들 초기화
	//-------------------------------------------------------------------------------------
	CircleCount = m_Model_CircleList->GetModelCount();
	//-------------------------------------------------------------------------------------

	//기본 행렬 변환, 3D모델, 셰이더 렌더링, 프러스텀 컬링
	//-------------------------------------------------------------------------------------



	//프러스텀 컬링 들어간 구 모델 리스트
	for (Circleindex = 0; Circleindex < CircleCount; Circleindex++)
	{
		//리스트에 있는것들 차례대로 데이터를 읽어온다.
		m_Model_CircleList->GetData(Circleindex, positionX, positionY, positionZ, color);

		radius = 1.0f;

		//Frustum Culling(프러스텀 컬링)
		//3D모델에 경계볼륨을 씌워서 절두체에 보이는지 체크한다.
		Circlerender = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		if (Circlerender)
		{
			//리스트에서 정의된대로 위치 변환
			D3DXMatrixTranslation(&CircleworldMatrix, positionX, positionY, positionZ);

			//3D모델(구체) 렌더링
			m_Model_Circle->Render(m_D3D->GetDeviceContext());

			//셰이더 렌더링
			result = m_Shader->RenderLightShader(m_D3D->GetDeviceContext(), m_Model_Circle->GetIndexCount(), CircleworldMatrix, viewMatrix, projectionMatrix
				, m_Model_Circle->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), color
				, m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
			if (!result) { return false; }

			//구가 각자 다른 위치를 가져야 하니까 reset시킨다!
			m_D3D->GetWorldMatrix(CircleworldMatrix);
		}

	}

	//평면 알파맵
	m_Model_Plane->Render(m_D3D->GetDeviceContext());

	result = m_Shader->RenderAlphaMapShader(m_D3D->GetDeviceContext(), m_Model_Plane->GetIndexCount(), PlaneworldMatrix, viewMatrix, projectionMatrix
		, m_Model_Plane->GetTripleTextureArray());
	if (!result) { return false; }



	//큐브1 노말맵(범프맵)
	m_Model_Cube->Render(m_D3D->GetDeviceContext());

	result = m_Shader->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Model_Cube->GetIndexCount(), Cube1worldMatrix, viewMatrix, projectionMatrix
		, m_Model_Cube->GetMultiTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result) { return false; }
	//result = m_Shader->RenderTextureShader(m_D3D->GetDeviceContext(), m_Model_Cube->GetIndexCount(), Cube1worldMatrix, viewMatrix, projectionMatrix
	//	, m_Model_Cube->GetTexture());

	//큐브2 안개 
	m_Model_Cube2->Render(m_D3D->GetDeviceContext());

	result = m_Shader->RenderFogShader(m_D3D->GetDeviceContext(), m_Model_Cube2->GetIndexCount(), Cube2worldMatrix, viewMatrix, projectionMatrix,
		m_Model_Cube2->GetTexture(), fogStart, fogEnd);
	if (!result) { return false; }



	//평면2 투명안개
	m_D3D->TurnOnAlphaBlending();

	m_Model_Plane2->Render(m_D3D->GetDeviceContext());

	result = m_Shader->RenderTransparentShader(m_D3D->GetDeviceContext(), m_Model_Plane2->GetIndexCount(), Plane2worldMatrix, viewMatrix, projectionMatrix
		, m_Model_Plane2->GetTexture(), 0.7f);
	if (!result) { return false; }

	m_D3D->TurnOffAlphaBlending();

	//거울
	m_Model_Mirror->Render(m_D3D->GetDeviceContext());

	result = m_Shader->RenderReflectionShader(m_D3D->GetDeviceContext(), m_Model_Mirror->GetIndexCount(), MirrorworldMatrix, viewMatrix, projectionMatrix,
		m_Model_Mirror->GetTexture(), m_RTT->GetShaderResourceView(), reflectionMatrix);
	if (!result) { return false; }



	*/
	//-------------------------------------------------------------------------------------
	//2D이미지, 2d셰이더 렌더링, Text, Z버퍼 ON/OFF, 알파블렌딩, RTT미니맵
	//-------------------------------------------------------------------------------------
	m_D3D->TurnZBufferOff();

//result = m_RTTTexture->Render(m_D3D->GetDeviceContext(), 1200, 0);
//if (!result) { return false; }
//
//result = m_Shader->RenderTextureShader(m_D3D->GetDeviceContext(), m_RTTTexture->GetIndexCount(), TextworldMatrix, baseViewMatrix, orthoMatrix, m_RTT->GetShaderResourceView());
//if (!result) { return false; }

	m_D3D->TurnOnAlphaBlending();

	result = m_Title->Render(m_D3D->GetDeviceContext(), TextworldMatrix, orthoMatrix);
	if (!result) { return false; }

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();
	//버퍼에 그려진 씬을 화면에 표시한다.
	m_D3D->EndScene();
	



	return true;
}
/*----------------------------------------------------------------------------------------
이름 : RenderToTexture()
용도 : 
- RTT기능을 사용할 수 있게하는 함수
- 평면 반사 기능을 사용하기 위해 반사 뷰 행렬을 생성해야 한다.
------------------------------------------------------------------------------------------*/
bool GraphicsClass::RenderToTexture()
{
	bool result;
	D3DXMATRIX worldMatrix, reflectionViewMatrix, projectionMatrix;
	D3DXMATRIX TranslationMatrix;

	//RTT기능을 초기화시킨다. = 렌더링 타겟을 rtt로 설정한다.
	m_RTT->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// RTT를 초기화 한다.
	m_RTT->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);

	//// 이 함수에 씬을 그리면 백버퍼 대신 RTT에 렌더링 된다.
	//result = RenderRTTScene();
	//if (!result)
	//{
	//	return false;
	//}

	//카메라가 반사 행렬을 계산하도록 한다. 
	m_Camera->RenderReflection(3.5f, 4.5f);


	//행렬들
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	D3DXMatrixRotationY(&worldMatrix, 0.6f);
	D3DXMatrixTranslation(&TranslationMatrix, -3 ,4.5, -8);
	D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &TranslationMatrix);

	//큐브1
	//-------------------------------------------------------------------------------------------------------------------
	m_Model_Cube->Render(m_D3D->GetDeviceContext());
	result = m_Shader->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Model_Cube->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix
		, m_Model_Cube->GetMultiTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result) { return false; }
	//result = m_Shader->RenderTextureShader(m_D3D->GetDeviceContext(), m_Model_Cube->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix
	//	, m_Model_Cube->GetTexture());

	if (!result) { return false; }

	//-------------------------------------------------------------------------------------------------------------------


	// 렌더링 타겟을 RTT에서 다시 백버퍼로 돌린다.
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

/*----------------------------------------------------------------------------------------
이름 : RenderScene()
용도 : RTT를 사용하기 위해 RTT에 그려질 씬 
------------------------------------------------------------------------------------------*/
bool GraphicsClass::RenderRTTScene()
{
	D3DXMATRIX worldMatrix, projectionMatrix, viewMatrix;

	bool result;

	m_minimapCamera->Render();
	m_minimapCamera->GetViewMatrix(minimapMatrix);


	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);


	D3DXMatrixRotationY(&worldMatrix, 0.8f);


	m_Model_Cube->Render(m_D3D->GetDeviceContext());

	result = m_Shader->RenderBumpMapShader(m_D3D->GetDeviceContext(), m_Model_Cube->GetIndexCount(), worldMatrix, minimapMatrix, projectionMatrix
		, m_Model_Cube->GetMultiTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor());
	if (!result) { return false; }


	return true;
}

/*----------------------------------------------------------------------------------------
이름 : RenderLoadingScene()
용도 : 로딩화면을 그린다.
------------------------------------------------------------------------------------------*/
bool GraphicsClass::RenderLoadingScene()
{
	bool result;
	D3DXMATRIX MainSceneworldMatrix, orthoMatrix;
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->GetWorldMatrix(MainSceneworldMatrix);

	m_D3D->BeginScene(1.0f, 1.0f, 1.0f, 1.0f);

	result = m_Loading->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result) { return false; }

	result = m_Shader->RenderTextureShader(m_D3D->GetDeviceContext(), m_Loading->GetIndexCount(), MainSceneworldMatrix, baseViewMatrix, orthoMatrix, m_Loading->GetTexture());
	if (!result) { return false; }


	m_D3D->EndScene();

	return true;
}
/*----------------------------------------------------------------------------------------
이름 : RenderMainScene()
용도 : 메인화면을 그린다.
------------------------------------------------------------------------------------------*/
bool GraphicsClass::RenderMainScene()
{
	bool result;
	D3DXMATRIX MainSceneworldMatrix, orthoMatrix;
	m_D3D->GetOrthoMatrix(orthoMatrix);
	m_D3D->GetWorldMatrix(MainSceneworldMatrix);

	m_D3D->BeginScene(1.0f,1.0f,1.0f,1.0f);

		result = m_2D_Love->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result) { return false; }

		result = m_Shader->RenderTextureShader(m_D3D->GetDeviceContext(), m_2D_Love->GetIndexCount(), MainSceneworldMatrix, baseViewMatrix, orthoMatrix, m_2D_Love->GetTexture());
		if (!result) { return false; }
	

	m_D3D->EndScene();

	return true;
}