//
// BGTD 9201
//	Class that represents a simple primitive
//

#include "Primitive.h"
#include <DirectXColors.h>



// Step 2
//  Create an array of vertices
VertexPositionColor Primitive::vertices[NUM_VERTS] =
{
	{ Vector3(0.0f, 1.0f, 0.0f), Colors::Red.v },
	{ Vector3(1.0f, -1.0f, 0.0f), Colors::Green.v },
	{ Vector3(-1.0f, -1.0f, 0.0f), Colors::Blue.v }
};

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
Primitive::Primitive()
{
	pVertexBuffer = nullptr;
	pEffect = nullptr;
	pInputLayout = nullptr;
}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
Primitive::~Primitive()
{
	// Make sure we clean up what ever we allocated!
	delete pEffect;
	if (pVertexBuffer != nullptr)
	{
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}

	if (pInputLayout != nullptr)
	{
		pInputLayout->Release();
		pInputLayout = nullptr;
	}
}

// ------------------------------------------------------------------------------------
// Initialize the vertex buffer
// ------------------------------------------------------------------------------------
void Primitive::InitializeGeometry(ID3D11Device* pDevice)
{
	// Step 3
	// Create the buffer

	// describe the vertex buffer we are trying to create
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = NUM_VERTS * sizeof(VertexPositionColor);
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	// setup the subresource data - tells D3D what data to use to initialize
	// the vertexbuffer with
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertices;

	// create the vertex buffer
	HRESULT hr = pDevice->CreateBuffer(&desc, &data, &pVertexBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"FAILED TO CREATE VERTEX BUFFER");
		assert(false);
	}
}

// ------------------------------------------------------------------------------------
// Initialzes the shaders
// ------------------------------------------------------------------------------------
void Primitive::InitializeShaders(ID3D11Device* pDevice)
{
	// Step 4
	// create and configure the shader
	pEffect = new BasicEffect(pDevice);
	pEffect->SetLightingEnabled(false);
	pEffect->SetTextureEnabled(false);
	pEffect->SetVertexColorEnabled(true);


	// Step 5
	//	Define the vertex input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		// first input element: position
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Step 6
	// create the input layout object

	// get the information about the vertex shader so we can create our vertex layout
	// get the information about the vertex shader so we can get our vertex layout
	void const* shaderByteCode;
	size_t byteCodeLength;

	pEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);


	HRESULT hr = pDevice->CreateInputLayout(
		layoutDesc, 2,
		shaderByteCode, byteCodeLength,
		&pInputLayout
		);
	if (FAILED(hr))
	{
		OutputDebugString(L"Could not create input layout");
		assert(false);
	}
}

// ------------------------------------------------------------------------------------
// Draw the primitive
// ------------------------------------------------------------------------------------
void Primitive::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& world, const Matrix& view, const Matrix& projection)
{
	// Step 7
	//	Set up the shader inputs and apply the shader
	//
	pEffect->SetWorld(world);
	pEffect->SetView(view);
	pEffect->SetProjection(projection);
	pEffect->Apply(pDeviceContext);	

	// Step 8
	// Set up our input layout
	pDeviceContext->IASetInputLayout(pInputLayout);

	// Step 9
	//  tell D3D we are drawing a triangle list
	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Step 10
	//  Tell the device which vertex buffer we are using
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// Step 11
	//	tell it to draw the first three vertices
	pDeviceContext->Draw(NUM_VERTS, 0);
}


