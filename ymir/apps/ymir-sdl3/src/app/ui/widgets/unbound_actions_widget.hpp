#pragma once

#include <app/shared_context.hpp>

#include <app/input/input_context.hpp>

#include <optional>
#include <set>
#include <unordered_set>

namespace app::ui::widgets {

class UnboundActionsWidget {
public:
    UnboundActionsWidget(SharedContext &context);

    void Display();

    void Capture(const std::optional<input::MappedAction> &unboundAction);
    void Capture(const std::unordered_set<input::MappedAction> &unboundActions);

private:
    SharedContext &m_context;

    std::set<input::MappedAction> m_unboundActions;
};

} // namespace app::ui::widgets
