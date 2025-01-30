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

// 삼각형을 하드 코딩
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
	ID3D11Device* Device = nullptr; //GPU와 통신하기 위한 D3D 장치
	ID3D11DeviceContext* DeviceContext = nullptr; //GPU 명령 실행을 담당하는 컨텍스트
	IDXGISwapChain* SwapChain = nullptr; //프레임 버퍼를 교체하는 데 사용되는 스왑

	//렌더링에 필요한 리소스 및 상태를 관리하기 위한 변수들
	ID3D11Texture2D* FrameBuffer = nullptr; //화면 출력용 텍스처
	ID3D11RenderTargetView* FrameBufferRTV = nullptr; //텍스처를 렌더 타겟으로 사용하는 뷰
	ID3D11RasterizerState* RasterizerState = nullptr; //래스터라이저 상태(컬링, 채우기 모드 등 정의)
	ID3D11Buffer* ConstantBuffer = nullptr; //쉐이더에 데이터를 전달하기 위한 상수 버퍼
    
    FLOAT ClearColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f }; //화면을 초기화
    D3D11_VIEWPORT ViewportInfo; //렌더링 영역을 정의하는 뷰포트 정보

    /* 쉐이더 생성, 소멸 함수 추가 */
    ID3D11VertexShader* SimpleVertexShader;
    ID3D11PixelShader* SimplePixelShader;
    ID3D11InputLayout* SimpleInputLayout;
    unsigned int Stride;

public:

    // 렌더러 초기화 함수


    void Create(HWND hWindow)
    {
        // Direct3D 장치 및 스왑 체인 생성
        CreateDeviceAndSwapChain(hWindow);

        // 프레임 버퍼 생성
        CreateFrameBuffer();

        // 래스터라이저 상태 생성
        CreateRasterizerState();

        // 깊이 스텐실 버퍼 및 블렌드 상태는 이 코드에서는 다루지 않음
    }

    // Direct3D 장치 및 스왑 체인을 생성하는 함수
    void CreateDeviceAndSwapChain(HWND hWindow)
    {
        // 지원하는 Direct3D 기능 레벨을 정의
        D3D_FEATURE_LEVEL featurelevels[] = { D3D_FEATURE_LEVEL_11_0 };

        // 스왑 체인 설정 구조체 초기화
        DXGI_SWAP_CHAIN_DESC swapchaindesc = {};
        swapchaindesc.BufferDesc.Width = 0; // 창 크기에 맞게 자동으로 설정
        swapchaindesc.BufferDesc.Height = 0; // 창 크기에 맞게 자동으로 설정
        swapchaindesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // 색상 포맷
        swapchaindesc.SampleDesc.Count = 1; // 멀티 샘플링 비활성화
        swapchaindesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 렌더 타겟으로 사용
        swapchaindesc.BufferCount = 2; // 더블 버퍼링
        swapchaindesc.OutputWindow = hWindow; // 렌더링할 창 핸들
        swapchaindesc.Windowed = TRUE; // 창 모드
        swapchaindesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 스왑 방식

        // Direct3D 장치와 스왑 체인을 생성
        D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
            featurelevels, ARRAYSIZE(featurelevels), D3D11_SDK_VERSION,
            &swapchaindesc, &SwapChain, &Device, nullptr, &DeviceContext);

        // 생성된 스왑 체인의 정보 가져오기
        SwapChain->GetDesc(&swapchaindesc);

        // 뷰포트 정보 설정
        ViewportInfo = { 0.0f, 0.0f, (float)swapchaindesc.BufferDesc.Width, (float)swapchaindesc.BufferDesc.Height, 0.0f, 1.0f };
    }

    // Direct3D 장치 및 스왑 체인을 해제하는 함수
    void ReleaseDeviceAndSwapChain()
    {
        if (DeviceContext)
        {
            DeviceContext->Flush(); // 남아있는 GPU 명령 실행
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

    // 프레임 버퍼를 생성하는 함수
    void CreateFrameBuffer()
    {
        // 스왑 체인으로부터 백 버퍼 텍스처 가져오기
        SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&FrameBuffer);

        // 렌더 타겟 뷰 생성
        D3D11_RENDER_TARGET_VIEW_DESC framebufferRTVdesc = {};
        framebufferRTVdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB; // 색상 포맷
        framebufferRTVdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // 2D 텍스처

        Device->CreateRenderTargetView(FrameBuffer, &framebufferRTVdesc, &FrameBufferRTV);
    }

    // 프레임 버퍼를 해제하는 함수
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

    // 래스터라이저 상태를 생성하는 함수
    /*
    래스터라이저는 3D 공간 도형 데이터를 2D 화면에 그릴 수 있는 픽셀 데이터로 변환하는 과정.
    쉐이더 단계를 거친 후 픽셀 데이터 생성 단계 전에 위치.
    */
    void CreateRasterizerState()
    {
        D3D11_RASTERIZER_DESC rasterizerdesc = {};
        rasterizerdesc.FillMode = D3D11_FILL_SOLID; // 채우기 모드
        rasterizerdesc.CullMode = D3D11_CULL_BACK; // 백 페이스 컬링

        Device->CreateRasterizerState(&rasterizerdesc, &RasterizerState);
    }

    /*
    
    래스터라이저 :
    3D 그래픽스 파이프라인에서 
    3D 모델의 기하학적 데이터 -> 2D 화면의 픽셀 
    변환하여 최종 이미지를 생성하는 과정을 담당하는 컴포넌트

    */
    // 래스터라이저 상태를 해제하는 함수
    void ReleaseRasterizerState()
    {
        if (RasterizerState)
        {
            RasterizerState->Release();
            RasterizerState = nullptr;
        }
    }

    // 렌더러에 사용된 모든 리소스를 해제하는 함수
    void Release()
    {
        RasterizerState->Release();

        // 렌더 타겟을 초기화
        DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

        ReleaseFrameBuffer();
        ReleaseDeviceAndSwapChain();
    }

    // 스왑 체인의 백 버퍼와 프론트 버퍼를 교체하여 화면에 출력
    void SwapBuffer()
    {
        SwapChain->Present(1, 0); // 1: VSync 활성화
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
    - 렌더링 준비 작업 및 렌더링 함수 추가
    
    D3D11 렌더링에 필요한 준비 작업을 위한 Prepare 함수
    
    Simple Shader 사용을 위한 PrepareShader 함수
    
    실질적인 Rendering 요청을 할 RenderPrimitive 함수
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

        // 버텍스 쉐이더에 상수 버퍼를 설정합니다.
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

    //상수 버퍼 추가

    struct FConstants
    {
        FVector3 Offset; // 12바이트 
        float Pad; //16바이트 정렬 필요해서 4바이트 float 패딩 추가
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


//WndProcHandler 선언
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//각종 메세지 처리 함수
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
	WCHAR WindowClass[] = L"JungleWindowClass"; // 그냥 클래스 이름인듯?

	WCHAR Title[] = L"Game Tech Lab"; //창 위에 타이틀 뜸

	//각종 메세지를 처리할 함수인 WndProc의 함수 포인터를 WindowClass 구조체에 넣는다.
	WNDCLASSW wndclass = { 0, WndProc, 0,0,0,0,0,0,0, WindowClass };

	//윈도우 클래스 등록
	RegisterClassW(&wndclass);

	// 1024 x 1024 크기에 윈도우 생성
	HWND hWnd = CreateWindowExW(0, WindowClass, Title, WS_POPUP | WS_VISIBLE | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 1024, nullptr, nullptr, hInstance, nullptr);

    URenderer renderer;

    renderer.Create(hWnd);
    renderer.CreateShader();  // 렌더러 생성 직후에 쉐이더를 생성하는 함수를 호출합니다.
    // 상수버퍼 생성
    renderer.CreateConstantBuffer();
    //렌더러 생성 후 ImGui 생성 및 초기화 함수 호출
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

	//각종 생성하는 코드를 여기에 추가합니다.
    enum ETypePrimitive
    {
        EPT_Triangle,
        EPT_Cube,
        EPT_Sphere,
        EPT_Max,
    };

    ETypePrimitive typePrimitive = EPT_Triangle;
    
    FVector3 offset(0.0f); // 도형의 움직임 정도를 담을 offset 변수
    FVector3 velocity(0.0f); // 도형의 움직임 정도를 담을 velocity 변수

    const float leftBorder = -1.0f;
    const float rightBorder = 1.0f;
    const float topBorder = -1.0f;
    const float bottomBorder = 1.0f;
    const float sphereRadius= 1.0f;

    bool bBoundBallToScreen = true;
    bool bPinballMovement = true;

    //초기속도 define

    const float ballSpeed = 0.004f;
    velocity.x = ((float)(rand() % 100 - 50)) * ballSpeed;
    velocity.y = ((float)(rand() % 100 - 50)) * ballSpeed;//0.001f를 조절해 초기속도 조절

    const int targetFPS = 60;
    const double targetFrameTime = 1000.0 / targetFPS;

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER startTime, endTime;
    double elapsedTime = 0.0;

	//Main Loop (Quit Message가 들어오기 전까지 아래 Loop를 무한히 실행)
	while (bIsExit == false) {
        QueryPerformanceCounter(&startTime);
        
        MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

			// 키 입력 메세지를 번역
			TranslateMessage(&msg);
			//메세지를 적절한 윈도우 프로시저에 전달, 메세지가 위에서 등록한 WndProc으로 전달
			DispatchMessage(&msg);
			
			if (msg.message == WM_QUIT) {
				bIsExit = true;
				break;
			}
            /* PeekMessage while문 밖에서 따로 입력 처리*/
            //else if (!bPinballMovement && msg.message == WM_KEYDOWN)
            //{
            //    // 눌린 키가 방향키면 해당 방향에 맞춰서
            //    // offset 변수의 x, y 멤버 변수의 값을 조정
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
                // 메시지 처리 후 키 상태 폴링
                if (!bPinballMovement)
                {
                    // 왼쪽 키가 눌려있는지 확인
                    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
                    {
                        offset.x -= 0.01f;
                    }
                    // 오른쪽 키가 눌려있는지 확인
                    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
                    {
                        offset.x += 0.01f;
                    }
                    // 위쪽 키가 눌려있는지 확인
                    if (GetAsyncKeyState(VK_UP) & 0x8000)
                    {
                        offset.y += 0.01f;
                    }
                    // 아래쪽 키가 눌려있는지 확인
                    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
                    {
                        offset.y -= 0.01f;
                    }
                }

            }
            //화면 밖으로 나가지 않게
            if (bBoundBallToScreen)
            {
                float renderRadius = sphereRadius * scaleMod;
                if (offset.x < leftBorder + renderRadius) offset.x = leftBorder + renderRadius;
                if (offset.x > rightBorder - renderRadius) offset.x = rightBorder - renderRadius;
                if (offset.y < topBorder + renderRadius) offset.y = topBorder + renderRadius;
                if (offset.y > bottomBorder - renderRadius) offset.y = bottomBorder - renderRadius;
            }
		}
        // 준비 작업
        renderer.Prepare();
        renderer.PrepareShader();

        // 생성한 버텍스 버퍼를 넘겨 실질적인 렌더링 요청

        // offset을 상수 버퍼로 업데이트
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

        //ImGui 렌더링 준비, 컨트롤 설정
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 이후 ImGui UI 컨트롤 추가는 ImGui::NewFrame()과 ImGui::Render() 사이인 여기에 위치합니다.
		ImGui::Begin("Jungle Property Window");

        ImGui::Text("Hello Jungle World!");

        ImGui::Checkbox("Bound Ball To Screen", &bBoundBallToScreen);
        ImGui::Checkbox("Pinball Movement", &bPinballMovement);
        //if (ImGui::Button("Quit this app"))
        //{
        //    // 현재 윈도우에 Quit 메시지를 메시지 큐로 보냄
        //    PostMessage(hWnd, WM_QUIT, 0, 0);
        //}

        // Change primitive 버튼
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

        //렌더링 요청 함수?
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


        // 다 그렸으면 버퍼를 교환
        renderer.SwapBuffer();

        do
        {
            Sleep(0);

            QueryPerformanceCounter(&endTime);

            elapsedTime = (endTime.QuadPart - startTime.QuadPart) * 1000.0 / frequency.QuadPart;


        } while (elapsedTime < targetFrameTime);
	}

    //ImGui 소멸 함수들 호출
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // 버텍스 버퍼 소멸은 Renderer 소멸전에 처리합니다.
    //vertexBuffer->Release();

    renderer.ReleaseVertexBuffer(vertexBufferTriangle);
    renderer.ReleaseVertexBuffer(vertexBufferCube);
    renderer.ReleaseVertexBuffer(vertexBufferSphere);

    // 렌더러 소멸
    renderer.ReleaseConstantBuffer(); // 상수버퍼 소멸
    renderer.ReleaseShader(); //렌더러 소멸 직전에 쉐이더를 소멸 시키는 함수를 호출
    renderer.Release();
	return 0;

}