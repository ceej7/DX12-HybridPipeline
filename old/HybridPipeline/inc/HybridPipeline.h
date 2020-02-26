#pragma once

#include <Camera.h>
#include <Game.h>
#include <IndexBuffer.h>
#include <Light.h>
#include <Window.h>
#include <Mesh.h>
#include <RenderTarget.h>
#include <RootSignature.h>
#include <Texture.h>
#include <VertexBuffer.h>

#include <DirectXMath.h>

class HybridPipeline : public Game
{
public:
    using super = Game;

    HybridPipeline(const std::wstring& name, int width, int height, bool vSync = false);
    virtual ~HybridPipeline();

    /**
     *  Load content required for the demo.
     */
    virtual bool LoadContent() override;

    /**
     *  Unload demo specific content that was loaded in LoadContent.
     */
    virtual void UnloadContent() override;
protected:
    /**
     *  Update the game logic.
     */
    virtual void OnUpdate(UpdateEventArgs& e) override;

    /**
     *  Render stuff.
     */
    virtual void OnRender(RenderEventArgs& e) override;

    /**
     * Invoked by the registered window when a key is pressed
     * while the window has focus.
     */
    virtual void OnKeyPressed(KeyEventArgs& e) override;

    /**
     * Invoked when a key on the keyboard is released.
     */
    virtual void OnKeyReleased(KeyEventArgs& e);

    /**
     * Invoked when the mouse is moved over the registered window.
     */
    virtual void OnMouseMoved(MouseMotionEventArgs& e);

    /**
     * Invoked when the mouse wheel is scrolled while the registered window has focus.
     */
    virtual void OnMouseWheel(MouseWheelEventArgs& e) override;


    virtual void OnResize(ResizeEventArgs& e) override; 

private:
    // Some geometry to render.
    std::unique_ptr<Mesh> m_CubeMesh;
    std::unique_ptr<Mesh> m_SphereMesh;
    std::unique_ptr<Mesh> m_ConeMesh;
    std::unique_ptr<Mesh> m_TorusMesh;
    std::unique_ptr<Mesh> m_PlaneMesh;

	// Define some lights.
	std::vector<PointLight> m_PointLights;
	std::vector<SpotLight> m_SpotLights;

	// pbr texture
	//// pipeline default
	Texture m_default_albedo_texture;
	Texture m_default_metallic_texture;
	Texture m_default_normal_texture;
	Texture m_default_roughness_texture;
	//// rust iron
	Texture m_rustedIron_albedo_texture;
	Texture m_rustedIron_metallic_texture;
	Texture m_rustedIron_normal_texture;
	Texture m_rustedIron_roughness_texture;
	//// grid metal
	Texture m_gridMetal_albedo_texture;
	Texture m_gridMetal_metallic_texture;
	Texture m_gridMetal_normal_texture;
	Texture m_gridMetal_roughness_texture;
	//// metal
	Texture m_metal_albedo_texture;
	Texture m_metal_metallic_texture;
	Texture m_metal_normal_texture;
	Texture m_metal_roughness_texture;

    // Deferred Render target
    RenderTarget m_DeferredRenderTarget;

    // Root signatures
    RootSignature m_DeferredRootSignature;
    RootSignature m_PostProcessingRootSignature;

    // Pipeline state object.
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_DeferredPipelineState;
    Microsoft::WRL::ComPtr<ID3D12PipelineState> m_PostProcessingPipelineState;

    D3D12_VIEWPORT m_Viewport;
    D3D12_RECT m_ScissorRect;

    Camera m_Camera;
    struct alignas( 16 ) CameraData
    {
        DirectX::XMVECTOR m_InitialCamPos;
        DirectX::XMVECTOR m_InitialCamRot;
    };
    CameraData* m_pAlignedCameraData;

    // Camera controller
    float m_Forward;
    float m_Backward;
    float m_Left;
    float m_Right;
    float m_Up;
    float m_Down;

    float m_Pitch;
    float m_Yaw;

    // Rotate the lights in a circle.
    bool m_AnimateLights;
    // Set to true if the Shift key is pressed.
    bool m_Shift;

    int m_Width;
    int m_Height;
};