#include "Path.h"

using namespace std;

Path::Path() : file(""), fileName(""), extension(""), folders(""), fullPath("")
{
}

Path::Path(const char * fullPath) : fullPath(fullPath)
{
	SplitPath();
}

Path::Path(const char * folders, const char * file) : folders(folders), file(file)
{
	fullPath = this->folders + this->file;
	SplitPath();
}

Path::Path(Path & copy) : fullPath(copy.fullPath)
{
	SplitPath();
}


Path::~Path()
{
}

void Path::SplitPath()
{
	Normalize();
	int lastSeparator = fullPath.find_last_of("\\/");
	int dot = fullPath.find_last_of(".");

	if (lastSeparator < fullPath.length())
	{
		folders = fullPath.substr(0, lastSeparator + 1);
		file = fullPath.substr(lastSeparator + 1);
		fileName = fullPath.substr(lastSeparator + 1, fullPath.size() - dot + 2);
	}
	else
	{
		folders.clear();
		file.clear();
		fileName.clear();
	}

	if (dot < fullPath.length())
		extension = fullPath.substr(dot + 1);
	else
		extension.clear();
}

void Path::MountPath()
{
	fullPath = folders + fileName + "." + extension;
	Normalize();
}

void Path::SetFileNameFromFile()
{
	int lastSeparator = fullPath.find_last_of("\\/");
	int dot = fullPath.find_last_of(".");
	fileName = fullPath.substr(lastSeparator + 1, fullPath.size() - dot + 2);
}

int Path::GetFolders(std::vector<std::string>& _folders)
{
	for (int i = 0, start = 0; i < folders.size(); ++i)
	{
		if (folders[i] == '/')
		{
			_folders.push_back(folders.substr(start, i - start + 1));
			start = i + 1;
		}
	}

	return _folders.size();
}

void Path::Normalize()
{
	for (string::iterator it = fullPath.begin(); it != fullPath.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
		//else
		//	*it = tolower(*it);
	}

	for (string::iterator it = folders.begin(); it != folders.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
		//else
		//	*it = tolower(*it);
	}
}

void Path::SetFileName(const char * _fileName)
{
	fileName = _fileName;
	MountPath();
}

void Path::SetFile(const char * _file)
{
	file = _file;
	int dot = file.find_last_of(".");
	fileName = file.substr(0, dot);
	extension = file.substr(dot + 1);
	MountPath();
}

void Path::SetExtension(const char * ext)
{
	extension = ext;
	MountPath();
}

void Path::SetFolders(const char * _folders)
{
	folders = _folders;
	MountPath();
}

void Path::SetFullPath(const char * _fullPath)
{
	fullPath = _fullPath;
	SplitPath();
}

void Path::Set(const char * folders, const char * fileName, const char * extension)
{
	this->folders = folders;
	this->fileName = fileName;
	this->extension = extension;
	file = fileName + string(".") + extension;
	MountPath();
}

void Path::InsertFolderAfter(const char * folder, const char * after)
{
	int insertPos = folders.find(after) + strlen(after);
	if (insertPos <= folders.size())
	{
		folders.insert(insertPos, folder);
		MountPath();
	}
}

int Path::CountFolders()
{
	int ret = 0;

	for (int i = 0, start = 0; i < folders.size(); ++i)
	{
		if (folders[i] == '/')
		{
			++ret;
			start = i + 1;
		}
	}

	return ret;
}

bool Path::Empty() const
{
	return fullPath.empty();
}

bool Path::EmptyFile() const
{
	return file.empty();
}

bool Path::EmptyFolder() const
{
	return folders.empty();
}

bool Path::IsFileLocatedInFolder(std::string folder)
{
	if (folders.size() >= folder.size())
	{
		int i = 0;
		for (string::iterator it = folder.begin(); it != folder.end() && i < folders.size(); ++it)
		{
			if (*it != folders[i])
				return false;
			++i;
		}
	}

	return true;
}
