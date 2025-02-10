#ifndef SKIAUI_H
#define SKIAUI_H

#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkPaint.h"
#include "../backend/CandidateScorer.h"
#include "../backend/Parser.h"
#include <memory>
#include <vector>
#include <string>

class SkiaUI {
public:
    SkiaUI(int width, int height, Parser* parser, CandidateScorer* scorer);
    ~SkiaUI() = default;

    void render(SkCanvas* canvas);
    void resize(int width, int height);
    void handleClick(int x, int y);
    void handleMouseMove(int x, int y);

private:
    // Layout constants
    static constexpr int HEADER_HEIGHT = 60;
    static constexpr int SIDEBAR_WIDTH = 300;
    static constexpr int PADDING = 20;

    // Member variables
    int width_;
    int height_;
    Parser* parser_;
    CandidateScorer* scorer_;

    // UI state
    int scrollOffset_;
    bool isDragging_;

    // Helper methods
    void renderHeader(SkCanvas* canvas);
    void renderSidebar(SkCanvas* canvas);
    void renderMainContent(SkCanvas* canvas);
    void drawText(SkCanvas* canvas, const char* text, float x, float y,
        float size, SkColor color);
};
#endif
