#pragma once

#include <DirectXMath.h>
#include <string>
#include <vector>

#include "BoundingBoxClass.h"

class BoundingBoxManagerSingleton
{
	int m_nBoxs; //number of elements in the list
	std::vector<BoundingBoxClass*> m_lBox; //list of Boxs
	std::vector<DirectX::XMFLOAT4X4> m_lMatrix; //list of Global spaces of each Box in the list
	static BoundingBoxManagerSingleton* m_pInstance; // Singleton pointer

	//new addition
	DirectX::XMFLOAT4X4 world;

public:
	/* Gets/Constructs the singleton pointer */
	static BoundingBoxManagerSingleton* GetInstance();
	/* Destroys the singleton */
	static void ReleaseInstance(void);

	/* Asks the singleton for the number of elements in the list */
	int GetBoxTotal(void);

	/*
	GenerateBoundingBox
	Generates a bounding Box from the specified instance name and adds it to the list
	*/
	void GenerateBoundingBox(DirectX::XMFLOAT4X4 a_mModelToWorld, std::string a_sInstanceName);

	/*
	SetBoundingBoxSpace
	Sets the Matrix that represents the global position of the Box specified by instance name
	*/
	void SetBoundingBoxSpace(DirectX::XMFLOAT4X4 a_mModelToWorld, std::string a_sInstanceName);

	/*
	IdentifyBox
	Returns the index in which the specified Box is stored in the list
	*/
	int IdentifyBox(std::string a_sInstanceName);

	/*
	CalculateCollision
	Calculates the collision among all the Boxs in the list
	*/
	void CalculateCollision(void);

private:
	/* Constructor */
	BoundingBoxManagerSingleton(void);
	/* Copy Constructor */
	BoundingBoxManagerSingleton(BoundingBoxManagerSingleton const& other);
	/* Copy Assignment Operator */
	BoundingBoxManagerSingleton& operator=(BoundingBoxManagerSingleton const& other);
	/* Destructor */
	~BoundingBoxManagerSingleton(void);

	/* Releases the objects memory */
	void Release(void);
	/* Initializates the objects fields */
	void Init(void);
};


