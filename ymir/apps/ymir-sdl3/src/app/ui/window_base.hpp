#pragma once

#include <app/shared_context.hpp>

#include <imgui.h>

#include <string>

namespace app::ui {

class WindowBase {
public:
    WindowBase(SharedContext &context);
    virtual ~WindowBase() = default;

    void Display();

    void RequestFocus();

    bool Open = false;

protected:
    SharedContext &m_context;

    struct WindowConfig {
        std::string name;
        ImGuiWindowFlags flags = ImGuiWindowFlags_None;
        bool allowClosingWithGamepad = true;
    };

    WindowConfig m_windowConfig;

    // Invoked before ImGui::Begin(...).
    // Can be used to set up window constraints, update name and flags dynamically, etc.
    virtual void PrepareWindow() {}

    // Draws the window's contents.
    virtual void DrawContents() = 0;

private:
    bool m_focusRequested = false;
};

} // namespace app::ui
