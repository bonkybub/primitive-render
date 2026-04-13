//
// BGTD 9201
//	Class that represents a simple primitive
//

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Effects.h>
#include <VertexTypes.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

using DirectX::VertexPositionColor;

class Primitive
{
public:
	Primitive();
	~Primitive();

	void InitializeGeometry(ID3D11Device* pDevice);
	void InitializeShaders(ID3D11Device* pDevice);
	void Draw(ID3D11DeviceContext* pDeviceContext, const Matrix& world, const Matrix& view, const Matrix& projection);


private:
	ID3D11Buffer* pVertexBuffer;
	ID3D11InputLayout* pInputLayout;
	BasicEffect*  pEffect;

	static const int NUM_VERTS = 3;
	static VertexPositionColor vertices[NUM_VERTS];

};