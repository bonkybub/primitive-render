#include "MyProject.h"
#include <Windowsx.h> // for GET__LPARAM macros
#include <SimpleMath.h>
#include <DirectXColors.h>
#include <sstream>
#include <CommonStates.h>


using namespace DirectX;
using namespace DirectX::SimpleMath;

// helper function
//
// returns a random float between 0 & 1
float RandFloat() { return float(rand())/float(RAND_MAX); } 

static const float CAMERA_SPEED = XM_PI * 0.2f;

//----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	MyProject application(hInstance);    // Create the class variable

	if( application.InitWindowsApp(L"DirectX 3D", nShowCmd) == false )    // Initialize the window, if all is well show and update it so it displays
	{
		return 0;                   // Error creating the window, terminate application
	}

	if( application.InitializeDirect3D() )
	{
		application.SetDepthStencil(true);      // Tell DirectX class to create and maintain a depth stencil buffer
		application.InitializeObjects();
		application.MessageLoop();				// Window has been successfully created, start the application message loop
	}

	return 0;
}

//----------------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------------
MyProject::MyProject(HINSTANCE hInstance)
	: DirectXClass(hInstance)
{
	mousePos = Vector2(clientWidth * 0.5f, clientHeight * 0.5f);
	buttonDown = false;
	
	ClearColor = Color(DirectX::Colors::Black.v);

	cameraPos = Vector3(0, 0, 6);

	cameraRadius = 8;
	cameraRadiusSpeed = 0;

	cameraRotation = Vector2::Zero;
	cameraRotationSpeed = Vector2::Zero;
}

//----------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------
MyProject::~MyProject()
{
}

//----------------------------------------------------------------------------------------------
// Initialize any fonts we need to use here
//----------------------------------------------------------------------------------------------
void MyProject::InitializeObjects()
{
	modelRotation = 0.0f;
	
	// set colours
	// bow
	grip.SetColor(Color(0.71f, 0.48f, 0.14f));
	upperGripBase1.SetColor(Color(0.15f, 0.19f, 0.64f));
	upperGripBase2.SetColor(Color(0.15f, 0.19f, 0.64f));
	lowerGripBase1.SetColor(Color(0.15f, 0.19f, 0.64f));
	lowerGripBase2.SetColor(Color(0.15f, 0.19f, 0.64f));
	upperLimb1.SetColor(Color(0.37f, 0.18f, 0.04f));
	upperLimb2.SetColor(Color(0.37f, 0.18f, 0.04f));
	upperLimb3.SetColor(Color(0.37f, 0.18f, 0.04f));
	upperLimbEnd.SetColor(Color(0.75f, 0.25f, 0.25f));
	lowerLimb1.SetColor(Color(0.37f, 0.18f, 0.04f));
	lowerLimb2.SetColor(Color(0.37f, 0.18f, 0.04f));
	lowerLimb3.SetColor(Color(0.37f, 0.18f, 0.04f));
	lowerLimbEnd.SetColor(Color(0.75f, 0.25f, 0.25f));
	upperTie.SetColor(Color(0.79f, 0.72f, 0.65f));
	lowerTie.SetColor(Color(0.79f, 0.72f, 0.65f));
	string.SetColor(Color(0.79f, 0.72f, 0.65f));

	// arrow 1
	arrow1PointTop.SetColor(Color(0.8f, 0.8f, 0.8f));
	arrow1PointBottom.SetColor(Color(0.8f, 0.8f, 0.8f));
	arrow1PointTie.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow1Shaft.SetColor(Color(0.6f, 0.36f, 0.25f));
	arrow1Fletching1.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow1Fletching2.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow1Nock.SetColor(Color(0.75f, 0.25f, 0.25f));

	// arrow 2
	arrow2PointTop.SetColor(Color(0.8f, 0.8f, 0.8f));
	arrow2PointBottom.SetColor(Color(0.8f, 0.8f, 0.8f));
	arrow2PointTie.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow2Shaft.SetColor(Color(0.6f, 0.36f, 0.25f));
	arrow2Fletching1.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow2Fletching2.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow2Nock.SetColor(Color(0.75f, 0.25f, 0.25f));

	// arrow 3
	arrow3PointTop.SetColor(Color(0.8f, 0.8f, 0.8f));
	arrow3PointBottom.SetColor(Color(0.8f, 0.8f, 0.8f));
	arrow3PointTie.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow3Shaft.SetColor(Color(0.6f, 0.36f, 0.25f));
	arrow3Fletching1.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow3Fletching2.SetColor(Color(0.79f, 0.72f, 0.65f));
	arrow3Nock.SetColor(Color(0.75f, 0.25f, 0.25f));

	// initialize geometry
	// primitives
	cylinder.InitializeGeometry(D3DDevice);
	cone.InitializeGeometry(D3DDevice);
	rhombuses.InitializeGeometry(D3DDevice);

	// arrow 1
	grip.InitializeGeometry(D3DDevice);
	upperGripBase1.InitializeGeometry(D3DDevice);
	upperGripBase2.InitializeGeometry(D3DDevice);
	lowerGripBase1.InitializeGeometry(D3DDevice);
	lowerGripBase2.InitializeGeometry(D3DDevice);
	upperLimb1.InitializeGeometry(D3DDevice);
	upperLimb2.InitializeGeometry(D3DDevice);
	upperLimb3.InitializeGeometry(D3DDevice);
	upperLimbEnd.InitializeGeometry(D3DDevice);
	lowerLimb1.InitializeGeometry(D3DDevice);
	lowerLimb2.InitializeGeometry(D3DDevice);
	lowerLimb3.InitializeGeometry(D3DDevice);
	lowerLimbEnd.InitializeGeometry(D3DDevice);
	upperTie.InitializeGeometry(D3DDevice);
	lowerTie.InitializeGeometry(D3DDevice);
	string.InitializeGeometry(D3DDevice);

	// arrow 1
	arrow1PointTop.InitializeGeometry(D3DDevice);
	arrow1PointBottom.InitializeGeometry(D3DDevice);
	arrow1PointTie.InitializeGeometry(D3DDevice);
	arrow1Shaft.InitializeGeometry(D3DDevice);
	arrow1Fletching1.InitializeGeometry(D3DDevice);
	arrow1Fletching2.InitializeGeometry(D3DDevice);
	arrow1Nock.InitializeGeometry(D3DDevice);

	// arrow 2
	arrow2PointTop.InitializeGeometry(D3DDevice);
	arrow2PointBottom.InitializeGeometry(D3DDevice);
	arrow2PointTie.InitializeGeometry(D3DDevice);
	arrow2Shaft.InitializeGeometry(D3DDevice);
	arrow2Fletching1.InitializeGeometry(D3DDevice);
	arrow2Fletching2.InitializeGeometry(D3DDevice);
	arrow2Nock.InitializeGeometry(D3DDevice);

	// arrow 3
	arrow3PointTop.InitializeGeometry(D3DDevice);
	arrow3PointBottom.InitializeGeometry(D3DDevice);
	arrow3PointTie.InitializeGeometry(D3DDevice);
	arrow3Shaft.InitializeGeometry(D3DDevice);
	arrow3Fletching1.InitializeGeometry(D3DDevice);
	arrow3Fletching2.InitializeGeometry(D3DDevice);
	arrow3Nock.InitializeGeometry(D3DDevice);

	// initialize shaders
	// primitives
	cylinder.InitializeShaders(D3DDevice);
	cone.InitializeShaders(D3DDevice);
	rhombuses.InitializeShaders(D3DDevice);

	// bow
	grip.InitializeShaders(D3DDevice);
	upperGripBase1.InitializeShaders(D3DDevice);
	upperGripBase2.InitializeShaders(D3DDevice);
	lowerGripBase1.InitializeShaders(D3DDevice);
	lowerGripBase2.InitializeShaders(D3DDevice);
	upperLimb1.InitializeShaders(D3DDevice);
	upperLimb2.InitializeShaders(D3DDevice);
	upperLimb3.InitializeShaders(D3DDevice);
	upperLimbEnd.InitializeShaders(D3DDevice);
	lowerLimb1.InitializeShaders(D3DDevice);
	lowerLimb2.InitializeShaders(D3DDevice);
	lowerLimb3.InitializeShaders(D3DDevice);
	lowerLimbEnd.InitializeShaders(D3DDevice);
	upperTie.InitializeShaders(D3DDevice);
	lowerTie.InitializeShaders(D3DDevice);
	string.InitializeShaders(D3DDevice);

	// arrow 1
	arrow1PointTop.InitializeShaders(D3DDevice);
	arrow1PointBottom.InitializeShaders(D3DDevice);
	arrow1PointTie.InitializeShaders(D3DDevice);
	arrow1Shaft.InitializeShaders(D3DDevice);
	arrow1Fletching1.InitializeShaders(D3DDevice);
	arrow1Fletching2.InitializeShaders(D3DDevice);
	arrow1Nock.InitializeShaders(D3DDevice);

	// arrow 2
	arrow2PointTop.InitializeShaders(D3DDevice);
	arrow2PointBottom.InitializeShaders(D3DDevice);
	arrow2PointTie.InitializeShaders(D3DDevice);
	arrow2Shaft.InitializeShaders(D3DDevice);
	arrow2Fletching1.InitializeShaders(D3DDevice);
	arrow2Fletching2.InitializeShaders(D3DDevice);
	arrow2Nock.InitializeShaders(D3DDevice);

	// arrow 3
	arrow3PointTop.InitializeShaders(D3DDevice);
	arrow3PointBottom.InitializeShaders(D3DDevice);
	arrow3PointTie.InitializeShaders(D3DDevice);
	arrow3Shaft.InitializeShaders(D3DDevice);
	arrow3Fletching1.InitializeShaders(D3DDevice);
	arrow3Fletching2.InitializeShaders(D3DDevice);
	arrow3Nock.InitializeShaders(D3DDevice);
}

//----------------------------------------------------------------------------------------------
// Window message handler
//----------------------------------------------------------------------------------------------
LRESULT MyProject::ProcessWindowMessages(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// for mouse positions
	Vector2 pos((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));

	switch (msg )
	{
	case WM_MOUSEMOVE:
		mouseDelta = pos - mousePos;
		mousePos = pos;
		OnMouseMove();
		return 0;
	case WM_LBUTTONUP:
		buttonDown = false;
		mouseDelta = pos - mousePos;
		mousePos = pos;
		break;
	case WM_LBUTTONDOWN:
		buttonDown = true;
		mouseDelta = pos - mousePos;
		mousePos = pos;
		OnMouseDown();
		break;
	case WM_KEYUP:
		if (wParam >= '0' && wParam <= '4')
		{
			PresentInterval = wParam - '0';
		}
		else if (wParam == VK_UP)	{	cameraRotationSpeed.y = 0;	}
		else if (wParam == VK_DOWN) {	cameraRotationSpeed.y = 0; }
		else if (wParam == VK_LEFT) {	cameraRotationSpeed.x = 0; }
		else if (wParam == VK_RIGHT){	cameraRotationSpeed.x = 0; }
		else if (wParam == VK_ADD)  {	cameraRadiusSpeed = 0; }
		else if (wParam == VK_SUBTRACT)  { cameraRadiusSpeed = 0; }
		else if (wParam == VK_SPACE)
		{
			cameraRotation = Vector2::Zero;
			cameraRadius   = 6;
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP)	{ cameraRotationSpeed.y = CAMERA_SPEED; }
		else if (wParam == VK_DOWN) { cameraRotationSpeed.y = -CAMERA_SPEED; }
		else if (wParam == VK_LEFT) { cameraRotationSpeed.x = -CAMERA_SPEED; }
		else if (wParam == VK_RIGHT){ cameraRotationSpeed.x = CAMERA_SPEED; }
		else if (wParam == VK_ADD)  { cameraRadiusSpeed = -1.0f; }
		else if (wParam == VK_SUBTRACT)  { cameraRadiusSpeed = 1.0f; }
		break;

	}

	// let the base class handle remaining messages
	return DirectXClass::ProcessWindowMessages(msg, wParam, lParam);
}

//----------------------------------------------------------------------------------------------
// Called by the render loop to render a single frame
//----------------------------------------------------------------------------------------------
void MyProject::Render(void)
{
	// calculate camera matrices
	ComputeViewProjection();

	// drawing primitives
	cylinder.Draw(DeviceContext, cylinderWorldMatrix, viewMatrix, projectionMatrix);
	cone.Draw(DeviceContext, coneWorldMatrix, viewMatrix, projectionMatrix);
	rhombuses.Draw(DeviceContext, rhombusWorldMatrix, viewMatrix, projectionMatrix);

	// drawing bow
	grip.Draw(DeviceContext, gripWorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	upperGripBase1.Draw(DeviceContext, upperGripBase1WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	upperGripBase2.Draw(DeviceContext, upperGripBase2WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	lowerGripBase1.Draw(DeviceContext, lowerGripBase1WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	lowerGripBase2.Draw(DeviceContext, lowerGripBase2WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	upperLimb1.Draw(DeviceContext, upperLimb1WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	upperLimb2.Draw(DeviceContext, upperLimb2WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	upperLimb3.Draw(DeviceContext, upperLimb3WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	upperLimbEnd.Draw(DeviceContext, upperLimbEndWorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	lowerLimb1.Draw(DeviceContext, lowerLimb1WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	lowerLimb2.Draw(DeviceContext, lowerLimb2WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	lowerLimb3.Draw(DeviceContext, lowerLimb3WorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	lowerLimbEnd.Draw(DeviceContext, lowerLimbEndWorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	upperTie.Draw(DeviceContext, upperTieWorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	lowerTie.Draw(DeviceContext, lowerTieWorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	string.Draw(DeviceContext, stringWorldMatrix * bowParentWorldMatrix, viewMatrix, projectionMatrix);
	
	// drawing arrow 1
	arrow1PointTop.Draw(DeviceContext, arrow1PointTopWorldMatrix * arrow1ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow1PointBottom.Draw(DeviceContext, arrow1PointBottomWorldMatrix * arrow1ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow1PointTie.Draw(DeviceContext, arrow1PointTieWorldMatrix * arrow1ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow1Shaft.Draw(DeviceContext, arrow1ShaftWorldMatrix * arrow1ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow1Fletching1.Draw(DeviceContext, arrow1Fletching1WorldMatrix * arrow1ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow1Fletching2.Draw(DeviceContext, arrow1Fletching2WorldMatrix * arrow1ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow1Nock.Draw(DeviceContext, arrow1NockWorldMatrix * arrow1ParentWorldMatrix, viewMatrix, projectionMatrix);

	// drawing arrow 2
	arrow2PointTop.Draw(DeviceContext, arrow2PointTopWorldMatrix * arrow2ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow2PointBottom.Draw(DeviceContext, arrow2PointBottomWorldMatrix * arrow2ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow2PointTie.Draw(DeviceContext, arrow2PointTieWorldMatrix * arrow2ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow2Shaft.Draw(DeviceContext, arrow2ShaftWorldMatrix * arrow2ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow2Fletching1.Draw(DeviceContext, arrow2Fletching1WorldMatrix * arrow2ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow2Fletching2.Draw(DeviceContext, arrow2Fletching2WorldMatrix * arrow2ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow2Nock.Draw(DeviceContext, arrow2NockWorldMatrix * arrow2ParentWorldMatrix, viewMatrix, projectionMatrix);

	// drawing arrow 3
	arrow3PointTop.Draw(DeviceContext, arrow3PointTopWorldMatrix * arrow3ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow3PointBottom.Draw(DeviceContext, arrow3PointBottomWorldMatrix * arrow3ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow3PointTie.Draw(DeviceContext, arrow3PointTieWorldMatrix * arrow3ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow3Shaft.Draw(DeviceContext, arrow3ShaftWorldMatrix * arrow3ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow3Fletching1.Draw(DeviceContext, arrow3Fletching1WorldMatrix * arrow3ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow3Fletching2.Draw(DeviceContext, arrow3Fletching2WorldMatrix * arrow3ParentWorldMatrix, viewMatrix, projectionMatrix);
	arrow3Nock.Draw(DeviceContext, arrow3NockWorldMatrix * arrow3ParentWorldMatrix, viewMatrix, projectionMatrix);

	// render the base class
	DirectXClass::Render();
}

//----------------------------------------------------------------------------------------------
// Called every frame to update objects.
//	deltaTime: how much time in seconds has elapsed since the last frame
//----------------------------------------------------------------------------------------------
void MyProject::Update(float deltaTime)
{
	UpdateCamera(deltaTime);

	// slowly change rotation
	modelRotation += deltaTime * XM_PI / 10.0f;

	// setting primitive transforms
	cylinderWorldMatrix = Matrix::CreateRotationX(modelRotation) *
		Matrix::CreateRotationY(modelRotation) * 
		Matrix::CreateRotationZ(-modelRotation) *
		Matrix::CreateTranslation(2.75f, -1.75f, 2.0f);

	coneWorldMatrix = Matrix::CreateRotationX(-modelRotation) *
		Matrix::CreateRotationY(modelRotation) * 
		Matrix::CreateRotationZ(modelRotation) *
		Matrix::CreateTranslation(-2.75f, -1.75f, 2.0f);

	rhombusWorldMatrix = Matrix::CreateRotationY(modelRotation) * 
		Matrix::CreateRotationZ(modelRotation) * 
		Matrix::CreateTranslation(0.0f, -1.75f, 2.0f);

	// setting up parts of bow
	gripWorldMatrix = Matrix::CreateScale(0.25f, 0.55f, 0.25f);

	upperGripBase1WorldMatrix = Matrix::CreateScale(0.7f, 0.08f, 0.7f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateTranslation(0.03f, 0.6f, 0.0f);

	upperGripBase2WorldMatrix = Matrix::CreateScale(0.7f, 0.08f, 0.7f) *
		Matrix::CreateTranslation(0.03f, 0.78f, 0.0f);

	lowerGripBase1WorldMatrix = Matrix::CreateScale(0.7f, 0.08f, 0.7f) *
		Matrix::CreateTranslation(0.03f, -0.6f, 0.0f);

	lowerGripBase2WorldMatrix = Matrix::CreateScale(0.7f, 0.08f, 0.7f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateTranslation(0.03f, -0.78f, 0.0f);

	upperLimb1WorldMatrix = Matrix::CreateScale(0.35f, 0.7f, 0.5f) *
		Matrix::CreateRotationZ(XM_PI / 9.0f) *
		Matrix::CreateTranslation(-0.27f, 1.5f, 0.0f);

	upperLimb2WorldMatrix = Matrix::CreateScale(0.35f, 0.6f, 0.5f) *
		Matrix::CreateRotationZ(XM_PI * 0.2f) *
		Matrix::CreateTranslation(-0.96f, 2.825f, 0.0f);

	upperLimb3WorldMatrix = Matrix::CreateScale(0.35f, 0.6f, 0.5f) *
		Matrix::CreateRotationZ(XM_PI / 3.3f) *
		Matrix::CreateTranslation(-1.94f, 3.79f, 0.0f);

	upperLimbEndWorldMatrix = Matrix::CreateScale(0.4f, 0.2f, 0.5f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateRotationZ(XM_PI * 0.1f) *
		Matrix::CreateTranslation(-2.47f, 4.26f, 0.0f);

	lowerLimb1WorldMatrix = Matrix::CreateScale(0.35f, 0.7f, 0.5f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateRotationZ(-XM_PI / 9.0f) *
		Matrix::CreateTranslation(-0.27f, -1.5f, 0.0f);

	lowerLimb2WorldMatrix = Matrix::CreateScale(0.35f, 0.6f, 0.5f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateRotationZ(-XM_PI * 0.2f) *
		Matrix::CreateTranslation(-0.96f, -2.825f, 0.0f);

	lowerLimb3WorldMatrix = Matrix::CreateScale(0.35f, 0.6f, 0.5f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateRotationZ(-XM_PI / 3.3f) *
		Matrix::CreateTranslation(-1.94f, -3.79f, 0.0f);

	lowerLimbEndWorldMatrix = Matrix::CreateScale(0.4f, 0.2f, 0.5f) *
		Matrix::CreateRotationZ(-XM_PI * 0.1f) *
		Matrix::CreateTranslation(-2.47f, -4.26f, 0.0f);

	upperTieWorldMatrix = Matrix::CreateScale(0.2f, 0.07f, 0.23f) *
		Matrix::CreateRotationZ(XM_PI * 0.25f) *
		Matrix::CreateTranslation(-2.37f, 4.1f, 0.0f);

	lowerTieWorldMatrix = Matrix::CreateScale(0.2f, 0.07f, 0.23f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateRotationZ(-XM_PI * 0.25f) *
		Matrix::CreateTranslation(-2.37f, -4.1f, 0.0f);

	stringWorldMatrix = Matrix::CreateScale(0.07f, 3.46f, 0.07f) *
		Matrix::CreateTranslation(-2.37f, 0.0f, 0.0f);

	// setting up parts of arrow 1
	arrow1PointTopWorldMatrix = Matrix::CreateScale(0.42f, 0.18f, 0.42f) *
		Matrix::CreateTranslation(0.0f, 3.34f, 0.0f);

	arrow1PointBottomWorldMatrix = Matrix::CreateScale(0.42f, 0.04f, 0.42f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateTranslation(0.0f, 3.08f, 0.0f);

	arrow1PointTieWorldMatrix = Matrix::CreateScale(0.23f, 0.06f, 0.23f) *
		Matrix::CreateTranslation(0.0f, 3.0f, 0.0f);

	arrow1ShaftWorldMatrix = Matrix::CreateScale(0.13f, 2.45f, 0.13f);

	arrow1Fletching1WorldMatrix = Matrix::CreateScale(0.24f, 0.5f, 1.0f) *
		Matrix::CreateTranslation(0.0f, -2.35f, 0.0f);

	arrow1Fletching2WorldMatrix = Matrix::CreateScale(0.24f, 0.5f, 1.0f) *
		Matrix::CreateRotationY(XM_PI * 0.5f) * 
		Matrix::CreateTranslation(0.0f, -2.35f, 0.0f);

	arrow1NockWorldMatrix = Matrix::CreateScale(0.15f, 0.12f, 0.15f) *
		Matrix::CreateTranslation(0.0f, -2.87f, 0.0f);

	// setting up parts of arrow 2
	arrow2PointTopWorldMatrix = Matrix::CreateScale(0.42f, 0.18f, 0.42f) *
		Matrix::CreateTranslation(0.0f, 3.34f, 0.0f);

	arrow2PointBottomWorldMatrix = Matrix::CreateScale(0.42f, 0.04f, 0.42f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateTranslation(0.0f, 3.08f, 0.0f);

	arrow2PointTieWorldMatrix = Matrix::CreateScale(0.23f, 0.06f, 0.23f) *
		Matrix::CreateTranslation(0.0f, 3.0f, 0.0f);

	arrow2ShaftWorldMatrix = Matrix::CreateScale(0.13f, 2.45f, 0.13f);

	arrow2Fletching1WorldMatrix = Matrix::CreateScale(0.24f, 0.5f, 1.0f) *
		Matrix::CreateTranslation(0.0f, -2.35f, 0.0f);

	arrow2Fletching2WorldMatrix = Matrix::CreateScale(0.24f, 0.5f, 1.0f) *
		Matrix::CreateRotationY(XM_PI * 0.5f) *
		Matrix::CreateTranslation(0.0f, -2.35f, 0.0f);

	arrow2NockWorldMatrix = Matrix::CreateScale(0.15f, 0.12f, 0.15f) *
		Matrix::CreateTranslation(0.0f, -2.87f, 0.0f);

	// setting up parts of arrow 3
	arrow3PointTopWorldMatrix = Matrix::CreateScale(0.42f, 0.18f, 0.42f) *
		Matrix::CreateTranslation(0.0f, 3.34f, 0.0f);

	arrow3PointBottomWorldMatrix = Matrix::CreateScale(0.42f, 0.04f, 0.42f) *
		Matrix::CreateRotationX(XM_PI) *
		Matrix::CreateTranslation(0.0f, 3.08f, 0.0f);

	arrow3PointTieWorldMatrix = Matrix::CreateScale(0.23f, 0.06f, 0.23f) *
		Matrix::CreateTranslation(0.0f, 3.0f, 0.0f);

	arrow3ShaftWorldMatrix = Matrix::CreateScale(0.13f, 2.45f, 0.13f);

	arrow3Fletching1WorldMatrix = Matrix::CreateScale(0.24f, 0.5f, 1.0f) *
		Matrix::CreateTranslation(0.0f, -2.35f, 0.0f);

	arrow3Fletching2WorldMatrix = Matrix::CreateScale(0.24f, 0.5f, 1.0f) *
		Matrix::CreateRotationY(XM_PI * 0.5f) *
		Matrix::CreateTranslation(0.0f, -2.35f, 0.0f);

	arrow3NockWorldMatrix = Matrix::CreateScale(0.15f, 0.12f, 0.15f) *
		Matrix::CreateTranslation(0.0f, -2.87f, 0.0f);

	// set up bow transforms
	bowParentWorldMatrix = Matrix::CreateScale(0.65f) *
		Matrix::CreateRotationZ(XM_PI * 0.2f) *
		Matrix::CreateTranslation(1.0f, 0.0f, 0.0f);

	bowParentWorldMatrix *= bowArrowJointWorldMatrix;		// connecting bow to joint matrix

	// set up arrow 1 transforms
	arrow1ParentWorldMatrix = Matrix::CreateScale(1.0f) *
		Matrix::CreateRotationZ(-XM_PI * 0.5f) * 
		Matrix::CreateTranslation(-1.5f, 0.0f, 0.15f);

	arrow1ParentWorldMatrix *= bowParentWorldMatrix;		// connecting arrow 1 to bow

	// set up bow and arrow 1 connection
	bowArrowJointWorldMatrix = Matrix::CreateRotationY(modelRotation * 2.0f) *
		Matrix::CreateTranslation(1.75f, 3.0f, -2.0f);

	// set up arrow 2 transforms
	arrow2ParentWorldMatrix = Matrix::CreateScale(0.65f) *
		Matrix::CreateRotationY(modelRotation * 3.0f) * 
		Matrix::CreateRotationZ(XM_PI) *
		Matrix::CreateTranslation(-3.0f, 2.25f, -2.0f);

	// set up arrow 3 transforms
	arrow3ParentWorldMatrix = Matrix::CreateScale(1.0f) *
		Matrix::CreateRotationZ(XM_PI * 0.05f) *
		Matrix::CreateTranslation(0.1f, 0.1f, -0.1f);

	arrow3ParentWorldMatrix *= arrow2ParentWorldMatrix;		// connecting arrow 3 to arrow 2
}


//----------------------------------------------------------------------------------------------
// Called when the mouse is released
//----------------------------------------------------------------------------------------------
void MyProject::OnMouseDown()
{

	// this is called when the left mouse button is clicked
	// mouse position is stored in mousePos variable
}

//----------------------------------------------------------------------------------------------
// Called when the mouse is moved
//----------------------------------------------------------------------------------------------
void MyProject::OnMouseMove()
{
}

//----------------------------------------------------------------------------------------------
// Computes the view and camera matrix
//----------------------------------------------------------------------------------------------
void MyProject::ComputeViewProjection()
{
	viewMatrix = Matrix::CreateLookAt(cameraPos, Vector3::Zero, Vector3::UnitY);
	projectionMatrix = Matrix::CreatePerspectiveFieldOfView(60.0f * XM_PI / 180.0f, (float)clientWidth / (float)clientHeight, 1, 20);
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void MyProject::UpdateCamera(float deltaTime)
{
	const float VERT_LIMIT = XM_PI * 0.35f;

	// update the radius
	cameraRadius += cameraRadiusSpeed * deltaTime;
	if (cameraRadius < 1) cameraRadius = 1;


	// update the rotation amounts
	cameraRotation += cameraRotationSpeed * deltaTime;

	// clamp the vertical rotation
	if (cameraRotation.y < -VERT_LIMIT) cameraRotation.y = -VERT_LIMIT;
	else if (cameraRotation.y > VERT_LIMIT) cameraRotation.y = VERT_LIMIT;

	// calculate the height
	cameraPos.y = cameraRadius * sinf(cameraRotation.y);
	float r = cameraRadius * cosf(cameraRotation.y);

	// calculate the orbit
	cameraPos.x = sinf(cameraRotation.x) * r;
	cameraPos.z = cosf(cameraRotation.x) * r;
}