#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memory>
#include <d3d11_1.h>

#include "Win32Project1.h"

#include "backbuffer.h"
#include "raytracer.h"
#include "world.h"
#include "maths.h"
#include "sphere.h"
#include "vec3.h"
#include "camera.h"

#include "CompiledVertexShader.h"
#include "CompiledPixelShader.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
static HWND g_Wnd;

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
static char s_Buffer[ 200 ];

static HRESULT InitD3DDevice();
static void ShutdownD3DDevice();
static void RenderFrame();

static D3D_FEATURE_LEVEL g_D3D11FeatureLevel = D3D_FEATURE_LEVEL_11_0;
static ID3D11Device* g_D3D11Device = nullptr;
static ID3D11DeviceContext* g_D3D11Ctx = nullptr;
static IDXGISwapChain* g_D3D11SwapChain = nullptr;
static ID3D11RenderTargetView* g_D3D11RenderTarget = nullptr;
static ID3D11VertexShader* g_VertexShader;
static ID3D11PixelShader* g_PixelShader;
static ID3D11Texture2D* g_BackbufferTexture;
static ID3D11ShaderResourceView* g_BackbufferSRV;
static ID3D11SamplerState* g_SamplerLinear;
static ID3D11RasterizerState* g_RasterState;

std::unique_ptr< Backbuffer > backBuffer;
std::unique_ptr< RayTracer > rayTracer;
std::unique_ptr< World > world;
std::unique_ptr< Camera > camera;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow )
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    backBuffer = std::make_unique< Backbuffer >( 200, 100 );

    const auto r = cosf( kPI / 4.0f );

    std::vector< std::shared_ptr< Hitable > > hitables = 
    {
        std::make_shared< Sphere >( Vec3( 0.0f, -1000.0f, 0.0f ), 1000.0f, std::make_shared< MaterialLambert >( Vec3( 0.5f, 0.5f, 0.5f ) ) ),
        std::make_shared< Sphere >( Vec3( 0.0f, 1.0f, 0.0f ), 1.0f, std::make_shared< MaterialDiElectric >( 1.5f ) ),
        std::make_shared< Sphere >( Vec3( -4.0f, 1.0f, 0.0f ), 1.0f, std::make_shared< MaterialLambert >( Vec3( 0.4f, 0.2f, 0.1f ) ) ),
        std::make_shared< Sphere >( Vec3( 4.0f, 1.0f, 0.0f ), 1.0f, std::make_shared< MaterialMetal>( Vec3( 0.7f, 0.6f, 0.5f ), 0.0f ) )
    };

    int n = 50000;

    int i = 1;

    uint32_t state = 1337;

    for ( auto a = -10; a < 10; a++ )
    {
        for ( auto b = -10; b < 10; b++ )
        {
            Vec3 center( a + 0.9 * RandomFloat01( state ), 0.2f, b + RandomFloat01( state ) );

            if ( ( center - Vec3( 4, 0.2, 0 ) ).Length() > 0.9f )
            {
                hitables.push_back( 
                    std::make_shared< Sphere >( 
                        center, 
                        0.2f, 
                        std::make_shared< MaterialMetal >( 
                            Vec3( 
                                0.5f * ( 1 + RandomFloat01( state ) ),
                                0.5f * ( 1 + RandomFloat01( state ) ),
                                0.5f * ( 1 + RandomFloat01( state ) )
                                ),
                            0.5f * RandomFloat01( state )
                            ) 
                        )
                    );
            }
        }
    }

    world = std::make_unique< World >( hitables );
    rayTracer = std::make_unique< RayTracer >( 4, 10 );

    const auto aspect_ratio = static_cast< float >( backBuffer->GetWidth() ) / static_cast< float >( backBuffer->GetHeight() );

    static const Vec3 
        look_from( 13.0f, 2.0f, 3.0f ),
        look_at( 0.0f, 0.0f, 0.0f );
    static const auto
        fov = 20.0f,
        aperture = 0.0f,
        focus_distance = 10.0f;

    camera = std::make_unique< Camera >( look_from, look_at, Vec3( 0.0f, 1.0f, 0.0f ), fov, aspect_ratio, aperture, focus_distance );

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    if ( FAILED( InitD3DDevice() ) )
    {
        ShutdownD3DDevice();
        return 0;
    }

    g_D3D11Device->CreateVertexShader( g_VSBytecode, ARRAYSIZE( g_VSBytecode ), NULL, &g_VertexShader );
    g_D3D11Device->CreatePixelShader( g_PSBytecode, ARRAYSIZE( g_PSBytecode ), NULL, &g_PixelShader );

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = backBuffer->GetWidth();
    texDesc.Height = backBuffer->GetHeight();
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DYNAMIC;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    texDesc.MiscFlags = 0;
    g_D3D11Device->CreateTexture2D( &texDesc, NULL, &g_BackbufferTexture );

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    g_D3D11Device->CreateShaderResourceView( g_BackbufferTexture, &srvDesc, &g_BackbufferSRV );

    D3D11_SAMPLER_DESC smpDesc = {};
    smpDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    smpDesc.AddressU = smpDesc.AddressV = smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    g_D3D11Device->CreateSamplerState( &smpDesc, &g_SamplerLinear );

    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.CullMode = D3D11_CULL_NONE;
    g_D3D11Device->CreateRasterizerState( &rasterDesc, &g_RasterState );

    MSG msg = { 0 };
    while ( msg.message != WM_QUIT )
    {
        if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            RenderFrame();
        }
    }

    ShutdownD3DDevice();

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WIN32PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   g_Wnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

static void RenderFrame()
{
    LARGE_INTEGER time1;
    QueryPerformanceCounter( &time1 );

    int ray_count = 0;

    rayTracer->Process( *backBuffer.get(), ray_count, *world.get(), *camera.get() );

    LARGE_INTEGER time2;
    QueryPerformanceCounter( &time2 );
    uint64_t dt = time2.QuadPart - time1.QuadPart;

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency( &frequency );

    double s = double( dt ) / double( frequency.QuadPart );
    sprintf_s( s_Buffer, sizeof( s_Buffer ), "%.2fms (%.1f FPS) %.2fMrays/s \n", s * 1000.0f, 1.f / s, ray_count / s * 1.0e-6f );
    SetWindowTextA( g_Wnd, s_Buffer );
    OutputDebugStringA( s_Buffer );

    if ( auto * back_buffer = backBuffer.get() )
    {
        D3D11_MAPPED_SUBRESOURCE mapped;
        g_D3D11Ctx->Map( g_BackbufferTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped );
        const uint8_t* src = ( const uint8_t* ) back_buffer->GetData();
        uint8_t* dst = ( uint8_t* ) mapped.pData;
        for ( int y = 0; y < back_buffer->GetHeight(); ++y )
        {
            memcpy( dst, src, back_buffer->GetWidth() * 16 );
            src += back_buffer->GetWidth() * 16;
            dst += mapped.RowPitch;
        }
        g_D3D11Ctx->Unmap( g_BackbufferTexture, 0 );
    }

    g_D3D11Ctx->VSSetShader( g_VertexShader, NULL, 0 );
    g_D3D11Ctx->PSSetShader( g_PixelShader, NULL, 0 );
    g_D3D11Ctx->PSSetShaderResources( 0, 1, &g_BackbufferSRV );
    g_D3D11Ctx->PSSetSamplers( 0, 1, &g_SamplerLinear );
    g_D3D11Ctx->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
    g_D3D11Ctx->RSSetState( g_RasterState );
    g_D3D11Ctx->Draw( 3, 0 );
    g_D3D11SwapChain->Present( 0, 0 );
}

static HRESULT InitD3DDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_Wnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT numFeatureLevels = ARRAYSIZE( featureLevels );
    hr = D3D11CreateDevice( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &g_D3D11Device, &g_D3D11FeatureLevel, &g_D3D11Ctx );
    if ( FAILED( hr ) )
        return hr;

    // Get DXGI factory
    IDXGIFactory1* dxgiFactory = nullptr;
    {
        IDXGIDevice* dxgiDevice = nullptr;
        hr = g_D3D11Device->QueryInterface( __uuidof( IDXGIDevice ), reinterpret_cast<void**>( &dxgiDevice ) );
        if ( SUCCEEDED( hr ) )
        {
            IDXGIAdapter* adapter = nullptr;
            hr = dxgiDevice->GetAdapter( &adapter );
            if ( SUCCEEDED( hr ) )
            {
                hr = adapter->GetParent( __uuidof( IDXGIFactory1 ), reinterpret_cast<void**>( &dxgiFactory ) );
                adapter->Release();
            }
            dxgiDevice->Release();
        }
    }
    if ( FAILED( hr ) )
        return hr;

    // Create swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = g_Wnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    hr = dxgiFactory->CreateSwapChain( g_D3D11Device, &sd, &g_D3D11SwapChain );

    // Prevent Alt-Enter
    dxgiFactory->MakeWindowAssociation( g_Wnd, DXGI_MWA_NO_ALT_ENTER );
    dxgiFactory->Release();

    if ( FAILED( hr ) )
        return hr;

    // RTV
    ID3D11Texture2D* pBackBuffer = nullptr;
    hr = g_D3D11SwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), reinterpret_cast<void**>( &pBackBuffer ) );
    if ( FAILED( hr ) )
        return hr;
    hr = g_D3D11Device->CreateRenderTargetView( pBackBuffer, nullptr, &g_D3D11RenderTarget );
    pBackBuffer->Release();
    if ( FAILED( hr ) )
        return hr;

    g_D3D11Ctx->OMSetRenderTargets( 1, &g_D3D11RenderTarget, nullptr );

    // Viewport
    D3D11_VIEWPORT vp;
    vp.Width = ( float ) width;
    vp.Height = ( float ) height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_D3D11Ctx->RSSetViewports( 1, &vp );

    return S_OK;
}

static void ShutdownD3DDevice()
{
    if ( g_D3D11Ctx ) g_D3D11Ctx->ClearState();

    if ( g_D3D11RenderTarget ) g_D3D11RenderTarget->Release();
    if ( g_D3D11SwapChain ) g_D3D11SwapChain->Release();
    if ( g_D3D11Ctx ) g_D3D11Ctx->Release();
    if ( g_D3D11Device ) g_D3D11Device->Release();
}
