#pragma once

#include <DirectXMath.h>
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
private:
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMFLOAT3 position;
	//XMFLOAT3 direction;
	float rotationX;
	float rotationY;
	XMFLOAT4 rotationQuat;
};
