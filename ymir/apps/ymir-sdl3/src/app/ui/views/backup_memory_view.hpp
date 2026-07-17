#pragma once

#include <app/shared_context.hpp>

#include <ymir/sys/backup_ram_defs.hpp>

#include <imgui.h>

#include <filesystem>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace app::ui {

class BackupMemoryView {
public:
    BackupMemoryView(SharedContext &context, std::string_view name, bool external);

    // NOTE: for external backup memory, the lock must be held by the caller
    // Pass nullptr to display an empty/disabled/unavailable backup memory list
    void SetBackupMemory(ymir::bup::IBackupMemory *bup);

    void Display();

    bool HasBackupMemory() const;
    bool HasSelection() const;

    std::vector<ymir::bup::BackupFile> ExportAll() const;
    std::vector<ymir::bup::BackupFile> ExportSelected() const;
    void ImportAll(std::span<const ymir::bup::BackupFile> files);

private:
    SharedContext &m_context;
    std::string m_name;
    bool m_external;

    std::set<std::string> m_selected;
    ymir::bup::IBackupMemory *m_bup = nullptr;

    // -----------------------------------------------------------------------------------------------------------------
    // File table drawing

    void ApplyRequests(ImGuiMultiSelectIO *msio, std::vector<ymir::bup::BackupFileInfo> &files);

    void DrawFileTableHeader();
    void DrawFileTableRow(const ymir::bup::BackupFileInfo &file, uint32 index = 0, bool selectable = false);

    // -----------------------------------------------------------------------------------------------------------------
    // Popups and modals

    void OpenFileImportOverwriteModal();
    void OpenFileImportResultModal();
    void OpenFilesExportSuccessfulModal(uint32 exportCount);
    void OpenImageImportSuccessfulModal();
    void OpenImageExportSuccessfulModal();
    void OpenErrorModal(std::string errorMessage);

    void DisplayConfirmDeleteModal(std::span<ymir::bup::BackupFileInfo> files);
    void DisplayConfirmFormatModal();
    void DisplayFileImportOverwriteModal(std::span<ymir::bup::BackupFileInfo> files);
    void DisplayFileImportResultModal();
    void DisplayFilesExportSuccessfulModal();
    void DisplayImageImportSuccessfulModal();
    void DisplayImageExportSuccessfulModal();
    void DisplayErrorModal();

    bool m_openFileImportOverwriteModal = false;
    bool m_openFileImportResultModal = false;

    bool m_openFilesExportSuccessfulModal = false;
    uint32 m_filesExportCount;

    bool m_openImageImportSuccessfulModal = false;
    bool m_openImageExportSuccessfulModal = false;

    bool m_openErrorModal = false;
    std::string m_errorModalMessage;

    // -----------------------------------------------------------------------------------------------------------------
    // File import action

    static void ProcessFileImport(void *userdata, std::span<std::filesystem::path> files, int filter);
    static void ProcessCancelFileImport(void *userdata, int filter);
    static void ProcessFileImportError(void *userdata, const char *errorMessage, int filter);

    void ImportFiles(std::span<std::filesystem::path> files);
    void CancelFileImport();
    void FileImportError(const char *errorMessage);

    enum class ImportFileResult { Success, FilesystemError, FileTruncated, BadMagic };

    ImportFileResult ImportFile(std::filesystem::path path, ymir::bup::BackupFile &out, std::error_code &error);

    struct BadImport {
        std::filesystem::path file;
        std::string errorMessage;
    };

    struct FailedImport {
        ymir::bup::BackupFileHeader file;
        std::string errorMessage;
    };

    struct OverwriteImport {
        ymir::bup::BackupFile file;
        bool overwrite = false;
    };

    std::vector<BadImport> m_importBad;
    std::vector<FailedImport> m_importFailed;
    std::vector<OverwriteImport> m_importOverwrite;
    std::vector<ymir::bup::BackupFileHeader> m_importSuccess;

    // -----------------------------------------------------------------------------------------------------------------
    // File export action

    struct ExportedFile {
        ymir::bup::BackupFile file;
        ymir::bup::BackupFileInfo info;
    };

    static void ProcessSingleFileExport(void *userdata, std::filesystem::path file, int filter);
    static void ProcessMultiFileExport(void *userdata, std::filesystem::path dir, int filter);
    static void ProcessCancelFileExport(void *userdata, int filter);
    static void ProcessFileExportError(void *userdata, const char *errorMessage, int filter);

    void ExportSingleFile(std::filesystem::path file);
    void ExportMultiFile(std::filesystem::path dir);
    void CancelFileExport();
    void FileExportError(const char *errorMessage);

    void ExportFile(std::filesystem::path path, const ExportedFile &exportedFile);

    std::vector<ExportedFile> m_filesToExport;

    // -----------------------------------------------------------------------------------------------------------------
    // Load image action

    static void ProcessImageImport(void *userdata, std::filesystem::path file, int filter);
    static void ProcessCancelImageImport(void *userdata, int filter);
    static void ProcessImageImportError(void *userdata, const char *errorMessage, int filter);

    void ImportImage(std::filesystem::path file);
    void CancelImageImport();
    void ImageImportError(const char *errorMessage);

    // -----------------------------------------------------------------------------------------------------------------
    // Save image action

    static void ProcessImageExport(void *userdata, std::filesystem::path file, int filter);
    static void ProcessCancelImageExport(void *userdata, int filter);
    static void ProcessImageExportError(void *userdata, const char *errorMessage, int filter);

    void ExportImage(std::filesystem::path file);
    void CancelImageExport();
    void ImageExportError(const char *errorMessage);

    std::vector<uint8> m_imageToSave;
};

} // namespace app::ui
