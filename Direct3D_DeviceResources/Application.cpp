//
// Application.cpp
//

#include "pch.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx11.h"
#include "imgui\imgui_impl_win32.h"
#include "imgui\imgui_stdlib.h"
#include "Application.h"
#include "Point.h"
#include "Torus.h"
#include "Sphere.h"
#include "VertexTypes.h"
#include "MatrixTransformations.h"


extern void ExitGame() noexcept;

using Microsoft::WRL::ComPtr;
constexpr float ROTATION_GAIN = 0.004f;
constexpr float SCALE_GAIN = 0.05f;
constexpr float PAN_GAIN = 0.05f;
constexpr float MOVEMENT_GAIN = 0.05f;

Application::Application() noexcept(false) : 
    m_camera({ 0.f, 1.5f, -5.f }, -0.29145679f, 0)
{
    srand(static_cast<unsigned int>(time(NULL)));
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Application::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowRounding = 10.0f;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext());
    
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);
    m_mouseState = m_mouse->GetState();

    m_grid = new Grid(m_deviceResources->GetD3DDevice(), GeometryType::Triangle);
}

#pragma region Frame Update
// Executes the basic game loop.
void Application::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Application::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    ProcessMouseInput();
    ProcessKeyboardInput();
}

void Application::ProcessMouseInput()
{
    auto mouse = m_mouse->GetState();
    if (mouse.positionMode == Mouse::MODE_RELATIVE)
    {
        if (mouse.rightButton) // Rotate camera
        {
            m_camera.Rotate(ROTATION_GAIN * mouse.y, ROTATION_GAIN * mouse.x);
        }
        else if (mouse.middleButton) // Pan camera
        {
            Vector3 move = (static_cast<float>(mouse.x) * m_camera.GetRight() - static_cast<float>(mouse.y) * m_camera.GetUp()) * PAN_GAIN;
            m_camera.Move(move);
        }

    }
    if (mouse.scrollWheelValue != m_mouseState.scrollWheelValue) // Zoom camera
    {
        m_camera.Move(m_camera.GetForward() * static_cast<float>(mouse.scrollWheelValue - m_mouseState.scrollWheelValue) * SCALE_GAIN);
    }
    m_mouseState = mouse;
    m_mouse->SetMode(mouse.rightButton || mouse.middleButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
}
void Application::ProcessKeyboardInput()
{
    auto kb = m_keyboard->GetState();
    if (kb.Escape) ExitGame();
    if (kb.W) m_camera.Move(m_camera.GetForward() * MOVEMENT_GAIN);
    if (kb.S) m_camera.Move(-m_camera.GetForward() * MOVEMENT_GAIN);
    if (kb.A) m_camera.Move(-m_camera.GetRight() * MOVEMENT_GAIN);
    if (kb.D) m_camera.Move(m_camera.GetRight() * MOVEMENT_GAIN);
    if (kb.Q) m_camera.Move(m_camera.GetUp() * MOVEMENT_GAIN);
    if (kb.E) m_camera.Move(-m_camera.GetUp() * MOVEMENT_GAIN);
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Application::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();
    
    // Render code

    // Check if View Matrix needs update
    if (m_camera.ViewMatrixNeedsUpdate())
    {
        m_viewMatrix = MatrixTransformations::CreateViewMatrix(m_camera.GetPosition(), m_camera.GetForward(), Vector3::UnitY);
        auto inverseView = m_viewMatrix.Invert();
        std::vector<DirectX::XMMATRIX> matrices = { m_viewMatrix, inverseView };
        m_deviceResources->UpdateViewMatrixCB(m_camera.GetPosition(), m_viewMatrix, inverseView);
        m_camera.ViewMatrixUpdated();
    }

    //context->OMSetDepthStencilState( m_deviceResources->GetDepthStencilState(), 0);
    m_grid->Render(context, m_selectedColor);
    for (auto& mesh : m_meshes)
        mesh->Render(context, m_selectedColor);

    RenderGUI();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

void Application::RenderGUI()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Render menu bar
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Add..."))
        {
            if (ImGui::MenuItem("Point", "CTRL+P"))
            {
                for (int counter = 0; counter < 50; counter++)
                {
                    float x = 1.0f * rand() / RAND_MAX;
                    float y = 1.0f * rand() / RAND_MAX;
                    float z = 1.0f * rand() / RAND_MAX;
                    m_meshes.push_back(
                        new Point(m_deviceResources->GetD3DDevice(),
                            Vector3{ x,y,z }, { x,y,z, 1.0f }));
                }
            }
            if (ImGui::MenuItem("Torus", "CTRL+T"))
            {
                int n = rand() % 18 + 3;
                int m = rand() % 18 + 3;
                float r = 0.5f * (rand() % 9 + 1);
                float R = 1.0f * (rand() % 9 + 2);
                m_meshes.push_back(new Torus(m_deviceResources->GetD3DDevice(), GeometryType::Quad, n, m, r, R));
            }
            if (ImGui::MenuItem("Sphere", "CTRL+S"))
            {
                int n = rand() % 18 + 3;
                int m = rand() % 18 + 3;
                float r = 0.5f * (rand() % 9 + 1);
                m_meshes.push_back(new Sphere(m_deviceResources->GetD3DDevice(), GeometryType::Quad, n, m, r));
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Help", "CTRL+H"))
            {
                m_helpWindowOpened = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (m_helpWindowOpened)
    {
        ImGui::Begin("Help", &m_helpWindowOpened);
        ImGui::Text("Controls:");
        ImGui::Text("W/S/A/D - Move camera forward/backward/left/right");
        ImGui::Text("Q/E - Move camera up/down");
        ImGui::Text("Mouse RMB - Rotate camera");
        ImGui::Text("Mouse MMB - Pan camera");
        ImGui::End();
    }

    // Application Settings window
    ImGui::Begin("Application Settings");
    ImGui::Checkbox("Hide grid", &m_grid->IsHidden);
    ImGui::Text("Grid type");
    if (ImGui::RadioButton("Tri", &m_grid->MeshType, GeometryType::Triangle))
    {
        m_grid->RecalculateGeometry(m_deviceResources->GetD3DDevice());
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Quad", &m_grid->MeshType, GeometryType::Quad))
    {
        m_grid->RecalculateGeometry(m_deviceResources->GetD3DDevice());
    }
    ImGui::ColorEdit3("Clear color", (float*)&m_clearColor);
    ImGui::ColorEdit3("Selected color", (float*)&m_selectedColor);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    // Render selected object Settings window
    auto selectedObjectIt = std::find_if(m_meshes.begin(), m_meshes.end(), [](auto& o) { return o->IsSelected; });
    if (selectedObjectIt != m_meshes.end())
    {
        ImGui::Begin("Selected Object Settings");
        ImGui::Checkbox("Hide object", &(*selectedObjectIt)->IsHidden);
        if (ImGui::SliderFloat3("Position", (float*)(*selectedObjectIt)->GetPosition(), -20.0f, 20.0f))
            (*selectedObjectIt)->RecalculateWorldMatrix();
        if (ImGui::SliderFloat3("Rotation", (float*)(*selectedObjectIt)->GetRotation(), -XM_PI, XM_PI))
            (*selectedObjectIt)->RecalculateWorldMatrix();
        if (ImGui::SliderFloat3("Scale", (float*)(*selectedObjectIt)->GetScale(), -20.0f, 20.0f))
            (*selectedObjectIt)->RecalculateWorldMatrix();
        ImGui::ColorEdit3("Color", (float*)&(*selectedObjectIt)->Color);
        Torus* torus = dynamic_cast<Torus*>(*selectedObjectIt);
        if (torus != nullptr)
        {
            if (ImGui::SliderInt("Torus division N", &torus->GetDivisionN(), 3, 100))
            {
                torus->RecalculateGeometry(m_deviceResources->GetD3DDevice());
            }
            if (ImGui::SliderInt("Torus division M", &torus->GetDivisionM(), 3, 100))
            {
                torus->RecalculateGeometry(m_deviceResources->GetD3DDevice());
            }
            if (ImGui::SliderFloat("Minor radius", &torus->GetMinorRadius(), 0.1f, 50.0f))
            {
                torus->RecalculateGeometry(m_deviceResources->GetD3DDevice());
            }
            if (ImGui::SliderFloat("Major radius", &torus->GetMajorRadius(), 0.1f, 50.0f))
            {
                torus->RecalculateGeometry(m_deviceResources->GetD3DDevice());
            }
        }
        Sphere* sphere = dynamic_cast<Sphere*>(*selectedObjectIt);
        if (sphere != nullptr)
        {
            if (ImGui::SliderInt("Sphere division N", &sphere->GetDivisionN(), 3, 100))
            {
                sphere->RecalculateGeometry(m_deviceResources->GetD3DDevice());
            }
            if (ImGui::SliderInt("Sphere division M", &sphere->GetDivisionM(), 3, 100))
            {
                sphere->RecalculateGeometry(m_deviceResources->GetD3DDevice());
            }
            if (ImGui::SliderFloat("Sphere radius", &sphere->GetRadius(), 0.1f, 50.0f))
            {
                sphere->RecalculateGeometry(m_deviceResources->GetD3DDevice());
            }
        }
        ImGui::End();
    }

    // Render SceneObjects window
    ImGui::Begin("Scene Objects");
    static char editName[32] = "";
    bool eraseObjects = false;
    if (ImGui::BeginListBox("ListBox", ImVec2(-1, 200)))
    {
        for (int i = 0; i < m_meshes.size(); ++i)
        {
            ImGui::PushID(i);
            if (!m_meshes[i]->IsRenaming)
            {
                if (ImGui::Selectable(m_meshes[i]->Name.c_str(), m_meshes[i]->IsSelected))
                {
                    if (!ImGui::GetIO().KeyCtrl)
                        std::for_each(m_meshes.begin(), m_meshes.end(), [](auto const& o) {o->IsSelected = false; });
                    m_meshes[i]->IsSelected = !m_meshes[i]->IsSelected;
                }
            }
            else // IsRenaming == true
            {
                if (ImGui::InputText("", &m_meshes[i]->Name, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    m_meshes[i]->IsRenaming = false;
                }
            }
            if(ImGui::BeginPopupContextItem("ObjectContextMenu"))
            {
                // If we open context menu on scene object that is not selected on the list, the the selection should automatically 
                // switch to the item which was context menu called on
                if (!m_meshes[i]->IsSelected)
                {
                    std::for_each(m_meshes.begin(), m_meshes.end(), [](auto const& o) { o->IsSelected = false; });
                    m_meshes[i]->IsSelected = true;
                }
                // If there is more than 1 item on the Scene Objects' list selected, then renaming funtionality is not available 
                // (as renaming more than 1 item in the same time is never easy to work out for user) 
                bool renameEnabled = std::count_if(m_meshes.begin(), m_meshes.end(), [](auto& o) { return o->IsSelected; }) == 1;
                if (ImGui::MenuItem("Rename", "", false, renameEnabled))
                {
                    m_meshes[i]->IsRenaming = true;
                }
                if (ImGui::MenuItem("Delete"))
                {
                    eraseObjects = true;
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        ImGui::EndListBox();
    }

    if (eraseObjects)
        m_meshes.erase(std::remove_if(m_meshes.begin(), m_meshes.end(), [](auto& o) { return o->IsSelected; }), m_meshes.end());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

// Helper method to clear the back buffers.
void Application::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, (float*)&m_clearColor);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Application::OnActivated()
{
}

void Application::OnDeactivated()
{
}

void Application::OnSuspending()
{
}

void Application::OnResuming()
{
    m_timer.ResetElapsedTime();
}

void Application::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Application::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Application::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();
}

void Application::OnWindowSizeChangedContinous(int width, int height)
{
    m_deviceResources->UpdateConstantBuffer(width, height);
}

// Properties
void Application::GetDefaultSize(int& width, int& height) const noexcept
{
    width = 1400;
    height = 700;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Application::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

    m_meshes.push_back(new Point(device, Vector3{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }));
    m_meshes.push_back(new Point(device, Vector3{ 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }));
    m_meshes.push_back(new Point(device, Vector3{ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }));
    m_meshes.push_back(new Point(device, Vector3{ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }));
    m_meshes.push_back(new Torus(device, GeometryType::Quad));
    m_meshes.push_back(new Sphere(device, GeometryType::Triangle, 32, 32, 2.0f));
}

// Allocate all memory resources that change on a window SizeChanged event.
void Application::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();

    m_deviceResources->UpdateConstantBuffer(size.right, size.bottom);
}

void Application::OnDeviceLost()
{
}

void Application::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
