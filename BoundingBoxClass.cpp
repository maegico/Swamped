#include "stdafx.h"
#include "BoundingBoxClass.h"

#include <vector>

using namespace DirectX;

//  BoundingBoxClass
void BoundingBoxClass::Init(void)
{
	m_bInitialized = false;
	m_v3Min = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_v3MinAABB = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_v3Max = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_v3MaxAABB = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_v3Centroid = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_v3CentroidAABB = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_sName = "NULL";
}
void BoundingBoxClass::Swap(BoundingBoxClass& other)
{
	std::swap(m_bInitialized, other.m_bInitialized);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_v3Centroid, other.m_v3Centroid);
	std::swap(m_sName, other.m_sName);
}
void BoundingBoxClass::Release(void)
{
	//No pointers to release
}
//The big 3
BoundingBoxClass::BoundingBoxClass() { Init(); }
BoundingBoxClass::BoundingBoxClass(BoundingBoxClass const& other)
{
	m_bInitialized = other.m_bInitialized;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_v3Centroid = other.m_v3Centroid;
	m_sName = other.m_sName;
	//
	m_v3MinAABB = other.m_v3MinAABB;
	m_v3MaxAABB = other.m_v3MaxAABB;
	m_v3CentroidAABB = other.m_v3CentroidAABB;
}
BoundingBoxClass& BoundingBoxClass::operator=(BoundingBoxClass const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		BoundingBoxClass temp(other);
		Swap(temp);
	}
	return *this;
}
BoundingBoxClass::~BoundingBoxClass() { Release(); };
//Accessors
bool BoundingBoxClass::IsInitialized(void) { return m_bInitialized; }
XMFLOAT3 BoundingBoxClass::GetMinimumOBB(void) { return m_v3Min; }
XMFLOAT3 BoundingBoxClass::GetMaximumOBB(void) { return m_v3Max; }
XMFLOAT3 BoundingBoxClass::GetCentroid(void) { return m_v3Centroid; }
std::string BoundingBoxClass::GetName(void) { return m_sName; }
//
XMFLOAT3 BoundingBoxClass::GetMinimumAABB(void) { return m_v3MinAABB; }
XMFLOAT3 BoundingBoxClass::GetMaximumAABB(void) { return m_v3MaxAABB; }
XMFLOAT3 BoundingBoxClass::GetCentroidAABB(void) { return m_v3CentroidAABB; }

//Methods
void BoundingBoxClass::GenerateOrientedBoundingBox(std::string a_sInstanceName)
{
	//If this has already been initialized there is nothing to do here
	if (m_bInitialized)
		return;

	{
		m_sName = a_sInstanceName;
		XMFLOAT3 * makeBox;

		std::vector<XMVECTOR> lVertices = pMeshMngr->GetVertices(m_sName); //THIS LINE IS IMPORTANT TO GETTING MESH's VERTICES
		unsigned int nVertices = lVertices.size();
		
		/*m_v3Centroid = lVertices[0];
		m_v3Max = lVertices[0];
		m_v3Min = lVertices[0];*/
		XMStoreFloat3(&m_v3Centroid, lVertices[0]);
		XMStoreFloat3(&m_v3Max, lVertices[0]);
		XMStoreFloat3(&m_v3Min, lVertices[0]);

		for (unsigned int nVertex = 1; nVertex < nVertices; nVertex++)
		{
			//m_v3Centroid += lVertices[nVertex];
			XMStoreFloat3(&makeBox[nVertex], lVertices[nVertex]);

			if (m_v3Min.x > makeBox[nVertex].x)
				m_v3Min.x = makeBox[nVertex].x;
			else if (m_v3Max.x < makeBox[nVertex].x)
				m_v3Max.x = makeBox[nVertex].x;

			if (m_v3Min.y > makeBox[nVertex].y)
				m_v3Min.y = makeBox[nVertex].y;
			else if (m_v3Max.y < makeBox[nVertex].y)
				m_v3Max.y = makeBox[nVertex].y;

			if (m_v3Min.z > makeBox[nVertex].z)
				m_v3Min.z = makeBox[nVertex].z;
			else if (m_v3Max.z < makeBox[nVertex].z)
				m_v3Max.z = makeBox[nVertex].z;
		}
		//calculate centroid as XMVECTOR
		//store vector in XMFLOAT3 m_v3Centroid

		//ONLY down here is where all the math happens
		//m_v3Centroid = (m_v3Min + m_v3Max) / 2.0f;

		XMVECTOR max(XMLoadFloat3(&m_v3Max));
		XMVECTOR min(XMLoadFloat3(&m_v3Min));
		XMVECTOR centroid = (max + min) / 2.0f;

		XMStoreFloat3(&m_v3Centroid, centroid);

		//What you could do(based on this link: http://stackoverflow.com/questions/10291862/what-is-the-best-way-to-get-distance-between-2-points-with-directxmath):
		//1) Make these as XMFLOATs to load
		XMFLOAT3 minX = XMFLOAT3(m_v3Min.x, 0.0f, 0.0f);
		XMVECTOR minSizeX = XMLoadFloat3(&minX);
		XMFLOAT3 maxX = XMFLOAT3(m_v3Max.x, 0.0f, 0.0f);
		XMVECTOR maxSizeX = XMLoadFloat3(&maxX);
		//2) Do calculations with these XMVECTORs (find distance formula)
		XMVECTOR diffX = minSizeX - maxSizeX;
		XMVECTOR distX = XMVectorSqrt(XMVector3Dot(diffX, diffX));
		//3) Store those distance calculations in some new XMFLOATS (XMFLOAT3 calcX)
		float calcX = 0.0f;
		XMStoreFloat(&calcX, distX);

		//1) Make these as XMFLOATs to load
		XMFLOAT3 minY = XMFLOAT3(0.0f, m_v3Min.y, 0.0f);
		XMVECTOR minSizeY = XMLoadFloat3(&minY);
		XMFLOAT3 maxY = XMFLOAT3(0.0f, m_v3Max.y, 0.0f);
		XMVECTOR maxSizeY = XMLoadFloat3(&maxY);
		//2) Do calculations with these XMVECTORs (find distance formula)
		XMVECTOR diffY = minSizeY - maxSizeY;
		XMVECTOR distY = XMVectorSqrt(XMVector3Dot(diffY, diffY));
		//3) Store those distance calculations in some new XMFLOATS (XMFLOAT3 calcX)
		float calcY = 0.0f;
		XMStoreFloat(&calcY, distY);

		//1) Make these as XMFLOATs to load
		XMFLOAT3 minZ = XMFLOAT3(0.0f, 0.0f, m_v3Min.z);
		XMVECTOR minSizeZ = XMLoadFloat3(&minZ);
		XMFLOAT3 maxZ = XMFLOAT3(0.0f, 0.0f, m_v3Max.z);
		XMVECTOR maxSizeZ = XMLoadFloat3(&maxZ);
		//2) Do calculations with these XMVECTORs (find distance formula)
		XMVECTOR diffZ = minSizeZ - maxSizeZ;
		XMVECTOR distZ = XMVectorSqrt(XMVector3Dot(diffZ, diffZ));
		//3) Store those distance calculations in some new XMFLOATS (XMFLOAT3 calcX)
		float calcZ = 0.0f;
		XMStoreFloat(&calcZ, distZ);

		//Last math thing: store in size vector
		m_v3Size.x = calcX;
		m_v3Size.y = calcY;
		m_v3Size.z = calcZ;

		//last step lads
		m_bInitialized = true;

	}
}

void BoundingBoxClass::GenerateAxisAlignedBoundingBox(XMFLOAT4X4 a_m4ModeltoWorld)
{
	//Generate the Axis Aligned Bounding Box here based on the Oriented Bounding Box
	//WHAT TO PUT IN PLACE OF pMeshMngr?
	//MeshManagerSingleton* pMeshMngr = MeshManagerSingleton::GetInstance();
	std::vector<XMVECTOR> lVertices = pMeshMngr->GetVertices(m_sName);
	unsigned int nVertices = lVertices.size();

	/*m_v3CentroidAABB = lVertices[0];
	m_v3MinAABB = static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[0], 1));
	m_v3MaxAABB = static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[0], 1));*/
	XMFLOAT3 lVerts;
	XMStoreFloat3(&lVerts, lVertices[0]);
	
	XMFLOAT4 temp(lVerts.x, lVerts.y, lVerts.z, 1.0f);
	XMVECTOR minCast = static_cast<XMVECTOR>(XMVector4Transform(XMLoadFloat4(&temp), XMLoadFloat4x4(&a_m4ModeltoWorld)));
	XMVECTOR maxCast = static_cast<XMVECTOR>(XMVector4Transform(XMLoadFloat4(&temp), XMLoadFloat4x4(&a_m4ModeltoWorld)));
	
	XMStoreFloat3(&m_v3CentroidAABB, lVertices[0]);
	XMStoreFloat3(&m_v3MinAABB, minCast);
	XMStoreFloat3(&m_v3MaxAABB, maxCast);


	for (unsigned int nVertex = 1; nVertex < nVertices; nVertex++) //FIGURE OUT HOW TO DO THIS FOR-LOOP
	{
		//s

		//we need this first to compare x, y, and z
		//XMStoreFloat3(&makeBoxAABB[nVertex], lVertices[nVertex]);

		//Make the three things before doing comparisons
		//XMVECTOR compare(lVerts.x, lVerts.y, lVerts.z, 1.0f);
		XMStoreFloat4(&makeBoxAABB[nVertex], lVertices[nVertex]);


		
		if (m_v3MinAABB.x > static_cast<XMVECTOR>(XMVector4Transform(&makeBoxAABB, XMLoadFloat4x4(&a_m4ModeltoWorld)).x)
			m_v3MinAABB.x = static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).x;
		else if (m_v3MaxAABB.x < static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).x)
			m_v3MaxAABB.x = static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).x;

		if (m_v3MinAABB.y > static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).y)
			m_v3MinAABB.y = static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).y;
		else if (m_v3MaxAABB.y < static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).y)
			m_v3MaxAABB.y = static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).y;

		if (m_v3MinAABB.z > static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).z)
			m_v3MinAABB.z = static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).z;
		else if (m_v3MaxAABB.z < static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).z)
			m_v3MaxAABB.z = static_cast<vector3>(a_m4ModeltoWorld * vector4(lVertices[nVertex], 1)).z;
	}
	//calculate centroid as XMVECTOR
	//store vector in XMFLOAT3 m_v3Centroid

	//ONLY down here is where all the math happens
	//m_v3Centroid = (m_v3Min + m_v3Max) / 2.0f;

	XMVECTOR max(XMLoadFloat3(&m_v3MaxAABB));
	XMVECTOR min(XMLoadFloat3(&m_v3MinAABB));
	XMVECTOR centroid = (max + min) / 2.0f;

	XMStoreFloat3(&m_v3CentroidAABB, centroid);

	//What you could do(based on this link: http://stackoverflow.com/questions/10291862/what-is-the-best-way-to-get-distance-between-2-points-with-directxmath):
	//1) Make these as XMFLOATs to load
	XMFLOAT3 minX = XMFLOAT3(m_v3MinAABB.x, 0.0f, 0.0f);
	XMVECTOR minSizeX = XMLoadFloat3(&minX);
	XMFLOAT3 maxX = XMFLOAT3(m_v3MaxAABB.x, 0.0f, 0.0f);
	XMVECTOR maxSizeX = XMLoadFloat3(&maxX);
	//2) Do calculations with these XMVECTORs (find distance formula)
	XMVECTOR diffX = minSizeX - maxSizeX;
	XMVECTOR distX = XMVectorSqrt(XMVector3Dot(diffX, diffX));
	//3) Store those distance calculations in some new XMFLOATS (XMFLOAT3 calcX)
	float calcX = 0.0f;
	XMStoreFloat(&calcX, distX);

	//1) Make these as XMFLOATs to load
	XMFLOAT3 minY = XMFLOAT3(0.0f, m_v3MinAABB.y, 0.0f);
	XMVECTOR minSizeY = XMLoadFloat3(&minY);
	XMFLOAT3 maxY = XMFLOAT3(0.0f, m_v3MaxAABB.y, 0.0f);
	XMVECTOR maxSizeY = XMLoadFloat3(&maxY);
	//2) Do calculations with these XMVECTORs (find distance formula)
	XMVECTOR diffY = minSizeY - maxSizeY;
	XMVECTOR distY = XMVectorSqrt(XMVector3Dot(diffY, diffY));
	//3) Store those distance calculations in some new XMFLOATS (XMFLOAT3 calcX)
	float calcY = 0.0f;
	XMStoreFloat(&calcY, distY);

	//1) Make these as XMFLOATs to load
	XMFLOAT3 minZ = XMFLOAT3(0.0f, 0.0f, m_v3MinAABB.z);
	XMVECTOR minSizeZ = XMLoadFloat3(&minZ);
	XMFLOAT3 maxZ = XMFLOAT3(0.0f, 0.0f, m_v3MaxAABB.z);
	XMVECTOR maxSizeZ = XMLoadFloat3(&maxZ);
	//2) Do calculations with these XMVECTORs (find distance formula)
	XMVECTOR diffZ = minSizeZ - maxSizeZ;
	XMVECTOR distZ = XMVectorSqrt(XMVector3Dot(diffZ, diffZ));
	//3) Store those distance calculations in some new XMFLOATS (XMFLOAT3 calcX)
	float calcZ = 0.0f;
	XMStoreFloat(&calcZ, distZ);

	//Last math thing: store in size vector
	m_v3SizeAABB.x = calcX;
	m_v3SizeAABB.y = calcY;
	m_v3SizeAABB.z = calcZ;

}
