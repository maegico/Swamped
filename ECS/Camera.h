#pragma once

#include <DirectXMath.h>
#include "EntityIdTypeDef.h"
#include "DXCore.h"
using namespace DirectX;
class Camera {
public:
	void Update();
	Camera(XMFLOAT3 pos);
	Camera();
	~Camera();
	XMFLOAT4X4 GetProjection();
	XMFLOAT4X4 GetView();
	XMFLOAT3 GetPosition();
	void RelativePositionDelta(float x, float y, float z);
	void RotationDelta(float x, float y);
	void CreateProjectionMatrix(float width, float height, float fov);
	void MouseInput(float x, float y);
	//XMFLOAT3 GetPosition(); 
private:
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT3 position;
	//XMFLOAT3 direction;
	float rotationX;
	float rotationY;
	XMFLOAT4 rotationQuat;
	EntityId cameraId;

	//for moving the camera
	DirectX::XMFLOAT3 m_forward;
	//DirectX::XMFLOAT3 m_up;
	DirectX::XMFLOAT3 m_right;
	DirectX::XMFLOAT3 movement;
};
