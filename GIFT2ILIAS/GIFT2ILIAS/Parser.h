#pragma once
#include "data_struct.h"

public ref class Parser{
public:
	Parser(){};
	virtual void Parse(System::String ^Input, System::Text::Encoding ^Encod){};
	virtual System::Collections::Generic::List<Data_Question ^> ^GetQuestions(){return nullptr;};
	virtual bool Problem(){return false;};
	virtual System::String ^GetName(){return "";};
};