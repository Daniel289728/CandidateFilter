#include "SkiaUI.h"
#include "include/core/SkRect.h"
#include "include/core/SkRRect.h"

SkiaUI::SkiaUI(int width, int height, Parser* parser, CandidateScorer* scorer)
    : width_(width)
    , height_(height)
    , parser_(parser)
    , scorer_(scorer)
    , scrollOffset_(0)
    , isDragging_(false) {
}

void SkiaUI::render(SkCanvas* canvas) {
    // Clear background
    SkPaint bgPaint;
    bgPaint.setColor(SK_ColorWHITE);
    canvas->drawPaint(bgPaint);

    renderHeader(canvas);
    renderSidebar(canvas);
    renderMainContent(canvas);
}

void SkiaUI::renderHeader(SkCanvas* canvas) {
    // Draw header background
    SkPaint headerPaint;
    headerPaint.setColor(SkColorSetRGB(50, 50, 50));
    canvas->drawRect(SkRect::MakeWH(static_cast<float>(width_),
        static_cast<float>(HEADER_HEIGHT)), headerPaint);

    // Draw title
    drawText(canvas, "Candidate Analyzer", PADDING, HEADER_HEIGHT / 2 + 8, 24, SK_ColorWHITE);
}

void SkiaUI::renderSidebar(SkCanvas* canvas) {
    // Draw sidebar background
    SkPaint sidebarPaint;
    sidebarPaint.setColor(SkColorSetRGB(240, 240, 240));

    canvas->drawRect(
        SkRect::MakeXYWH(0, static_cast<float>(HEADER_HEIGHT),
            static_cast<float>(SIDEBAR_WIDTH),
            static_cast<float>(height_ - HEADER_HEIGHT)),
        sidebarPaint
    );

    // Draw refresh button
    SkPaint buttonPaint;
    buttonPaint.setColor(SkColorSetRGB(70, 130, 210));

    SkRect buttonRect = SkRect::MakeXYWH(
        static_cast<float>(PADDING),
        static_cast<float>(HEADER_HEIGHT + PADDING),
        120,
        40
    );
    canvas->drawRect(buttonRect, buttonPaint);

    // Draw button text
    drawText(canvas, "Refresh", buttonRect.centerX() - 30,
        buttonRect.centerY() + 6, 16, SK_ColorWHITE);
}

void SkiaUI::renderMainContent(SkCanvas* canvas) {
    // Draw main content background
    SkPaint contentPaint;
    contentPaint.setColor(SK_ColorWHITE);

    canvas->drawRect(
        SkRect::MakeXYWH(static_cast<float>(SIDEBAR_WIDTH),
            static_cast<float>(HEADER_HEIGHT),
            static_cast<float>(width_ - SIDEBAR_WIDTH),
            static_cast<float>(height_ - HEADER_HEIGHT)),
        contentPaint
    );

    // Draw content heading
    drawText(canvas, "Candidates", SIDEBAR_WIDTH + PADDING,
        HEADER_HEIGHT + PADDING + 20, 18, SK_ColorBLACK);
}

void SkiaUI::drawText(SkCanvas* canvas, const char* text, float x, float y,
    float size, SkColor color) {
    SkPaint paint;
    paint.setColor(color);
    paint.setAntiAlias(true);

    SkFont font;
    font.setSize(size);

    canvas->drawString(text, x, y, font, paint);
}

void SkiaUI::handleClick(int x, int y) {
    // Handle refresh button click
    if (x >= PADDING && x <= PADDING + 120 &&
        y >= HEADER_HEIGHT + PADDING && y <= HEADER_HEIGHT + PADDING + 40) {
        // Refresh data
    }
}

void SkiaUI::handleMouseMove(int x, int y) {
    // Handle mouse movement if needed
}

void SkiaUI::resize(int width, int height) {
    width_ = width;
    height_ = height;
}