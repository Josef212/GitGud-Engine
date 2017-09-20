#include "M_FileSystem.h"

#include "App.h"

#include "Path.h"

#include "PhysFS\include\physfs.h"
#include <SDL.h>


M_FileSystem::M_FileSystem(const char* name, bool startEnabled) : Module(name, startEnabled)
{
	_LOG(LOG_INFO, "FileSystem: Creation.");

	char* basePath = SDL_GetBasePath();
	//TODO: Path according to DEBUG / RELEASE???
	PHYSFS_init(basePath);

	AddPath(".");
	AddPath("Data");

	SDL_free(basePath);
}

M_FileSystem::~M_FileSystem()
{
	_LOG(LOG_INFO, "FileSystem: Destroying.");
	PHYSFS_deinit();
}

bool M_FileSystem::Init(JsonFile* file)
{
	_LOG(LOG_INFO, "FileSystem: Init.");
	bool ret = true;

	char* writePath = SDL_GetBasePath();

	if (PHYSFS_setWriteDir(writePath) == 0)
	{
		_LOG(LOG_ERROR, "Could not set write dir: %s\n", PHYSFS_getLastError());
		ret = false;
	}
	else
	{
		_LOG(LOG_INFO, "FS: Write dir is %s\n", writePath);
		AddPath(writePath, GetSaveDir());

		_LOG(LOG_INFO, "Fs: Base path: %s\n", GetBaseDir());

	}

	DisplaySearchPaths();

	SDL_free(writePath);

	return ret;
}

bool M_FileSystem::CleanUp()
{
	_LOG(LOG_INFO, "FileSystem: CleanUp.");
	return true;
}

bool M_FileSystem::Exist(const char * file) const
{
	return PHYSFS_exists(file);
}

bool M_FileSystem::MakeDir(const char * path, const char* mount) const
{
	bool ret = false;

	if (path)
	{
		char newDir[128];

		if (mount)
			sprintf_s(newDir, 128, "%s%s", mount, path);
		else
			sprintf_s(newDir, 128, path);

		if (PHYSFS_mkdir(newDir) == 0)
		{
			_LOG(LOG_ERROR, "Could not create dir: %s. Error: %s", newDir, PHYSFS_getLastError());
		}
		else
		{
			_LOG(LOG_INFO, "Just created a new dir: %s.", newDir);
			ret = true;
		}
	}

	return ret;
}

bool M_FileSystem::IsDirectory(const char * path) const
{
	return PHYSFS_isDirectory(path);
}

const char * M_FileSystem::GetBaseDir() const
{
	return PHYSFS_getBaseDir();
}

void M_FileSystem::DisplaySearchPaths() const
{
	for (char** i = PHYSFS_getSearchPath(); *i != NULL; ++i)
		_LOG(LOG_INFO, "[%s] is in search path.", *i);
}

int M_FileSystem::GetSearchPaths(std::vector<std::string>& paths)
{
	for (char** i = PHYSFS_getSearchPath(); *i != NULL; ++i)
		paths.push_back(*i);

	return paths.size();
}

uint M_FileSystem::Load(const char * file, char ** buffer) const
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fsFile, *buffer, 1, (PHYSFS_sint32)size);
			if (readed != size)
			{
				_LOG(LOG_ERROR, "Error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
			{
				ret = (uint)readed;
			}
		}

		if (PHYSFS_close(fsFile) == 0)
			_LOG(LOG_ERROR, "Error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
	{
		_LOG(LOG_ERROR, "Error while opening file %s: %s\n", file, PHYSFS_getLastError());
	}

	return ret;
}

SDL_RWops * M_FileSystem::Load(const char * file) const
{
	char* buffer = nullptr;
	int size = Load(file, &buffer);

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != nullptr)
			r->close = closeSDLRWops;

		return r;
	}
	else
	{
		return nullptr;
	}
}

uint M_FileSystem::Save(const char * file, const void * buffer, uint size) const
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openWrite(file);

	if (fsFile)
	{
		PHYSFS_sint64 written = PHYSFS_write(fsFile, (const void*)buffer, 1, size);
		if (written != size)
		{
			_LOG(LOG_ERROR, "Error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
		{
			ret = (uint)written;
		}

		if (PHYSFS_close(fsFile) == 0)
			_LOG(LOG_ERROR, "Error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
	{
		_LOG(LOG_ERROR, "Error while opening file %s: %s\n", file, PHYSFS_getLastError());
	}

	return ret;
}

uint M_FileSystem::GetFilesOnDir(const char * dir, std::vector<std::string>& files)const
{
	uint ret = 0;

	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** it = rc; *it != nullptr; ++it)
	{
		files.push_back(*it);
		++ret;
	}

	PHYSFS_freeList(rc);

	return ret;
}

uint M_FileSystem::GetFilesAndDirs(const char * dir, std::vector<std::string>& files, std::vector<std::string>& dirs)const
{
	uint ret = 0;

	std::string directory(dir);

	char** rc = PHYSFS_enumerateFiles(dir);
	for (char** it = rc; *it != nullptr; ++it)
	{
		if (IsDirectory((directory + *it).c_str()))
			dirs.push_back(*it);
		else
			files.push_back(*it);
		++ret;
	}

	PHYSFS_freeList(rc);

	return ret;
}

double M_FileSystem::GetLastModification(const char * file) const
{
	return PHYSFS_getLastModTime(file);
}

bool M_FileSystem::Destroy(std::string filename) const
{
	int ret;
	std::vector<std::string> directories, files;
	if (IsDirectory(filename.c_str()))
	{
		GetFilesAndDirs(filename.c_str(), files, directories);
		for (std::vector<std::string>::const_iterator directory = directories.begin(); directory != directories.end(); ++directory)
		{
			std::string directory_name = filename + *directory + "/";
			Destroy(directory_name);
		}

		for (std::vector<std::string>::const_iterator file = files.begin(); file != files.end(); ++file)
			PHYSFS_delete((filename + '/' + (*file)).data());
	}
	ret = PHYSFS_delete(filename.data());

	return (ret) ? true : false;
}

bool M_FileSystem::DuplicateFile(const char * src, const char * dst) const
{
	bool ret = false;

	if (!src || !dst)return ret;

	char* buffer = nullptr;
	int size = Load(src, &buffer);

	if (size > 0)
	{
		int succes = Save(dst, buffer, size);
		_LOG(LOG_INFO, "Fs: Duplicated file from src [%s] to dst [%s].", src, dst);
		ret = (succes == size);
	}
	else
	{
		_LOG(LOG_ERROR, "Could not duplicate file from src [%s] to dst [%s].", src, dst);
	}

	RELEASE_ARRAY(buffer);

	return ret;
}

void M_FileSystem::SplitPath(const char * originalPath, std::string * path, std::string * file, std::string * extension)
{
	if (!originalPath)
		return;

	std::string fullPath(originalPath);
	NormalizePath(fullPath);
	int lastSeparator = fullPath.find_last_of("\\/");
	int dot = fullPath.find_last_of(".");

	if (path)
	{
		if (lastSeparator < fullPath.length())
			*path = fullPath.substr(0, lastSeparator + 1);
		else
			path->clear();
	}

	if (file)
	{
		if (lastSeparator < fullPath.length())
			*file = fullPath.substr(lastSeparator + 1);
		else
			*file = fullPath;
	}

	if (extension)
	{
		if (dot < fullPath.length())
			*extension = fullPath.substr(dot + 1);
		else
			extension->clear();
	}
}

void M_FileSystem::NormalizePath(char * path)
{
	if (!path)
		return;

	int len = strlen(path);
	for (uint i = 0; i < len; ++i)
	{
		if (path[i] == '\\')
			path[i] = '/';
		else
			path[i] = tolower(path[i]);
	}
}

void M_FileSystem::NormalizePath(std::string & path)
{
	for (std::string::iterator it = path.begin(); it < path.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
		else
			*it = tolower(*it);
	}
}

bool M_FileSystem::AddPath(const char * pathOrZip, const char * mountPoint)
{
	bool ret = false;

	if (PHYSFS_mount(pathOrZip, mountPoint, 1) == 0)
	{
		_LOG(LOG_ERROR, "Error while adding a path or zip(%s): %s.", pathOrZip, PHYSFS_getLastError());
	}
	else
	{
		ret = true;
	}

	return ret;
}

int closeSDLRWops(SDL_RWops * rw)
{
	RELEASE_ARRAY(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}
