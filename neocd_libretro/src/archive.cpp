#include "archive.h"
#include "archivezip.h"
#include "libretro_log.h"
#include "misc.h"
#include "path.h"
#include "streams/file_stream.h"

static Archive::Type getArchiveType(const std::string& archiveFilename)
{
    auto ext = path_get_extension(archiveFilename.c_str());

    if (string_compare_insensitive(ext, "zip"))
        return Archive::TypeZip;

    return Archive::TypeUnknown;
}

static bool readFileUncompressed(const std::string &path, void *buffer, size_t maximumSize, size_t *reallyRead)
{
    RFILE* file = filestream_open(path.c_str(), RETRO_VFS_FILE_ACCESS_READ, RETRO_VFS_FILE_ACCESS_HINT_NONE);
    if (!file)
        return false;

    size_t wasRead = filestream_read(file, buffer, maximumSize);

    filestream_close(file);

    if (reallyRead)
        *reallyRead = wasRead;

    return true;
}

static int64_t getFileSizeUncompressed(const std::string &path)
{
    RFILE* file = filestream_open(path.c_str(), RETRO_VFS_FILE_ACCESS_READ, RETRO_VFS_FILE_ACCESS_HINT_NONE);

    if (!file)
        return -1;

    int64_t ret = filestream_get_size(file);

    filestream_close(file);

    return ret;
}

namespace Archive
{
std::vector<std::string> getFileList(const std::string &archiveFilename)
{
    const auto archiveType = getArchiveType(archiveFilename);

    if (archiveType == TypeZip)
        return ArchiveZip::getFileList(archiveFilename);

    Libretro::Log::message(RETRO_LOG_ERROR, "Archive: Unknown archive type %s\n", archiveFilename.c_str());
    return std::vector<std::string>();
}

int64_t getFileSize(const std::string &path)
{
    std::string archive;
    std::string filename;

    split_compressed_path(path, archive, filename);

    if (archive.empty())
        return getFileSizeUncompressed(filename);

    const auto archiveType = getArchiveType(archive);

    if (archiveType == TypeZip)
	return ArchiveZip::getFileSize(archive, filename);

    Libretro::Log::message(RETRO_LOG_ERROR, "Archive: Unknown archive type %s\n", archive.c_str());

    return -1;
}

bool readFile(const std::string &path, void *buffer, size_t maximumSize, size_t *reallyRead)
{
    std::string archive;
    std::string filename;
    split_compressed_path(path, archive, filename);

    if (archive.empty())
        return readFileUncompressed(filename, buffer, maximumSize, reallyRead);

    const auto archiveType = getArchiveType(archive);

    if (archiveType == TypeZip)
        return ArchiveZip::readFile(archive, filename, buffer, maximumSize, reallyRead);

    Libretro::Log::message(RETRO_LOG_ERROR, "Archive: Unknown archive type %s\n", archive.c_str());

    return false;
}
} // namespace Archive
