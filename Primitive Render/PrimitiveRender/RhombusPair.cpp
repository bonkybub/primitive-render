//
//	Class that represents a rhombus
//

#include "RhombusPair.h"
#include <DirectXColors.h>

//  the vertices
VertexPositionColor RhombusPair::vertices[NUM_VERTS] =
{
	// front faces
	{ Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.95f, 0.46f, 1.0f) },
	{ Vector3(-0.7f, -0.7f, 0.0f), Color(1.0f, 0.95f, 0.46f, 1.0f) },
	{ Vector3(-0.7f, 0.7f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.95f, 0.46f, 1.0f) },
	{ Vector3(-0.7f, 0.7f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.0f, 1.4f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.95f, 0.46f, 1.0f) },
	{ Vector3(0.0f, 1.4f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.7f, 0.7f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.95f, 0.46f, 1.0f) },
	{ Vector3(0.7f, 0.7f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.7f, -0.7f, 0.0f), Color(1.0f, 0.95f, 0.46f, 1.0f) },

	// back faces
	{ Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.24f, 0.22f, 1.0f) },
	{ Vector3(-0.7f, 0.7f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(-0.7f, -0.7f, 0.0f), Color(1.0f, 0.24f, 0.22f, 1.0f) },
	{ Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.24f, 0.22f, 1.0f) },
	{ Vector3(0.0f, 1.4f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(-0.7f, 0.7f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.24f, 0.22f, 1.0f) },
	{ Vector3(0.7f, 0.7f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.0f, 1.4f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) },
	{ Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 0.24f, 0.22f, 1.0f) },
	{ Vector3(0.7f, -0.7f, 0.0f), Color(1.0f, 0.24f, 0.22f, 1.0f) },
	{ Vector3(0.7f, 0.7f, 0.0f), Color(1.0f, 0.55f, 0.26f, 1.0f) }
};

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
RhombusPair::RhombusPair()
{
	pVertexBuffer = nullptr;
	pEffect = nullptr;
	pInputLayout = nullptr;
	objColor = Color(1.0f, 1.0f, 1.0f, 0.0f);
	*coloredVerts = *vertices;
}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
RhombusPair::~RhombusPair()
{
	// clean up whatever was allocated
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
void RhombusPair::InitializeGeometry(ID3D11Device* pDevice)
{
	// create the buffer
	
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
	if (objColor.A() > 0.0f)
		data.pSysMem = coloredVerts;
	else
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
void RhombusPair::InitializeShaders(ID3D11Device* pDevice)
{
	// create and configure the shader
	pEffect = new BasicEffect(pDevice);
	pEffect->SetLightingEnabled(false);
	pEffect->SetTextureEnabled(false);
	pEffect->SetVertexColorEnabled(true);

	//	define the vertex input layout
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		// first input element: position
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

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
// Draw the rhombus
// ------------------------------------------------------------------------------------
void RhombusPair::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& world, const Matrix& view, const Matrix& projection)
{
	// set up the shader inputs and apply the shader
	pEffect->SetWorld(world);
	pEffect->SetView(view);
	pEffect->SetProjection(projection);
	pEffect->Apply(pDeviceContext);

	// set up our input layout
	pDeviceContext->IASetInputLayout(pInputLayout);

	// tell D3D we are drawing a triangle list
	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// tell the device which vertex buffer we are using
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

	// tell it to draw the first three vertices
	pDeviceContext->Draw(NUM_VERTS, 0);
}

// ------------------------------------------------------------------------------------
// Set solid colour for rhombus pair
// ------------------------------------------------------------------------------------
void RhombusPair::SetColor(Color c)
{
	objColor = c;

	for (int i = 0; i < NUM_VERTS; i++)
		coloredVerts[i] = { vertices[i].position, objColor };
}