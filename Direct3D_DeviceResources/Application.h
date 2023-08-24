//
// Application.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "Mesh.h"
#include "Grid.h"
#include "Camera.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Application final : public DX::IDeviceNotify
{
public:

    Application() noexcept(false);
    ~Application() = default;

    Application(Application&&) = default;
    Application& operator= (Application&&) = default;

    Application(Application const&) = delete;
    Application& operator= (Application const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);
    void OnWindowSizeChangedContinous(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void ProcessMouseInput();
    void ProcessKeyboardInput();

    void Render();
    void RenderGUI();
    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>        m_deviceResources;
    Camera                                      m_camera;
    Matrix                                      m_viewMatrix;
    Matrix                                      m_projectionMatrix;

    std::unique_ptr<DirectX::Keyboard>          m_keyboard;
    std::unique_ptr<DirectX::Mouse>             m_mouse;
    DirectX::Mouse::State                       m_mouseState;

    std::vector<Mesh*>                          m_meshes;
    Grid*                                       m_grid;
    XMFLOAT4                                    m_clearColor    =   { 0.275f, 0.392f, 0.47f, 1.00f };
    XMFLOAT4                                    m_selectedColor =   { 0.67f, 0.48f, 0.49f, 1.00f };

    // Rendering loop timer.
    DX::StepTimer                               m_timer;
};
