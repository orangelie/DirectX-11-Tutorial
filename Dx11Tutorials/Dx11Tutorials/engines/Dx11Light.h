#pragma once
#include "../includes/ZekrosEngine.h"

struct Vertex {
	XMFLOAT3 Position;
	XMFLOAT4 Color;
};

namespace Colorsx {
	static const XMFLOAT4 Aqua = { 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
	static const XMFLOAT4 Blue = { 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
	static const XMFLOAT4 Coral = { 1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f };
	static const XMFLOAT4 DarkOrange = { 1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f };
	static const XMFLOAT4 Green = { 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f };
	static const XMFLOAT4 Honeydew = { 0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f };
	static const XMFLOAT4 White = { 1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
	static const XMFLOAT4 Yellow = { 1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
}

class Dx11Light : public orangelie::Engine::ZekrosEngine {
private:
	void BuildGeometyBuffers() {
		orangelie::Mesh::GeometryGenerator::MeshData box;


		orangelie::Mesh::GeometryGenerator geoGen;
		geoGen.CreateBox(1.0f, 1.0f, 1.0f, box);

		XMFLOAT4 Colors[] = {
			Colorsx::Aqua,
			Colorsx::Blue,
			Colorsx::Green,
			Colorsx::Honeydew,
			Colorsx::White,
			Colorsx::Yellow,
			Colorsx::DarkOrange,
			Colorsx::Coral
		};

		std::vector<Vertex> vertices(box.Vertices.size());
		for (size_t i = 0; i < box.Vertices.size(); ++i) {
			vertices[i].Position = box.Vertices[i].Position;
			vertices[i].Color = Colors[i % 8];
		}

		std::vector<UINT> indices = box.Indices;

		D3D11_BUFFER_DESC verticesDesc = {};
		verticesDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		verticesDesc.ByteWidth = vertices.size() * sizeof(Vertex);
		verticesDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		verticesDesc.MiscFlags = 0;
		verticesDesc.StructureByteStride = 0;
		verticesDesc.Usage = D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA verticesSubResourcesDesc = {};
		verticesSubResourcesDesc.pSysMem = vertices.data();

		HR(m_Device->CreateBuffer(&verticesDesc, &verticesSubResourcesDesc, m_VertexBuffer.GetAddressOf()));


		D3D11_BUFFER_DESC indicesDesc = {};
		indicesDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		indicesDesc.ByteWidth = indices.size() * sizeof(UINT);
		indicesDesc.CPUAccessFlags = 0;
		indicesDesc.MiscFlags = 0;
		indicesDesc.StructureByteStride = 0;
		indicesDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA indicesSubResourcesDesc = {};
		indicesSubResourcesDesc.pSysMem = indices.data();

		HR(m_Device->CreateBuffer(&indicesDesc, &indicesSubResourcesDesc, m_IndexBuffer.GetAddressOf()));

		m_BoxIndexCount = indices.size();
		m_BoxVertexOffset = m_BoxIndexOffset = 0;
	}

	void BuildFX() {
		DWORD shaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		ComPtr<ID3D10Blob> ShaderBins = nullptr;
		ComPtr<ID3D10Blob> ErrorMsgs = nullptr;

		HRESULT hResult = D3DX11CompileFromFile(
			"FX/shader.fx",
			nullptr, nullptr, nullptr,
			"fx_5_0",
			shaderFlags,
			0, nullptr,
			ShaderBins.GetAddressOf(),
			ErrorMsgs.GetAddressOf(),
			nullptr);

		if (ErrorMsgs != nullptr) {
			MessageBoxA(0, (char*)ErrorMsgs->GetBufferPointer(), "## Shader Compile Error ##", MB_OK);
			HR(-1);
		}
		HR(hResult);


		HR(D3DX11CreateEffectFromMemory(
			ShaderBins->GetBufferPointer(),
			ShaderBins->GetBufferSize(),
			0,
			m_Device.Get(),
			m_FX.GetAddressOf()));

		//HR(D3DX11CreateEffectFromFile(L"FX/shader.fxo", 0, m_Device.Get(), m_FX.GetAddressOf());

		m_Tech = m_FX->GetTechniqueByName("ColorTech");

		m_gWorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
		m_gDeltaTime = m_FX->GetVariableByName("gDeltaTime")->AsScalar();
		m_gTotalTime = m_FX->GetVariableByName("gTotalTime")->AsScalar();
	}

	void BuildShaderAndInputLayout() {
		m_InputElements = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		D3DX11_PASS_DESC passDesc = {};
		HR(m_Tech->GetPassByIndex(0)->GetDesc(&passDesc));
		HR(m_Device->CreateInputLayout(
			m_InputElements.data(),
			(UINT)m_InputElements.size(),
			passDesc.pIAInputSignature,
			(UINT)passDesc.IAInputSignatureSize,
			&m_InputLayout));
	}

protected:
	// Virtuals
	virtual void init() {
		BuildGeometyBuffers();
		BuildFX();
		BuildShaderAndInputLayout();

		XMStoreFloat4x4(&m_World, XMMatrixIdentity());

		auto scale = XMMatrixScaling(2.0f, 1.0f, 2.0f);
		auto offset = XMMatrixTranslation(0.0f, 0.5f, 0.0f);
		XMStoreFloat4x4(&m_World, XMMatrixMultiply(XMLoadFloat4x4(&m_World), XMMatrixMultiply(scale, offset)));
	}

	virtual void update(float dt) {
		static float t = 0.0f;
		t += dt;

		float x = m_Radius * sinf(m_Phi) * cosf(m_Theta);
		float y = m_Radius * cosf(m_Phi);
		float z = m_Radius * sinf(m_Phi) * sinf(m_Theta);

		XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
		XMVECTOR target = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
		XMStoreFloat4x4(&m_View, V);

		m_gDeltaTime->SetFloat(dt);
		m_gTotalTime->SetFloat(t);
	}

	virtual void draw(float dt) {
		assert(m_SwapChain);

		const FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		m_ImmediateContext->ClearRenderTargetView(m_Rtv.Get(), color);
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// ---------------------------------------------------------------------------------------------------

		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		m_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_ImmediateContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		m_ImmediateContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		m_ImmediateContext->IASetInputLayout(m_InputLayout.Get());

		XMMATRIX W = XMLoadFloat4x4(&m_World);
		XMMATRIX V = XMLoadFloat4x4(&m_View);
		XMMATRIX P = XMLoadFloat4x4(&m_Projection);
		XMMATRIX WVP = W * V * P;

		m_gWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WVP));

		D3DX11_TECHNIQUE_DESC techniqueDesc = {};
		m_Tech->GetDesc(&techniqueDesc);

		for (uint32_t i = 0; i < techniqueDesc.Passes; i++) {
			m_Tech->GetPassByIndex(i)->Apply(0, m_ImmediateContext.Get());
			m_ImmediateContext->DrawIndexed(m_BoxIndexCount, m_BoxIndexOffset, m_BoxVertexOffset);
		}


		// ---------------------------------------------------------------------------------------------------

		m_SwapChain->Present(0, 0);
	}

	virtual void resize(float dt) {

		XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * XM_PI, (float)m_ClientWidth / (float)m_ClientHeight, 1.0f, 1000.0f);
		XMStoreFloat4x4(&m_Projection, P);
	}

	// Mouse Controller
	virtual void MouseDown(WPARAM btnState, int x, int y) {
		m_LastMousePos.x = x;
		m_LastMousePos.y = y;

		SetCapture(m_hWnd);
	}

	virtual void MouseUp(WPARAM btnState, int x, int y) {
		ReleaseCapture();
	}

	virtual void MouseMove(WPARAM btnState, int x, int y) {
		if ((btnState & MK_RBUTTON) != 0) {
			float dx = 0.005f * static_cast<float>(x - m_LastMousePos.x);
			float dy = 0.005f * static_cast<float>(y - m_LastMousePos.y);

			m_Radius += dx - dy;

			m_Radius = CLAMP(m_Radius, 0.0f, 15.0f);
		}

		else if ((btnState & MK_LBUTTON) != 0) {
			float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_LastMousePos.x));
			float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_LastMousePos.y));

			m_Theta += dx;
			m_Phi += dy;

			m_Phi = CLAMP(m_Phi, 0.1f, XM_PI - 0.1f);
		}

		m_LastMousePos.x = x;
		m_LastMousePos.y = y;
	}

private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_InputElements;
	ComPtr<ID3DX11Effect> m_FX = nullptr;
	ComPtr<ID3DX11EffectTechnique> m_Tech = nullptr;
	ComPtr<ID3D11InputLayout> m_InputLayout = nullptr;

	ComPtr<ID3D11Buffer> m_VertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> m_IndexBuffer = nullptr;

	ID3DX11EffectMatrixVariable* m_gWorldViewProj = nullptr;
	ID3DX11EffectScalarVariable* m_gDeltaTime = nullptr;
	ID3DX11EffectScalarVariable* m_gTotalTime = nullptr;

	XMFLOAT4X4 m_World, m_View, m_Projection;

	POINT m_LastMousePos = {};
	float m_Phi = 0.25f;
	float m_Theta = 0.1f;
	float m_Radius = 5.0f;

	UINT m_BoxIndexCount, m_BoxVertexOffset, m_BoxIndexOffset;

};