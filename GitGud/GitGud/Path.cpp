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

Path::Path(Path & copy)
{
	fullPath = copy.fullPath;
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
		fileName = fullPath.substr(lastSeparator + 1, dot);
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

void Path::Normalize()
{
	for (string::iterator it = fullPath.begin(); it != fullPath.end(); ++it)
	{
		if (*it == '\\')
			*it = '/';
		else
			*it = tolower(*it);
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
