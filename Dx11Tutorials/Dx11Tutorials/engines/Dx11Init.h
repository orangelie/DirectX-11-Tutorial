#include "../includes/ZekrosEngine.h"

class Dx11Init : public orangelie::Engine::ZekrosEngine {
protected:
	// Screen Resize
	virtual void OnResize() {
		ZekrosEngine::OnResize();


	}

	// Virtuals
	virtual void init() {

	}

	virtual void update(float dt) {

	}

	virtual void draw(float dt) {
		assert(m_SwapChain);

		static float elapsedTime = 0.0f;
		elapsedTime += dt;

		const float cspeed = 5.0f;
		const float r = sinf((elapsedTime + cspeed) + 0);
		const float g = sinf((elapsedTime + cspeed) + 2);
		const float b = sinf((elapsedTime + cspeed) + 4);

		const FLOAT color[4] = { r, g, b, 1.0f };

		m_ImmediateContext->ClearRenderTargetView(m_Rtv.Get(), color);
		m_ImmediateContext->ClearDepthStencilView(
			m_DepthStencilView.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
			1.0f,
			0);

		m_SwapChain->Present(0, 0);
	}

	// Mouse Controller
	virtual void MouseDown(WPARAM btnState, int x, int y) {

	}

	virtual void MouseUp(WPARAM btnState, int x, int y) {

	}

	virtual void MouseMove(WPARAM btnState, int x, int y) {

	}

};