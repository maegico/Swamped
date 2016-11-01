#include "Camera.h"

Camera::Camera(XMFLOAT3 pos) {
	rotationX = 0;
	rotationY = 0;
	XMStoreFloat4(&rotationQuat, XMQuaternionRotationRollPitchYaw(0, rotationX, rotationY));
	position = pos;

	//direction = XMFLOAT3(0, 0, 1);
	Update();
}

Camera::Camera() : Camera(XMFLOAT3(0,0,0)) {

}

Camera::~Camera() {

}

void Camera::CreateProjectionMatrix(float width, float height, float fov) {
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		fov * (3.1415926535f/180.0f),		// Field of View Angle
		width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projection, XMMatrixTranspose(P)); // Transpose for HLSL!
}

XMFLOAT4X4 Camera::GetProjection() {
	return projection;
}

XMFLOAT4X4 Camera::GetView() {
	return view;
}

void Camera::RelativePositionDelta(float x, float y, float z)
{
	XMFLOAT3 delta = XMFLOAT3(x, y, z);
	XMVECTOR delt = XMLoadFloat3(&delta);
	XMVECTOR rotQuat = XMLoadFloat4(&rotationQuat);
	XMStoreFloat3(&delta, XMVector3Rotate(delt, rotQuat));
	position.x += delta.x;
	position.y += delta.y;
	position.z += delta.z;

}

void Camera::RotationDelta(float x, float y) {
	rotationX += x;
	rotationY += y;
}

void Camera::Update() {
	XMFLOAT3 forward = XMFLOAT3(0, 0, 1);
	XMFLOAT3 up = XMFLOAT3(0, 1, 0);
	XMVECTOR rotationQuaternion = XMQuaternionRotationRollPitchYaw(rotationX, rotationY,0);
	XMStoreFloat4(&rotationQuat, rotationQuaternion);
	XMVECTOR newDirection = XMLoadFloat3(&forward);
	newDirection = XMVector3Rotate(newDirection, rotationQuaternion);
	XMMATRIX newView = XMMatrixLookToLH(XMLoadFloat3(&position), newDirection, XMLoadFloat3(&up));
	XMStoreFloat4x4(&view,XMMatrixTranspose(newView));
}