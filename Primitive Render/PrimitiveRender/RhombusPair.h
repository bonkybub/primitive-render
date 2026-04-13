//
//	Class that represents a rhombus
//

#define RHOMBUS_PAIR_H
#ifdef RHOMBUS_PAIR_H

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <VertexTypes.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

using DirectX::VertexPositionColor;

class RhombusPair
{
public:
	RhombusPair();
	~RhombusPair();

	void InitializeGeometry(ID3D11Device* pDevice);
	void InitializeShaders(ID3D11Device* pDevice);
	void Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& world, const Matrix& view, const Matrix& projection);

	// set solid colour for the rhombus pair
	void SetColor(Color c);


private:
	ID3D11Buffer* pVertexBuffer;
	ID3D11InputLayout* pInputLayout;
	BasicEffect* pEffect;

	static const int NUM_VERTS = 24;
	static VertexPositionColor vertices[NUM_VERTS];

	// for giving the rhombus pair one solid colour
	Color objColor;
	VertexPositionColor coloredVerts[NUM_VERTS];
};

#endif
