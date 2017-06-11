#ifndef __PATH_H__
#define __PATH_H__

#include <string>
#include <vector>

class Path
{
public:
	Path();
	Path(const char* fullPath);
	Path(const char* folders, const char* file);
	Path(Path& copy);

	virtual ~Path();

	const char* GetFileName()const { return fileName.c_str(); }
	const char* GetExtension()const { return extension.c_str(); }
	const char* GetFile()const { return file.c_str(); }
	const char* GetPathToFile()const { return folders.c_str(); }
	const char* GetFullPath()const { return fullPath.c_str(); }
	
	int GetFolders(std::vector<std::string>& _folders);

	void SetFileName(const char* _fileName);
	void SetFile(const char* _file);
	void SetExtension(const char* ext);
	void SetFolders(const char* _folders);
	void SetFullPath(const char* _fullPath);

	void Normalize();
	void InsertFolderAfter(const char* folder, const char* after);
	int CountFolders();

private:
	void SplitPath();
	void MountPath();
	
public:

private:
	std::string file;
	std::string fileName;
	std::string extension;
	std::string folders;

	std::string fullPath;

};

#endif // !__PATH_H__