#ifndef __M_FILESYSTEM_H__
#define __M_FILESYSTEM_H__

#include "Module.h"
#include <vector>
#include <string>

struct SDL_RWops;
int closeSDLRWops(SDL_RWops* rw);

class M_FileSystem : public Module
{
public:
	M_FileSystem(const char* name, bool startEnabled = true);
	virtual ~M_FileSystem();

	bool Init(JsonFile* file)override;
	bool CleanUp()override;

	bool AddPath(const char* pathOrZip, const char* mountPoint = nullptr);
	bool Exist(const char* file)const;
	bool MakeDir(const char* path, const char* mount = nullptr)const;
	bool IsDirectory(const char* path)const;

	const char* GetSaveDir()const { return "save/"; }

	const char* GetBaseDir()const;

	void DisplaySearchPaths()const;
	int GetSearchPaths(std::vector<std::string>& paths);

	uint Load(const char* file, char** buffer)const;
	SDL_RWops* Load(const char* file)const;

	uint Save(const char* file, const void* buffer, uint size)const;

	uint GetFilesOnDir(const char* dir, std::vector<std::string>& files)const;
	uint GetFilesAndDirs(const char* dir, std::vector<std::string>& files, std::vector<std::string>& dirs)const;

	double GetLastModification(const char* file)const;
	bool Destroy(std::string filename)const;
	bool DuplicateFile(const char* src, const char* dst)const;

	void SplitPath(const char* originalPath, std::string* path, std::string* file = nullptr, std::string* extension = nullptr);
	void NormalizePath(char* path);
	void NormalizePath(std::string& path);
};


#endif // !__M_FILESYSTEM_H__