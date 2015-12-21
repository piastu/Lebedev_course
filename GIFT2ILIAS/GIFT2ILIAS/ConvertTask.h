#pragma once
#include "data_struct.h"
#include "Converter.h"

public ref class ConvertComponent{
public:
	ConvertComponent ^Parent;
	System::Object ^Tag;

	static const int StateUndefined = -1;
	static const int StateWaiting = 0;
	static const int StateSuccess = 1;
	static const int StateError = 2;

	ConvertComponent(){
		Parent = nullptr;
		Tag = nullptr;
	}
	virtual void AddChild(ConvertComponent ^CC){
		throw;
	}
	virtual void DelChild(ConvertComponent ^CC){}
	virtual ConvertComponent ^GetChild(int i){return nullptr;}
	virtual int ChildCount(){return 0;}
	virtual void Convert(){}
	virtual int GetState(){
		return StateUndefined;
	}
	virtual System::String ^GetTitle(){
		return "";
	}
	virtual ConvertComponent ^GetParent(){
		return Parent;
	}
	virtual void SetParent(ConvertComponent ^P){
		Parent = P;
	}
};

public ref class ConvertTask: public ConvertComponent{
public:
	QF_Converter ^C;
	System::String ^InputFile;
	System::String ^OutputDir;
	System::String ^Encod;
	System::String ^PoolTitle;
	System::String ^PoolDescr;
	System::String ^OutputFile;

	int State;
	
	ConvertTask(): ConvertComponent(){
		Parent = nullptr;
		C = nullptr;
		State = StateWaiting;
		InputFile = "";
		OutputDir = "";
		Encod = "";
		PoolTitle = "";
		PoolDescr = "";
		OutputFile = "";
	}
	void CopyParams(ConvertTask ^CT){
		C = CT->C;
		State = CT->State;
		InputFile = CT->InputFile;
		OutputDir = CT->OutputDir;
		Encod = CT->Encod;
		PoolTitle = CT->PoolTitle;
		PoolDescr = CT->PoolDescr;
		OutputFile = CT->OutputFile;
	}
	virtual void Convert() override{
		try{
			OutputFile = C->Convert(InputFile,OutputDir,Encod,PoolTitle,PoolDescr);
			State = StateSuccess;
		}catch(System::Exception ^e){
			State = StateError;
		}
	}
	virtual int GetState() override{
		return State;
	}
	virtual System::String ^GetTitle() override{
		return PoolTitle;
	}
};

public ref class ConvertGroup: public ConvertComponent{
public:
	System::Collections::Generic::List<ConvertComponent ^> ^CTList;
	
	System::String ^Title;

	ConvertGroup(): ConvertComponent(){
		Parent = nullptr;
		CTList = gcnew System::Collections::Generic::List<ConvertComponent ^>();
		Title = "";
	};
	void CopyParams(ConvertGroup ^CG){
		Title = CG->Title;
	}
	virtual void AddChild(ConvertComponent ^CC) override{
		CTList->Add(CC);
		CC->SetParent(this);
	};
	virtual void DelChild(ConvertComponent ^CC) override{
		CTList->Remove(CC);
	};
	virtual void Convert() override{
		for (int i=0;i<CTList->Count;i++){
			if (CTList[i]->GetState() != ConvertComponent::StateSuccess){
				CTList[i]->Convert();
			}
		}
	};
	virtual int GetState() override{
		int SuccessCount = 0;
		for (int i=0;i<CTList->Count;i++){
			if (CTList[i]->GetState() == ConvertComponent::StateError) return ConvertComponent::StateError;
			if (CTList[i]->GetState() == ConvertComponent::StateSuccess) SuccessCount++;
		}
		if (SuccessCount > 0 && SuccessCount == CTList->Count) return ConvertComponent::StateSuccess;
		return ConvertComponent::StateWaiting;
	};
	virtual ConvertComponent ^GetChild(int i) override{
		if (i >= 0 && i < CTList->Count) return CTList[i];
		return nullptr;
	}
	virtual int ChildCount() override{
		return CTList->Count;
	}
	virtual System::String ^GetTitle() override{
		return Title;
	}
};

public ref class ConvertTasksPool{
protected:
	static const ConvertTasksPool ^self = nullptr;
	System::Collections::Generic::List<ConvertTask ^> ^CTList;
private:
	ConvertTasksPool(){
		CTList = gcnew System::Collections::Generic::List<ConvertTask ^>();
	};
public:
	static const ConvertTasksPool ^Instance(){
		if (self == nullptr) self = gcnew ConvertTasksPool();
		return self;
	};
	ConvertTask ^Get(){
		ConvertTask ^CT;
		if (CTList->Count == 0){
			CT = gcnew ConvertTask();
		}else{
			CT = CTList[0];
			CTList->RemoveAt(0);
		}
		return CT;
	}
	ConvertTask ^Get(QF_Converter ^C, System::String ^InputFile, System::String ^OutputDir, System::String ^Encod, System::String ^PoolTitle, System::String ^PoolDescr){
		ConvertTask ^CT;
		if (CTList->Count == 0){
			CT = gcnew ConvertTask();
		}else{
			CT = CTList[0];
			CTList->RemoveAt(0);
		}
		CT->C = C;
		CT->InputFile = InputFile;
		CT->OutputDir = OutputDir;
		CT->Encod = Encod;
		CT->PoolTitle = PoolTitle;
		CT->PoolDescr = PoolDescr;
		return CT;
	}
	void Put(ConvertTask ^C){
		C->OutputFile = "";
		C->State = ConvertComponent::StateWaiting;
		CTList->Add(C);
	}
};