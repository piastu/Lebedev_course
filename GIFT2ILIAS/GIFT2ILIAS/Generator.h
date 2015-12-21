#pragma once
#include "data_struct.h"

public ref class Generator{
public:
	Generator(){};
	virtual System::String ^Generate(Data_Pool ^NewPool,System::String ^OutputDir){return "";};
	virtual System::String ^GetName(){return "";};
};