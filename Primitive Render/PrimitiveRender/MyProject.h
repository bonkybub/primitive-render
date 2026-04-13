#ifndef _MyProject_h
#define _MyProject_h

#include "DirectX.h"
#include "Font.h"
#include "TextureType.h"
#include "Primitive.h"
#include "IndexedPrimitive.h"
#include "HexagonalCylinder.h"
#include "HexagonalCone.h"
#include "RhombusPair.h"

// forward declare the sprite batch

using namespace DirectX;
using namespace DirectX::SimpleMath;


//----------------------------------------------------------------------------------------------
// Main project class
//	Inherits the directx class to help us initalize directX
//----------------------------------------------------------------------------------------------

class MyProject : public DirectXClass
{
public:
	// constructor
	MyProject(HINSTANCE hInstance);
	~MyProject();

	// Initialize any directX objects
	void InitializeObjects();

	// window message handler
	LRESULT ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam);

	// Called by the render loop to render a single frame
	void Render(void);

	// Called by directX framework to allow you to update any scene objects
	void Update(float deltaTime);

	// Compute the view and projection matrices
	void ComputeViewProjection();

private:

	Matrix viewMatrix;
	Matrix projectionMatrix;

	// mouse variables
	Vector2 mousePos;
	Vector2 mouseDelta;
	bool buttonDown;

	// for camera controls
	Vector3 cameraPos;
	Vector2 cameraRotationSpeed;
	Vector2 cameraRotation;
	float cameraRadius;
	float cameraRadiusSpeed;

	void UpdateCamera(float deltaTime);

	// call when the mouse is released
	void OnMouseDown();
	void OnMouseMove();

	HexagonalCylinder cylinder;
	Matrix cylinderWorldMatrix;

	HexagonalCone cone;
	Matrix coneWorldMatrix;

	RhombusPair rhombuses;
	Matrix rhombusWorldMatrix;

	// bow parts
	HexagonalCylinder grip;
	HexagonalCone upperGripBase1;
	HexagonalCone upperGripBase2;
	HexagonalCone lowerGripBase1;
	HexagonalCone lowerGripBase2;
	HexagonalCylinder upperLimb1;
	HexagonalCylinder upperLimb2;
	HexagonalCone upperLimb3;
	HexagonalCone upperLimbEnd;
	HexagonalCylinder lowerLimb1;
	HexagonalCylinder lowerLimb2;
	HexagonalCone lowerLimb3;
	HexagonalCone lowerLimbEnd;
	HexagonalCylinder upperTie;
	HexagonalCylinder lowerTie;
	HexagonalCylinder string;

	// bow matrices
	Matrix bowParentWorldMatrix;
	Matrix gripWorldMatrix;
	Matrix upperGripBase1WorldMatrix;
	Matrix upperGripBase2WorldMatrix;
	Matrix lowerGripBase1WorldMatrix;
	Matrix lowerGripBase2WorldMatrix;
	Matrix upperLimb1WorldMatrix;
	Matrix upperLimb2WorldMatrix;
	Matrix upperLimb3WorldMatrix;
	Matrix upperLimbEndWorldMatrix;
	Matrix lowerLimb1WorldMatrix;
	Matrix lowerLimb2WorldMatrix;
	Matrix lowerLimb3WorldMatrix;
	Matrix lowerLimbEndWorldMatrix;
	Matrix upperTieWorldMatrix;
	Matrix lowerTieWorldMatrix;
	Matrix stringWorldMatrix;

	// arrow 1 parts
	HexagonalCone arrow1PointTop;
	HexagonalCone arrow1PointBottom;
	HexagonalCylinder arrow1PointTie;
	HexagonalCylinder arrow1Shaft;
	RhombusPair arrow1Fletching1;
	RhombusPair arrow1Fletching2;
	HexagonalCylinder arrow1Nock;

	// arrow 1 matrices
	Matrix arrow1ParentWorldMatrix;
	Matrix arrow1PointTopWorldMatrix;
	Matrix arrow1PointBottomWorldMatrix;
	Matrix arrow1PointTieWorldMatrix;
	Matrix arrow1ShaftWorldMatrix;
	Matrix arrow1Fletching1WorldMatrix;
	Matrix arrow1Fletching2WorldMatrix;
	Matrix arrow1NockWorldMatrix;

	// joint matrix for bow and arrow 1
	Matrix bowArrowJointWorldMatrix;

	// arrow 2 parts
	HexagonalCone arrow2PointTop;
	HexagonalCone arrow2PointBottom;
	HexagonalCylinder arrow2PointTie;
	HexagonalCylinder arrow2Shaft;
	RhombusPair arrow2Fletching1;
	RhombusPair arrow2Fletching2;
	HexagonalCylinder arrow2Nock;

	// arrow 2 matrices
	Matrix arrow2ParentWorldMatrix;
	Matrix arrow2PointTopWorldMatrix;
	Matrix arrow2PointBottomWorldMatrix;
	Matrix arrow2PointTieWorldMatrix;
	Matrix arrow2ShaftWorldMatrix;
	Matrix arrow2Fletching1WorldMatrix;
	Matrix arrow2Fletching2WorldMatrix;
	Matrix arrow2NockWorldMatrix;

	// arrow 3 parts
	HexagonalCone arrow3PointTop;
	HexagonalCone arrow3PointBottom;
	HexagonalCylinder arrow3PointTie;
	HexagonalCylinder arrow3Shaft;
	RhombusPair arrow3Fletching1;
	RhombusPair arrow3Fletching2;
	HexagonalCylinder arrow3Nock;

	// arrow 3 matrices
	Matrix arrow3ParentWorldMatrix;
	Matrix arrow3PointTopWorldMatrix;
	Matrix arrow3PointBottomWorldMatrix;
	Matrix arrow3PointTieWorldMatrix;
	Matrix arrow3ShaftWorldMatrix;
	Matrix arrow3Fletching1WorldMatrix;
	Matrix arrow3Fletching2WorldMatrix;
	Matrix arrow3NockWorldMatrix;

	float modelRotation;
};

#endif