#pragma once

#include <DirectXMath.h> 
#include <string>

class BoundingBoxClass
{
	bool m_bInitialized; //Has the elements of this object been initialized before?
	DirectX::XMFLOAT3 m_v3Centroid;//Center point of the model, the origin in GSpace in which the sphere is going to be located
	DirectX::XMFLOAT3 m_v3CentroidAABB;
	DirectX::XMFLOAT3 m_v3Min;//Collection of minimal components of the model
	DirectX::XMFLOAT3 m_v3Max;//Collection of maximum componets of the model
	DirectX::XMFLOAT3 m_v3MinAABB;
	DirectX::XMFLOAT3 m_v3MaxAABB;
	DirectX::XMFLOAT3 m_v3Size;//Size of the Box
	DirectX::XMFLOAT3 m_v3SizeAABB;
	std::string m_sName;//Name of the BoundingBox (to relate to the instance)

	/*
    Some extra floats
	*/
	//DirectX::XMFLOAT3 * makeBox;
	DirectX::XMFLOAT4 * makeBoxAABB;

public:
	/* Constructor */
	BoundingBoxClass(void);
	/* Copy Constructor */
	BoundingBoxClass(BoundingBoxClass const& other);
	/* Copy Assignment Operator*/
	BoundingBoxClass& operator=(BoundingBoxClass const& other);
	/* Destructor */
	~BoundingBoxClass(void);

	/* Swaps the contents of the object with another object's content */
	void Swap(BoundingBoxClass& other);

	/*
	IsInitialized
	Returns information about whether or not the object has been initialized
	*/
	bool IsInitialized(void);

	/*
	GetMinimum
	Returns the minimum vector
	*/
	DirectX::XMFLOAT3 GetMinimumOBB(void);

	//NEW
	DirectX::XMFLOAT3 GetMinimumAABB(void);

	/*
	GetMaximum
	Returns the maximum vector
	*/
	DirectX::XMFLOAT3 GetMaximumOBB(void);

	//NEW
	DirectX::XMFLOAT3 GetMaximumAABB(void);

	/*
	GetCentroid
	Returns the origin of the Cartesian Coordinate System in which the sphere is drawn in GSpace
	*/
	DirectX::XMFLOAT3 GetCentroid(void);

	//NEW
	DirectX::XMFLOAT3 GetCentroidAABB(void);

	/*
	GetName
	Returns the name of the sphere to associate it with an instance
	*/
	std::string GetName(void);

	/*
	GenerateOrientedBoundingBox
	Creates the Axis Aligned Bounding Box in local space of the model specified in the instance name
	*/
	void GenerateOrientedBoundingBox(std::string a_sInstanceName);

	/*
	GenerateAxisAlignedBoundingBox
	Creates the Oriented Bounding Box in global space
	*/
	void GenerateAxisAlignedBoundingBox(DirectX::XMFLOAT4X4 a_m4ModeltoWorld);

private:
	/* Releases the object from memory */
	void Release(void);

	/* Initialize the object's fields */
	void Init(void);

	/*
	TODO:
	- Figure out what that one line is in the Singleton (minor)
	- Figure out what to do with pMeshMngr related code (Mike's work)
	- AABB For-loop!!!!
	*/
};

