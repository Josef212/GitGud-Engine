#include "JsonFile.h"

#include "parson-master\parson.h"

JsonFile::JsonFile()
{
	valRoot = json_value_init_object();
	if (valRoot)
	{
		objRoot = json_value_get_object(valRoot);
		clean = true;
	}
}

JsonFile::JsonFile(const char* buffer)
{
	if (buffer)
	{
		valRoot = json_parse_string(buffer);
		if (valRoot)
		{
			objRoot = json_value_get_object(valRoot);
			clean = true;
		}
	}
}

JsonFile::JsonFile(JSON_Object* section) : objRoot(section)
{
}

JsonFile::~JsonFile()
{
	if (clean && valRoot)
		json_value_free(valRoot);
}

//------------------------------------------------
//---------------Getters--------------------------
//------------------------------------------------

JSON_Value* JsonFile::GetVal(const char* valName, int index)
{
	if (index < 0)
		return json_object_get_value(objRoot, valName);
	else
	{
		JSON_Array* ary = json_object_get_array(objRoot, valName);
		return (ary) ? (json_array_get_value(ary, index)) : (nullptr);
	}
}

JsonFile JsonFile::GetSection(const char* sectionName)
{
	//assert(sectionName);
	return JsonFile(json_object_get_object(objRoot, sectionName));
}

const char* JsonFile::GetString(const char* name, const char* defaultStr, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? (json_value_get_string(val)) : (defaultStr);
}

bool JsonFile::GetBool(const char* name, bool defaultBool, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? (json_value_get_boolean(val)) : (defaultBool);
}

int JsonFile::GetInt(const char* name, int defaultInt, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? ((int)json_value_get_number(val)) : (defaultInt);
}

float JsonFile::GetFloat(const char* name, float defaultFloat, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? ((float)json_value_get_number(val)) : (defaultFloat);
}

double JsonFile::GetDouble(const char* name, double defaultDouble, int index)
{
	JSON_Value* val = GetVal(name, index);
	return (val) ? ((double)json_value_get_number(val)) : (defaultDouble);
}

float* JsonFile::GetFloatArray(const char* name)
{
	//For now we can get a float or any value type from array using the float getter + index
	return nullptr;
}

JsonFile JsonFile::GetArray(const char* name, int index)const
{
	JSON_Array* array = json_object_get_array(objRoot, name);
	if (array)
		return JsonFile(json_array_get_object(array, index));
	return JsonFile((JSON_Object*)nullptr);
}

int JsonFile::GetArraySize(const char* name)const
{
	int ret = -1;
	JSON_Array* array = json_object_get_array(objRoot, name);
	if (array)
		ret = json_array_get_count(array);
	return ret;
}

float3 JsonFile::GetFloat3(const char* name, float3 default)
{
	return float3(GetFloat(name, default.x, 0), GetFloat(name, default.y, 1), GetFloat(name, default.z, 2));
}

/*Color JsonFile::GetColor(const char* name, Color default)
{
	return Color(GetFloat(name, default.r, 0),
		GetFloat(name, default.g, 1),
		GetFloat(name, default.b, 2),
		GetFloat(name, default.a, 3));
}*/

//--------------------------------------

//------------------------------------------------
//---------------Setters--------------------------
//------------------------------------------------

JsonFile JsonFile::AddSection(const char* sectionName)
{
	json_object_set_value(objRoot, sectionName, json_value_init_object());
	return GetSection(sectionName);
}

bool JsonFile::AddString(const char* name, const char* value)
{
	return json_object_set_string(objRoot, name, value) == JSONSuccess;
}

bool JsonFile::AddBool(const char* name, bool value)
{
	return json_object_set_boolean(objRoot, name, value) == JSONSuccess;
}

bool JsonFile::AddInt(const char* name, int value)
{
	return json_object_set_number(objRoot, name, (double)value) == JSONSuccess;
}

bool JsonFile::AddFloat(const char* name, float value)
{
	return json_object_set_number(objRoot, name, (double)value) == JSONSuccess;
}

bool JsonFile::AddDouble(const char* name, double value)
{
	return json_object_set_number(objRoot, name, value) == JSONSuccess;
}

bool JsonFile::AddArray(const char* name)
{
	JSON_Value* val = json_value_init_array();
	array = json_value_get_array(val);

	return json_object_set_value(objRoot, name, val) == JSONSuccess;
}

bool JsonFile::AddArrayEntry(const JsonFile& file)
{
	if (array)
		return json_array_append_value(array, json_value_deep_copy(file.valRoot)) == JSONSuccess;
	return false;
}

bool JsonFile::AddIntArray(const char* name, int* iArray, uint size)
{
	if (!name || !iArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_number(ar, iArray[i]);

	json_object_set_value(objRoot, name, val);

	return true;
}

bool JsonFile::AddFloatArray(const char* name, float* fArray, uint size)
{
	if (!name || !fArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_number(ar, fArray[i]);

	json_object_set_value(objRoot, name, val);

	return true;
}

bool JsonFile::AddFloatArray(const char* name, const float* fArray, uint size)
{
	if (!name || !fArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_number(ar, fArray[i]);

	json_object_set_value(objRoot, name, val);

	return true;
}

bool JsonFile::AddBoolArray(const char* name, bool* bArray, uint size)
{
	if (!name || !bArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_boolean(ar, bArray[i]);

	json_object_set_value(objRoot, name, val);

	return true;
}

bool JsonFile::AddStringArray(const char* name, const char** sArray, uint size)
{
	if (!name || !sArray || size <= 0)
		return false;

	JSON_Value* val = json_value_init_array();
	JSON_Array* ar = json_value_get_array(val);
	for (uint i = 0; i < size; ++i)
		json_array_append_string(ar, sArray[i]);

	json_object_set_value(objRoot, name, val);

	return true;
}

bool JsonFile::AddFloat3(const char* name, float3 vec)
{
	return AddFloatArray(name, vec.ptr(), 3);
}

/*bool JsonFile::AddColor(const char* name, Color col)
{
	return AddFloatArray(name, &col, 4);
}*/

//--------------------------------------

uint JsonFile::WriteJson(char** buffer, bool fastMode)
{
	return (fastMode) ? (WriteFast(buffer)) : (WriteStyled(buffer));
}

uint JsonFile::WriteFast(char** buffer)
{
	uint ret = json_serialization_size(valRoot);
	*buffer = new char[ret];
	json_serialize_to_buffer(valRoot, *buffer, ret);

	return ret;
}

uint JsonFile::WriteStyled(char** buffer)
{
	uint ret = json_serialization_size_pretty(valRoot);
	*buffer = new char[ret];
	json_serialize_to_buffer_pretty(valRoot, *buffer, ret);

	return ret;
}