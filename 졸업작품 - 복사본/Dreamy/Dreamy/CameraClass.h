////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>

/*
	우리가 어떻게 장면을 보는지에 대한 정보를 DirectX11에게 전달하는 카메라 클래스.
	카메라 클래스는 카메라의 위치, 현재 회전 상태를 계속 가져야 한다.
	이 정보를 이용하여 렌더링시에 HLSL셰이더에서 사용할 뷰 행렬을 생성한다.

	18.02.09 반사 예제 진행 중
	평면 반사를 다루기 위해 추가 사항 있음.

*/

////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(D3DXVECTOR3);
	void SetRotation(D3DXVECTOR3);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	//카메라의 위치와 회전 상태에 기반한 뷰 행렬을 생성한다.
	void Render();

	//셰이더에서 렌더링에 사용할 수 있도록 카메라 객체의 뷰 행렬을 받아온다.
	void GetViewMatrix(D3DXMATRIX&);

	//평면 반사를 위한 두 함수.
	void RenderReflection(float, float);
	D3DXMATRIX GetReflectionViewMatrix();

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;

	D3DXVECTOR3 m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_reflectionViewMatrix;
};

#endif