#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "Globals.h"

class Resource;

class Importer
{
public:
	Importer()
	{}

	virtual ~Importer()
	{}

	virtual bool LoadResource(Resource* resource) = 0;
};

#endif // !__IMPORTER_H__
