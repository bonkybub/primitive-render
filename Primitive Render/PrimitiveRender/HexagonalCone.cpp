//
//	Class that represents a hexagonal cone
//

#include "HexagonalCone.h"
#include <DirectXColors.h>

// the vertices
VertexPositionColor HexagonalCone::vertices[NUM_VERTS] =
{
	{ Vector3(0.0f , -1.2f , 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f) },		// Vertex 0
	{ Vector3(-0.2f , -1.2f , -0.36f), Color(0.66f, 1.0f, 0.97f, 1.0f) },	// Vertex 1
	{ Vector3(0.2f , -1.2f , -0.36f), Color(0.66f, 1.0f, 0.97f, 1.0f) },	// Vertex 2
	{ Vector3(0.4f , -1.2f , 0.0f), Color(0.66f, 1.0f, 0.97f, 1.0f) },		// Vertex 3
	{ Vector3(-0.2f , -1.2f , 0.36f), Color(0.58f, 0.98f, 0.67f, 1.0f) },	// Vertex 4
	{ Vector3(-0.4f , -1.2f , 0.0f), Color(0.58f, 0.98f, 0.67f, 1.0f) },	// Vertex 5
	{ Vector3(0.2f , -1.2f , 0.36f), Color(0.58f, 0.98f, 0.67f, 1.0f) },	// Vertex 6
	{ Vector3(0.0f , 1.2f , 0.0f), Color(0.91f, 0.68f, 0.72f, 1.0f) },		// Vertex 7
};

// the indices
DWORD HexagonalCone::indices[NUM_INDICES] =
{
	0, 2, 1,
	0, 3, 2,
	0, 6, 3,
	0, 4, 6,
	0, 5, 4,
	0, 1, 5,
	7, 1, 2,
	7, 2, 3,
	7, 3, 6,
	7, 6, 4,
	7, 4, 5,
	7, 5, 1
};

// ------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------
HexagonalCone::HexagonalCone()
{
	pVertexBuffer = nullptr;
	pEffect = nullptr;
	pInputLayout = nullptr;
	pIndexBuffer = nullptr;
	objColor = Color(1.0f, 1.0f, 1.0f, 0.0f);
	*coloredVerts = *vertices;
}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
HexagonalCone::~HexagonalCone()
{
	// clean up whatever was allocated
	delete pEffect;
	if (pVertexBuffer != nullptr)
	{
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}
	if (pIndexBuffer != nullptr)
	{
		pIndexBuffer->Release();
		pIndexBuffer = nullptr;
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
void HexagonalCone::InitializeGeometry(ID3D11Device* pDevice)
{
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

	// set up  the index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = NUM_INDICES * sizeof(DWORD);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;

	hr = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	if (FAILED(hr))
	{
		OutputDebugString(L"FAILED TO CREATE INDEX BUFFER");
		assert(false);
	}
}

// ------------------------------------------------------------------------------------
// Initializes the shaders
// ------------------------------------------------------------------------------------
void HexagonalCone::InitializeShaders(ID3D11Device* pDevice)
{
	// create and configure the shader
	pEffect = new BasicEffect(pDevice);
	pEffect->SetLightingEnabled(false);
	pEffect->SetTextureEnabled(false);
	pEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;
	pEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	// create the input layout
	HRESULT hr = pDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		&pInputLayout);
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create input layout");
		assert(0);
	}
}

// ------------------------------------------------------------------------------------
// Draw the cone
// ------------------------------------------------------------------------------------
void HexagonalCone::Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& world, const Matrix& view, const Matrix& projection)
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

	// set the index buffer
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// tell it to draw the primitive
	pDeviceContext->DrawIndexed(NUM_INDICES, 0, 0);
}

// ------------------------------------------------------------------------------------
// Set solid colour for cone
// ------------------------------------------------------------------------------------
void HexagonalCone::SetColor(Color c)
{
	objColor = c;

	for (int i = 0; i < NUM_VERTS; i++)
		coloredVerts[i] = { vertices[i].position, objColor };
}