#include <windows.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <d3d11.h>
#include <d3dcompiler.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

struct FVertexSimple
{
    float x, y, z;    // Position
    float r, g, b, a; // Color
};

//Structure for a 3D vector
struct FVector3
{
    float x, y, z;
    FVector3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
};
#include "Sphere.h"

// �ﰢ���� �ϵ� �ڵ�
FVertexSimple triangle_vertices[] =
{
    {  0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top vertex (red)
    {  1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right vertex (green)
    { -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f }  // Bottom-left vertex (blue)
};

FVertexSimple cube_vertices[] =
{
    // Front face (Z+)
    { -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Bottom-left (red)
    { -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-left (yellow)
    {  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)
    { -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-left (yellow)
    {  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-right (blue)
    {  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)

    // Back face (Z-)
    { -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 1.0f }, // Bottom-left (cyan)
    {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-right (magenta)
    { -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
    { -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
    {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-right (magenta)
    {  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-right (yellow)

    // Left face (X-)
    { -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f }, // Bottom-left (purple)
    { -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
    { -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)
    { -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f }, // Top-left (blue)
    { -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f }, // Top-right (yellow)
    { -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right (green)

    // Right face (X+)
    {  0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f, 1.0f }, // Bottom-left (orange)
    {  0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f }, // Bottom-right (gray)
    {  0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 1.0f }, // Top-left (purple)
    {  0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.5f, 1.0f }, // Top-left (purple)
    {  0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f }, // Bottom-right (gray)
    {  0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.5f, 1.0f }, // Top-right (dark blue)

    // Top face (Y+)
    { -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.5f, 1.0f }, // Bottom-left (light green)
    { -0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 1.0f }, // Top-left (cyan)
    {  0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f }, // Bottom-right (white)
    { -0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 1.0f }, // Top-left (cyan)
    {  0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.0f, 1.0f }, // Top-right (brown)
    {  0.5f,  0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f }, // Bottom-right (white)

    // Bottom face (Y-)
    { -0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.0f, 1.0f }, // Bottom-left (brown)
    { -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top-left (red)
    {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f }, // Bottom-right (purple)
    { -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f }, // Top-left (red)
    {  0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f }, // Top-right (green)
    {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.5f, 1.0f }, // Bottom-right (purple)
};



class URenderer {
public:
	ID3D11Device* Device = nullptr; //GPU�� ����ϱ� ���� D3D ��ġ
	ID3D11DeviceContext* DeviceContext = nullptr; //GPU ��� ������ ����ϴ� ���ؽ�Ʈ
	IDXGISwapChain* SwapChain = nullptr; //������ ���۸� ��ü�ϴ� �� ���Ǵ� ����

	//�������� �ʿ��� ���ҽ� �� ���¸� �����ϱ� ���� ������
	ID3D11Texture2D* FrameBuffer = nullptr; //ȭ�� ��¿� �ؽ�ó
	ID3D11RenderTargetView* FrameBufferRTV = nullptr; //�ؽ�ó�� ���� Ÿ������ ����ϴ� ��
	ID3D11RasterizerState* RasterizerState = nullptr; //�����Ͷ����� ����(�ø�, ä��� ��� �� ����)
	ID3D11Buffer* ConstantBuffer = nullptr; //���̴��� �����͸� �����ϱ� ���� ��� ����
    
    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; //ȭ���� �ʱ�ȭ
    D3D11_VIEWPORT ViewportInfo; //������ ������ �����ϴ� ����Ʈ ����

    /* ���̴� ����, �Ҹ� �Լ� �߰� */
    ID3D11VertexShader* SimpleVertexShader;
    ID3D11PixelShader* SimplePixelShader;
    ID3D11InputLayout* SimpleInputLayout;
    unsigned int Stride;

public:

    // ������ �ʱ�ȭ �Լ�


    void Create(HWND hWindow)
    {
        // Direct3D ��ġ �� ���� ü�� ����
        CreateDeviceAndSwapChain(hWindow);

        // ������ ���� ����
        CreateFrameBuffer();

        // �����Ͷ����� ���� ����
        CreateRasterizerState();

        // ���� ���ٽ� ���� �� ���� ���´� �� �ڵ忡���� �ٷ��� ����
    }

    // Direct3D ��ġ �� ���� ü���� �����ϴ� �Լ�
    void CreateDeviceAndSwapChain(HWND hWindow)
    {
        // �����ϴ� Direct3D ��� ������ ����
        D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

        // ���� ü�� ���� ����ü �ʱ�ȭ
        DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
        swapchaindesc.BufferDesc.Width = 0; // â ũ�⿡ �°� �ڵ����� ����
        swapchaindesc.BufferDesc.Height = 0; // â ũ�⿡ �°� �ڵ����� ����
        swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ���� ����
        swapchaindesc.SampleDesc.Count = 1; // ��Ƽ ���ø� ��Ȱ��ȭ
        swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���� Ÿ������ ���
        swapchaindesc.BufferCount = 2; // ���� ���۸�
        swapchaindesc.OutputWindow = hWindow; // �������� â �ڵ�
        swapchaindesc.Windowed = TRUE; // â ���
        swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // ���� ���

        // Direct3D ��ġ�� ���� ü���� ����
        D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
            featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
            &swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

        // ������ ���� ü���� ���� ��������
        SwapChain->GetDesc(&swapchaindesc);

        // ����Ʈ ���� ����
        ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
    }

    // Direct3D ��ġ �� ���� ü���� �����ϴ� �Լ�
    void ReleaseDeviceAndSwapChain()
    {
        if (DeviceContext)
        {
            DeviceContext->Flush(); // �����ִ� GPU ��� ����
        }

        if (SwapChain)
        {
            SwapChain->Release();
            SwapChain = nullptr;
        }

        if (Device)
        {
            Device->Release();
            Device = nullptr;
        }

        if (DeviceContext)
        {
            DeviceContext->Release();
            DeviceContext = nullptr;
        }
    }

    // ������ ���۸� �����ϴ� �Լ�
    void CreateFrameBuffer()
    {
        // ���� ü�����κ��� �� ���� �ؽ�ó ��������
        SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

        // ���� Ÿ�� �� ����
        D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
        framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // ���� ����
        framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D �ؽ�ó

        Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
    }

    // ������ ���۸� �����ϴ� �Լ�
    void ReleaseFrameBuffer()
    {
        if (FrameBuffer)
        {
            FrameBuffer->Release();
            FrameBuffer = nullptr;
        }

        if (FrameBufferRTV)
        {
            FrameBufferRTV->Release();
            FrameBufferRTV = nullptr;
        }
    }

    // �����Ͷ����� ���¸� �����ϴ� �Լ�
    /*
    �����Ͷ������� 3D ���� ���� �����͸� 2D ȭ�鿡 �׸� �� �ִ� �ȼ� �����ͷ� ��ȯ�ϴ� ����.
    ���̴� �ܰ踦 ��ģ �� �ȼ� ������ ���� �ܰ� ���� ��ġ.
    */
    void CreateRasterizerState()
    {
        D3D11_RASTERIZER_DESC rasterizerdesc = {};
        rasterizerdesc.FillMode = D3D11_FILL_SOLID; // ä��� ���
        rasterizerdesc.CullMode = D3D11_CULL_BACK; // �� ���̽� �ø�

        Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
    }

    /*
    
    �����Ͷ����� :
    3D �׷��Ƚ� ���������ο��� 
    3D ���� �������� ������ -> 2D ȭ���� �ȼ� 
    ��ȯ�Ͽ� ���� �̹����� �����ϴ� ������ ����ϴ� ������Ʈ

    */
    // �����Ͷ����� ���¸� �����ϴ� �Լ�
    void ReleaseRasterizerState()
    {
        if (RasterizerState)
        {
            RasterizerState->Release();
            RasterizerState = nullptr;
        }
    }

    // �������� ���� ��� ���ҽ��� �����ϴ� �Լ�
    void Release()
    {
        RasterizerState->Release();

        // ���� Ÿ���� �ʱ�ȭ
        DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

        ReleaseFrameBuffer();
        ReleaseDeviceAndSwapChain();
    }

    // ���� ü���� �� ���ۿ� ����Ʈ ���۸� ��ü�Ͽ� ȭ�鿡 ���
    void SwapBuffer()
    {
        SwapChain->Present(1, 0); // 1: VSync Ȱ��ȭ
    }

    void CreateShader()
    {
        ID3DBlob* vertexshaderCSO;
        ID3DBlob* pixelshaderCSO;

        D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainVS", "vs_5_0", 0, 0, &vertexshaderCSO, nullptr);

        Device->CreateVertexShader(vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), nullptr, &SimpleVertexShader);

        D3DCompileFromFile(L"ShaderW0.hlsl", nullptr, nullptr, "mainPS", "ps_5_0", 0, 0, &pixelshaderCSO, nullptr);

        Device->CreatePixelShader(pixelshaderCSO->GetBufferPointer(), pixelshaderCSO->GetBufferSize(), nullptr, &SimplePixelShader);

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        Device->CreateInputLayout(layout, ARRAYSIZE(layout), vertexshaderCSO->GetBufferPointer(), vertexshaderCSO->GetBufferSize(), &SimpleInputLayout);

        Stride = sizeof(FVertexSimple);

        vertexshaderCSO->Release();
        pixelshaderCSO->Release();
    }

    void ReleaseShader()
    {
        if (SimpleInputLayout)
        {
            SimpleInputLayout->Release();
            SimpleInputLayout = nullptr;
        }

        if (SimplePixelShader)
        {
            SimplePixelShader->Release();
            SimplePixelShader = nullptr;
        }

        if (SimpleVertexShader)
        {
            SimpleVertexShader->Release();
            SimpleVertexShader = nullptr;
        }
    }

    /*
    - ������ �غ� �۾� �� ������ �Լ� �߰�
    
    D3D11 �������� �ʿ��� �غ� �۾��� ���� Prepare �Լ�
    
    Simple Shader ����� ���� PrepareShader �Լ�
    
    �������� Rendering ��û�� �� RenderPrimitive �Լ�
    */
    void Prepare()
    {
        DeviceContext->ClearRenderTargetView(FrameBufferRTV, ClearColor);

        DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        DeviceContext->RSSetViewports(1, &ViewportInfo);
        DeviceContext->RSSetState(RasterizerState);

        DeviceContext->OMSetRenderTargets(1, &FrameBufferRTV, nullptr);
        DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
    }

    void PrepareShader()
    {
        DeviceContext->VSSetShader(SimpleVertexShader, nullptr, 0);
        DeviceContext->PSSetShader(SimplePixelShader, nullptr, 0);
        DeviceContext->IASetInputLayout(SimpleInputLayout);

        // ���ؽ� ���̴��� ��� ���۸� �����մϴ�.
        if (ConstantBuffer)
        {
            DeviceContext->VSSetConstantBuffers(0, 1, &ConstantBuffer);
        }
    }

    void RenderPrimitive(ID3D11Buffer* pBuffer, UINT numVertices)
    {
        UINT offset = 0;
        DeviceContext->IASetVertexBuffers(0, 1, &pBuffer, &Stride, &offset);

        DeviceContext->Draw(numVertices, 0);
    }

    ID3D11Buffer* CreateVertexBuffer(FVertexSimple* vertices, UINT byteWidth)
    {
        // 2. Create a vertex buffer
        D3D11_BUFFER_DESC vertexbufferdesc = {};
        vertexbufferdesc.ByteWidth = byteWidth;
        vertexbufferdesc.Usage = D3D11_USAGE_IMMUTABLE; // will never be updated 
        vertexbufferdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexbufferSRD = { vertices };

        ID3D11Buffer* vertexBuffer;

        Device->CreateBuffer(&vertexbufferdesc, &vertexbufferSRD, &vertexBuffer);

        return vertexBuffer;
    }

    void ReleaseVertexBuffer(ID3D11Buffer* vertexBuffer)
    {
        vertexBuffer->Release();
    }

    //��� ���� �߰�

    struct FConstants
    {
        FVector3 Offset; // 12����Ʈ 
        float Pad; //16����Ʈ ���� �ʿ��ؼ� 4����Ʈ float �е� �߰�
    };


    void CreateConstantBuffer()
    {
        D3D11_BUFFER_DESC constantbufferdesc = {};
        constantbufferdesc.ByteWidth = sizeof(FConstants) + 0xf & 0xfffffff0; // ensure constant buffer size is multiple of 16 bytes
        constantbufferdesc.Usage = D3D11_USAGE_DYNAMIC; // will be updated from CPU every frame
        constantbufferdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        constantbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

        Device->CreateBuffer(&constantbufferdesc, nullptr, &ConstantBuffer);
    }

    void ReleaseConstantBuffer()
    {
        if (ConstantBuffer)
        {
            ConstantBuffer->Release();
            ConstantBuffer = nullptr;
        }
    }

    void UpdateConstant(FVector3 Offset)
    {
        if (ConstantBuffer)
        {
            D3D11_MAPPED_SUBRESOURCE constantbufferMSR;

            DeviceContext->Map(ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &constantbufferMSR); // update constant buffer every frame
            FConstants* constants = (FConstants*)constantbufferMSR.pData;
            {
                constants->Offset = Offset;
            }
            DeviceContext->Unmap(ConstantBuffer, 0);
        }
    }
};


//WndProcHandler ����
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//���� �޼��� ó�� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    {
        return true;
    }
    switch (message) 
	{
	case WM_DESTROY:
		//signal that the app should quit.
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WCHAR WindowClass[] = L"JungleWindowClass"; // �׳� Ŭ���� �̸��ε�?

	WCHAR Title[] = L"Game Tech Lab"; //â ���� Ÿ��Ʋ ��

	//���� �޼����� ó���� �Լ��� WndProc�� �Լ� �����͸� WindowClass ����ü�� �ִ´�.
	WNDCLASSW wndclass = { 0, WndProc, 0,0,0,0,0,0,0, WindowClass };

	//������ Ŭ���� ���
	RegisterClassW(&wndclass);

	// 1024 x 1024 ũ�⿡ ������ ����
	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024, nullptr, nullptr, hInstance, nullptr);

    URenderer renderer;

    renderer.Create(hWnd);
    renderer.CreateShader();  // ������ ���� ���Ŀ� ���̴��� �����ϴ� �Լ��� ȣ���մϴ�.
    // ������� ����
    renderer.CreateConstantBuffer();
    //������ ���� �� ImGui ���� �� �ʱ�ȭ �Լ� ȣ��
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init((void*)hWnd);
    ImGui_ImplDX11_Init(renderer.Device, renderer.DeviceContext);


    UINT numVerticesTriangle = sizeof(triangle_vertices) / sizeof(FVertexSimple);
    UINT numVerticesCube = sizeof(cube_vertices) / sizeof(FVertexSimple);
    UINT numVerticesSphere = sizeof(sphere_vertices) / sizeof(FVertexSimple);

    float scaleMod = 0.1f;

    for (UINT i = 0; i < numVerticesSphere; ++i)
    {
        sphere_vertices[i].x *= scaleMod;
        sphere_vertices[i].y *= scaleMod;
        sphere_vertices[i].z *= scaleMod;
    }

    ID3D11Buffer* vertexBufferTriangle = renderer.CreateVertexBuffer(triangle_vertices, sizeof(triangle_vertices));
    ID3D11Buffer* vertexBufferCube = renderer.CreateVertexBuffer(cube_vertices, sizeof(cube_vertices));
    ID3D11Buffer* vertexBufferSphere = renderer.CreateVertexBuffer(sphere_vertices, sizeof(sphere_vertices));


	bool bIsExit = false;

	//���� �����ϴ� �ڵ带 ���⿡ �߰��մϴ�.
    enum ETypePrimitive
    {
        EPT_Triangle,
        EPT_Cube,
        EPT_Sphere,
        EPT_Max,
    };

    ETypePrimitive typePrimitive = EPT_Triangle;
    
    FVector3 offset(0.0f); // ������ ������ ������ ���� offset ����
    FVector3 velocity(0.0f); // ������ ������ ������ ���� velocity ����

    const float leftBorder = -1.0f;
    const float rightBorder = 1.0f;
    const float topBorder = -1.0f;
    const float bottomBorder = 1.0f;
    const float sphereRadius= 1.0f;

    bool bBoundBallToScreen = true;
    bool bPinballMovement = true;

    //�ʱ�ӵ� define

    const float ballSpeed = 0.004f;
    velocity.x = ((float)(rand() % 100 - 50)) * ballSpeed;
    velocity.y = ((float)(rand() % 100 - 50)) * ballSpeed;//0.001f�� ������ �ʱ�ӵ� ����

    const int targetFPS = 60;
    const double targetFrameTime = 1000.0 / targetFPS;

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER startTime, endTime;
    double elapsedTime = 0.0;

	//Main Loop (Quit Message�� ������ ������ �Ʒ� Loop�� ������ ����)
	while (bIsExit == false) {
        QueryPerformanceCounter(&startTime);
        
        MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

			// Ű �Է� �޼����� ����
			TranslateMessage(&msg);
			//�޼����� ������ ������ ���ν����� ����, �޼����� ������ ����� WndProc���� ����
			DispatchMessage(&msg);
			
			if (msg.message == WM_QUIT) {
				bIsExit = true;
				break;
			}
            /* PeekMessage while�� �ۿ��� ���� �Է� ó��*/
            //else if (!bPinballMovement && msg.message == WM_KEYDOWN)
            //{
            //    // ���� Ű�� ����Ű�� �ش� ���⿡ ���缭
            //    // offset ������ x, y ��� ������ ���� ����
            //    if (msg.wParam == VK_LEFT)
            //    {
            //        offset.x -= 0.01f;
            //    }
            //    if (msg.wParam == VK_RIGHT)
            //    {
            //        offset.x += 0.01f;
            //    }
            //    if (msg.wParam == VK_UP)
            //    {
            //        offset.y += 0.01f;
            //    }
            //    if (msg.wParam == VK_DOWN)
            //    {
            //        offset.y -= 0.01f;
            //    }
            //}
            if (bPinballMovement)
            {
                offset.x += velocity.x;
                offset.y += velocity.y;
                offset.z += velocity.z;

                float renderRadius = sphereRadius * scaleMod;
                if (offset.x < leftBorder + renderRadius) velocity.x *= -1.0f;
                if (offset.x > rightBorder - renderRadius) velocity.x *= -1.0f;
                if (offset.y < topBorder + renderRadius) velocity.y *= -1.0f;
                if (offset.y > bottomBorder - renderRadius) velocity.y *= -1.0f;
            }
            else {
                // �޽��� ó�� �� Ű ���� ����
                if (!bPinballMovement)
                {
                    // ���� Ű�� �����ִ��� Ȯ��
                    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
                    {
                        offset.x -= 0.01f;
                    }
                    // ������ Ű�� �����ִ��� Ȯ��
                    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
                    {
                        offset.x += 0.01f;
                    }
                    // ���� Ű�� �����ִ��� Ȯ��
                    if (GetAsyncKeyState(VK_UP) & 0x8000)
                    {
                        offset.y += 0.01f;
                    }
                    // �Ʒ��� Ű�� �����ִ��� Ȯ��
                    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
                    {
                        offset.y -= 0.01f;
                    }
                }

            }
            //ȭ�� ������ ������ �ʰ�
            if (bBoundBallToScreen)
            {
                float renderRadius = sphereRadius * scaleMod;
                if (offset.x < leftBorder + renderRadius) offset.x = leftBorder + renderRadius;
                if (offset.x > rightBorder - renderRadius) offset.x = rightBorder - renderRadius;
                if (offset.y < topBorder + renderRadius) offset.y = topBorder + renderRadius;
                if (offset.y > bottomBorder - renderRadius) offset.y = bottomBorder - renderRadius;
            }
		}
        // �غ� �۾�
        renderer.Prepare();
        renderer.PrepareShader();

        // ������ ���ؽ� ���۸� �Ѱ� �������� ������ ��û

        // offset�� ��� ���۷� ������Ʈ
        renderer.UpdateConstant(offset);
        typePrimitive = EPT_Sphere;

        switch (typePrimitive)
        {
        case EPT_Triangle:
            renderer.RenderPrimitive(vertexBufferTriangle, numVerticesTriangle);
            break;
        case EPT_Cube:
            renderer.RenderPrimitive(vertexBufferCube, numVerticesCube);
            break;
        case EPT_Sphere:
            renderer.RenderPrimitive(vertexBufferSphere, numVerticesSphere);
            break;
        }

        //ImGui ������ �غ�, ��Ʈ�� ����
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // ���� ImGui UI ��Ʈ�� �߰��� ImGui::NewFrame()�� ImGui::Render() ������ ���⿡ ��ġ�մϴ�.
		ImGui::Begin("Jungle Property Window");

        ImGui::Text("Hello Jungle World!");

        ImGui::Checkbox("Bound Ball To Screen", &bBoundBallToScreen);
        ImGui::Checkbox("Pinball Movement", &bPinballMovement);
        //if (ImGui::Button("Quit this app"))
        //{
        //    // ���� �����쿡 Quit �޽����� �޽��� ť�� ����
        //    PostMessage(hWnd, WM_QUIT, 0, 0);
        //}

        // Change primitive ��ư
        /*if (ImGui::Button("Change primitive"))
        {
            switch (typePrimitive)
            {
            case EPT_Triangle:
                typePrimitive = EPT_Cube;
                break;
            case EPT_Cube:
                typePrimitive = EPT_Sphere;
                break;
            case EPT_Sphere:
                typePrimitive = EPT_Triangle;
                break;
            }
        }*/

		ImGui::End();

        //������ ��û �Լ�?
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


        // �� �׷����� ���۸� ��ȯ
        renderer.SwapBuffer();

        do
        {
            Sleep(0);

            QueryPerformanceCounter(&endTime);

            elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;


        } while (elapsedTime < targetFrameTime);
	}

    //ImGui �Ҹ� �Լ��� ȣ��
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // ���ؽ� ���� �Ҹ��� Renderer �Ҹ����� ó���մϴ�.
    //vertexBuffer->Release();

    renderer.ReleaseVertexBuffer(vertexBufferTriangle);
    renderer.ReleaseVertexBuffer(vertexBufferCube);
    renderer.ReleaseVertexBuffer(vertexBufferSphere);

    // ������ �Ҹ�
    renderer.ReleaseConstantBuffer(); // ������� �Ҹ�
    renderer.ReleaseShader(); //������ �Ҹ� ������ ���̴��� �Ҹ� ��Ű�� �Լ��� ȣ��
    renderer.Release();
	return 0;

}