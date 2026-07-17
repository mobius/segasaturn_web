#pragma once

#include <ymir/util/dev_log.hpp>

#include <SDL3/SDL_error.h>

#include <cassert>
#include <concepts>
#include <filesystem>
#include <span>
#include <string_view>
#include <vector>

namespace util {

namespace grp {

    // -----------------------------------------------------------------------------
    // Dev log groups

    // Hierarchy:
    //
    // base

    struct base {
        static constexpr bool enabled = true;
        static constexpr devlog::Level level = devlog::level::debug;
        static constexpr std::string_view name = "FileDialog";
    };

} // namespace grp

inline void NoopCancelFileDialogCallback(void *, int) {}

// SDL3 file dialog callback function wrapping functions with the following signatures:
//   void (*acceptCallback)(void *userdata, std::filesystem::path path, int filter)
//   void (*cancelCallback)(void *userdata, int filter)
//   void (*errorCallback)(void *userdata, const char *errorMessage, int filter)
// The wrapper function expects only one file or directory to be selected. Useful for save file or open directory
// dialogs.
//
// If a file is selected, the accept callback is invoked.
// If multiple files are selected, the callback is invoked with the first file in the selection. In debug builds, an
// assertion is raised in this case.
// If the user cancels or the file dialog fails to open, a dev log message is printed and the cancel callback is
// invoked.
// If an error occurred, a dev log message is printed and the error callback is invoked with the error message.
template <auto acceptCallback, auto cancelCallback, auto errorCallback>
    requires(std::invocable<decltype(acceptCallback), void *, std::filesystem::path, int> &&
             std::invocable<decltype(cancelCallback), void *, int> &&
             std::invocable<decltype(errorCallback), void *, const char *, int>)
void WrapSingleSelectionCallback(void *userdata, const char *const *filelist, int filter) {
    if (filelist == nullptr) {
        const char *errorMessage = SDL_GetError();
        devlog::error<grp::base>("Failed to open file dialog: {}", errorMessage);
        errorCallback(userdata, errorMessage, filter);
    } else if (*filelist == nullptr) {
        devlog::info<grp::base>("File dialog cancelled");
        cancelCallback(userdata, filter);
    } else {
        // Only one file or directory should be selected
        assert(filelist[1] == nullptr);
        const char *file = *filelist;
        std::string fileStr = file;
        const std::u8string u8File{fileStr.begin(), fileStr.end()};
        acceptCallback(userdata, u8File, filter);
    }
}

// SDL3 file dialog callback function wrapping functions with the following signatures:
//   void (*acceptCallback)(void *userdata, std::span<std::filesystem::path> files, int filter)
//   void (*cancelCallback)(void *userdata, int filter)
//   void (*errorCallback)(void *userdata, const char *errorMessage, int filter)
// Useful for open file dialogs with multiple selection.
//
// If at least one file is selected, the accept callback is invoked with all selected files.
// If the user cancels or the file dialog fails to open, a dev log message is printed and the cancel callback is
// invoked.
// If an error occurred, a dev log message is printed and the error callback is invoked with the error message.
template <auto acceptCallback, auto cancelCallback, auto errorCallback>
    requires(std::invocable<decltype(acceptCallback), void *, std::span<std::filesystem::path>, int> &&
             std::invocable<decltype(cancelCallback), void *, int> &&
             std::invocable<decltype(errorCallback), void *, const char *, int>)
void WrapMultiSelectionCallback(void *userdata, const char *const *filelist, int filter) {
    if (filelist == nullptr) {
        const char *errorMessage = SDL_GetError();
        devlog::error<grp::base>("Failed to open file dialog: {}", errorMessage);
        errorCallback(userdata, errorMessage, filter);
    } else if (*filelist == nullptr) {
        devlog::info<grp::base>("File dialog cancelled");
        cancelCallback(userdata, filter);
    } else {
        std::vector<std::filesystem::path> files{};
        while (*filelist != nullptr) {
            std::string fileStr = *filelist;
            const std::u8string u8File{fileStr.begin(), fileStr.end()};
            files.push_back(u8File);
            filelist++;
        }
        acceptCallback(userdata, files, filter);
    }
}

} // namespace util
