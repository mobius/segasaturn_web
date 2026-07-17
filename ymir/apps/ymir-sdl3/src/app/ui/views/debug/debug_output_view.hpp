#pragma once

#include <app/shared_context.hpp>

#include <app/debug/scu_tracer.hpp>

namespace app::ui {

class DebugOutputView {
public:
    DebugOutputView(SharedContext &context);

    void Display();

private:
    SharedContext &m_context;
    SCUTracer &m_tracer;

    static void ProcessExportDebugOutput(void *userdata, std::filesystem::path file, int filter);
    static void ProcessCancelExport(void *userdata, int filter);
    static void ProcessExportError(void *userdata, const char *errorMessage, int filter);

    void ExportDebugOutput(std::filesystem::path file);
    void ShowErrorDialog(const char *message);
};

} // namespace app::ui
