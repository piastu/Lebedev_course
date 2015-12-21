#pragma once
#include "data_struct.h"
#include "Parser.h"
#include "Generator.h"

public ref class QF_Converter{
public:
	Parser ^ParserDelegate;
	Generator ^GeneratorDelegate;

	QF_Converter(){
		ParserDelegate = nullptr;
		GeneratorDelegate = nullptr;
	}
	void SetParser(Parser ^P){
		ParserDelegate = P;
	}
	void SetGenerator(Generator ^G){
		GeneratorDelegate = G;
	}
	System::String ^Convert(System::String ^InputFile, System::String ^OutputDir, System::String ^Encod, System::String ^PoolTitle, System::String ^PoolDescr){
		ParserDelegate->Parse(InputFile,System::Text::Encoding::GetEncoding(Encod));

		Data_Pool ^NewPool = gcnew Data_Pool();
		NewPool->pool_id = 1;
		NewPool->Title = PoolTitle;
		NewPool->Description = PoolDescr;

		for (int i=0;i<ParserDelegate->GetQuestions()->Count;i++){
			NewPool->Questions->Add(ParserDelegate->GetQuestions()[i]);
		}

		return GeneratorDelegate->Generate(NewPool,OutputDir);
	}
};