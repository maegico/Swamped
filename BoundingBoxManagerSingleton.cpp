#include "stdafx.h"
#include "BoundingBoxManagerSingleton.h"

using namespace DirectX;

//  BoundingBoxManagerSingleton
BoundingBoxManagerSingleton* BoundingBoxManagerSingleton::m_pInstance = nullptr;
void BoundingBoxManagerSingleton::Init(void)
{
	m_nBoxs = 0;
}
void BoundingBoxManagerSingleton::Release(void)
{
	//Clean the list of Boxs
	for (int n = 0; n < m_nBoxs; n++)
	{
		//Make sure to release the memory of the pointers
		if (m_lBox[n] != nullptr)
		{
			delete m_lBox[n];
			m_lBox[n] = nullptr;
		}
	}
	m_lBox.clear();
	m_lMatrix.clear();
	m_nBoxs = 0;
}
BoundingBoxManagerSingleton* BoundingBoxManagerSingleton::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new BoundingBoxManagerSingleton();
	}
	return m_pInstance;
}
void BoundingBoxManagerSingleton::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
//The big 3
BoundingBoxManagerSingleton::BoundingBoxManagerSingleton() { Init(); }
BoundingBoxManagerSingleton::BoundingBoxManagerSingleton(BoundingBoxManagerSingleton const& other) { }
BoundingBoxManagerSingleton& BoundingBoxManagerSingleton::operator=(BoundingBoxManagerSingleton const& other) { return *this; }
BoundingBoxManagerSingleton::~BoundingBoxManagerSingleton() { Release(); };
//Accessors
int BoundingBoxManagerSingleton::GetBoxTotal(void) { return m_nBoxs; }

//--- Non Standard Singleton Methods
void BoundingBoxManagerSingleton::GenerateBoundingBox(XMFLOAT4X4 a_mModelToWorld, std::string a_sInstanceName)
{
	//HOW DO WE VERIFY THAT THE INSTANCE OF THE MODEL IS LOADED?

		int nBox = IdentifyBox(a_sInstanceName);
		if (nBox == -1)
		{
			//Create a new bounding Box
			BoundingBoxClass* pBB = new BoundingBoxClass();

			BoundingBoxClass* aBB = new BoundingBoxClass();

			//construct its information out of the instance name
			pBB->GenerateOrientedBoundingBox(a_sInstanceName);

			//aBB->GenerateAxisAlignedBoundingBox(a_mModelToWorld); ?

			//Push the Box back into the list
			m_lBox.push_back(pBB);
			//m_lBox.push_back(aBB);

			//m_lBox.push_back(aBB);

			//Push a new matrix into the list
			XMMATRIX ident = XMMatrixIdentity();
			XMStoreFloat4x4(&world, ident);
			m_lMatrix.push_back(world); 

			//Increase the number of Boxes
			m_nBoxs++;
		}
		else //If the box has already been created you will need to check its global orientation
		{
			m_lBox[nBox]->GenerateAxisAlignedBoundingBox(a_mModelToWorld);
			//aBB->doSomething
		}
		nBox = IdentifyBox(a_sInstanceName);
		m_lMatrix[nBox] = a_mModelToWorld;
	
}

void BoundingBoxManagerSingleton::SetBoundingBoxSpace(XMFLOAT4X4 a_mModelToWorld, std::string a_sInstanceName)
{
	int nBox = IdentifyBox(a_sInstanceName);
	//If the Box was found
	if (nBox != -1)
	{
		//Set up the new matrix in the appropriate index
		m_lMatrix[nBox] = a_mModelToWorld;
	}
}

int BoundingBoxManagerSingleton::IdentifyBox(std::string a_sInstanceName)
{
	//Go one by one for all the Boxs in the list
	for (int nBox = 0; nBox < m_nBoxs; nBox++)
	{
		//If the current Box is the one we are looking for we return the index
		if (a_sInstanceName == m_lBox[nBox]->GetName())
			return nBox;
	}
	return -1;//couldn't find it return with no index
}

void BoundingBoxManagerSingleton::CalculateCollision(void)
{
	//Create a placeholder for all center points
	std::vector<XMFLOAT3> lCentroid;
	//for all Boxs...
	for (int nBox = 0; nBox < m_nBoxs; nBox++)
	{
		//Place all the centroids of Boxs in global space
		lCentroid.push_back(static_cast<XMFLOAT3>(m_lMatrix[nBox] * vector4(m_lBox[nBox]->GetCentroidAABB(), 1.0f))); //what to do with this line?
	}

	//Now the actual check
	for (int i = 0; i < m_nBoxs - 1; i++)
	{
		for (int j = i + 1; j < m_nBoxs; j++)
		{
			//If the distance between the center of both Boxes is less than the sum of their radius there is a collision
			//For this check we will assume they will be colliding unless they are not in the same space in X, Y or Z
			//so we place them in global positions
			
			//Make them XMFLOAT3's because there is no math being performed on them 
			XMFLOAT3 v1Min = (XMFLOAT3(m_lBox[i]->GetMinimumAABB())); //Key for HW assignment
			XMFLOAT3 v1Max = (XMFLOAT3(m_lBox[i]->GetMaximumAABB()));

			//?
			//vector3 v1Minno = static_cast<vector3>(m_lMatrix[i] * vector4(m_lBox[i]->GetMinimumOBB(),1));
			//vector3 v1Maxxo = static_cast<vector3>(m_lMatrix[i] * vector4(m_lBox[i]->GetMaximumOBB(),1));

			XMFLOAT3 v2Min = (XMFLOAT3(m_lBox[j]->GetMinimumAABB()));
			XMFLOAT3 v2Max = (XMFLOAT3(m_lBox[j]->GetMaximumAABB()));

			//more
			//vector3 v2Minno = static_cast<vector3>(m_lMatrix[j] * vector4(m_lBox[j]->GetMinimumOBB(),1));
			//vector3 v2Maxxo = static_cast<vector3>(m_lMatrix[j] * vector4(m_lBox[j]->GetMaximumOBB(),1));

			bool bColliding = true;
			if (v1Max.x /*+ (m_lBox[i]->m_v3SizeAABB.x/2)*/ < v2Min.x || v1Min.x > v2Max.x)
				bColliding = false;
			else if (v1Max.y < v2Min.y || v1Min.y > v2Max.y)
				bColliding = false;
			else if (v1Max.z < v2Min.z || v1Min.z > v2Max.z)
				bColliding = false;
			//
			//these may be added to the above?
			/*if(v1Maxxo.x < v2Minno.x || v1Minno.x > v2Maxxo.x)
			bColliding = false;
			else if(v1Maxxo.y < v2Minno.y || v1Minno.y > v2Maxxo.y)
			bColliding = false;
			else if(v1Maxxo.z < v2Minno.z || v1Minno.z > v2Maxxo.z)
			bColliding = false;*/

			if (bColliding)
			{
				//some kind of action happens from  when they collide
			}
		}
	}
}
