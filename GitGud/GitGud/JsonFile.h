#ifndef __JSONFILE_H__
#define __JSONFILE_H__

#include "jsoncpp/json.h"
#include "MathGeoLib/include/Math/MathAll.h"
#include "Color.h"

typedef long long int64;
typedef unsigned long long uint64;

class JsonFile
{
public:
	JsonFile();
	JsonFile(const char* buffer);
	JsonFile(Json::Value& section);
	virtual ~JsonFile();

	// -----------------------------------------------------

	int GetArraySize(const char* name);
	Json::Value Value()const { return objRoot; }

	// -----------------------------------------------------

	JsonFile GetSection(const char* sectionName);

	int GetInt(const char* name, int def, int index = -1);
	unsigned int GetUInt(const char* name, unsigned int def, int index = -1);
	int64 GetInt64(const char* name, int64 def, int index = -1);
	uint64 GetUInt64(const char* name, uint64 def, int index = -1);
	float GetFloat(const char* name, float def, int index = -1);
	double GetDouble(const char* name, double def, int index = -1);
	bool GetBool(const char* name, bool def, int index = -1);
	std::string GetString(const char* name, std::string def, int index = -1);

	float3 GetFloat3(const char* name, float3 def);
	Quat GetQuaternion(const char* name, Quat def);
	Color GetColor(const char* name, Color def);

	JsonFile GetObjectFromArray(const char* name, unsigned int index);

	// ======

	void AddSection(const char* name, JsonFile& section);

	void AddInt(const char* name, int value);
	void AddIntArray(const char* name, int* data, unsigned int size);
	void AddUInt(const char* name, unsigned int value);
	void AddUIntArray(const char* name, unsigned int* data, unsigned int size);
	void AddInt64(const char* name, int64 value);
	void AddInt64Array(const char* name, int64* data, unsigned int size);
	void AddUInt64(const char* name, uint64 value);
	void AddUIntArray(const char* name, uint64* data, unsigned int size);
	void AddFloat(const char* name, float value);
	void AddFloatArray(const char* name, float* data, unsigned int size);
	void AddDouble(const char* name, double value);
	void AddDoubleArray(const char* name, double* data, unsigned int size);
	void AddBool(const char* name, bool value);
	void AddBoolArray(const char* name, bool* data, unsigned int size);
	void AddString(const char* name, const char* value);
	void AddCStringArray(const char* name, const char** data, unsigned int size);
	void AddString(const char* name, std::string value);
	void AddStringArray(const char* name, std::string* data, unsigned int size);

	void AddValue(const char* name, Json::Value value);
	void AddArrayValue(const char* name, Json::Value value, unsigned int index);
	void AppendArrayValue(const char* name, Json::Value value);

	void AddFloat3(const char* name, float3 value);
	void AddQuaternion(const char* name, Quat value);
	void AddColor(const char* name, Color value);

	// -----------------------------------------------------

	std::string Write(bool styled = false);


private:
	void WriteFast(std::string&)const;
	void WriteStyled(std::string&)const;

private:
	Json::Value objRoot = Json::Value::null;
};

#endif