#include <include/core/SkCanvas.h>
#include <include/core/SkSurface.h>
#include <include/core/SkGraphics.h>
#include <include/core/SkBitmap.h>
#include "../backend/CandidateScorer.h"
#include "../backend/Parser.h"
#include "../ui/SkiaUI.h"
#include <windows.h>
#include <vector>
#include <memory>
#include <string>

// Forward declaration of the main Windows procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Global variables
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const char* const WINDOW_CLASS_NAME = "CandidateAnalyzerWindow";
SkBitmap bitmap;
std::unique_ptr<SkCanvas> canvas;
std::unique_ptr<SkiaUI> skiaUI;

// Main application logic
int RunApplication(HINSTANCE hInstance, int nCmdShow) {
    // Initialize Skia
    SkGraphics::Init();
    
    // Register window class
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClassEx(&wc);
    
    // Create window
    HWND hwnd = CreateWindowEx(
        0,
        WINDOW_CLASS_NAME,
        "Candidate Analyzer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    
    if (!hwnd) {
        return 1;
    }
    
    // Create initial bitmap and canvas
    bitmap.allocPixels(SkImageInfo::MakeN32Premul(WINDOW_WIDTH, WINDOW_HEIGHT));
    canvas = std::make_unique<SkCanvas>(bitmap);
    
    // Initialize UI
    Parser parser;
    CandidateScorer scorer;
    skiaUI = std::make_unique<SkiaUI>(WINDOW_WIDTH, WINDOW_HEIGHT, &parser, &scorer);
    
    // Show window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return static_cast<int>(msg.wParam);
}

// Windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    return RunApplication(hInstance, nCmdShow);
}

// Standard entry point
int main(int argc, char* argv[]) {
    return RunApplication(GetModuleHandle(nullptr), SW_SHOW);
}

// Window procedure implementation
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            if (canvas && skiaUI) {
                canvas->clear(SK_ColorWHITE);
                skiaUI->render(canvas.get());
                
                BITMAPINFO bmi = { 0 };
                bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bmi.bmiHeader.biWidth = bitmap.width();
                bmi.bmiHeader.biHeight = -bitmap.height(); // Top-down
                bmi.bmiHeader.biPlanes = 1;
                bmi.bmiHeader.biBitCount = 32;
                bmi.bmiHeader.biCompression = BI_RGB;
                
                SetDIBitsToDevice(hdc,
                    0, 0,
                    bitmap.width(), bitmap.height(),
                    0, 0,
                    0, bitmap.height(),
                    bitmap.getPixels(),
                    &bmi,
                    DIB_RGB_COLORS);
            }
            
            EndPaint(hwnd, &ps);
            return 0;
        }
        
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            
            if (width > 0 && height > 0) {
                bitmap.allocPixels(SkImageInfo::MakeN32Premul(width, height));
                canvas = std::make_unique<SkCanvas>(bitmap);
                
                if (skiaUI) {
                    skiaUI->resize(width, height);
                }
                
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            return 0;
        }
        
        case WM_LBUTTONDOWN: {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            if (skiaUI) {
                skiaUI->handleClick(xPos, yPos);
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            return 0;
        }
        
        case WM_MOUSEMOVE: {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            if (skiaUI) {
                skiaUI->handleMouseMove(xPos, yPos);
                InvalidateRect(hwnd, nullptr, TRUE);
            }
            return 0;
        }
        
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
