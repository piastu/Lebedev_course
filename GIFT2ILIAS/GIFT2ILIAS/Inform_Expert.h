#pragma once
#include "data_struct.h"
#include "Converter.h"
#include "ILIAS_Generator.h"
#include "MoodleXML.h"
#include "AST.h"
#include "ConvertTask.h"

public ref class Inform_Expert{
public:
	ConvertComponent ^CTTree;

	Inform_Expert(){
		CTTree = nullptr;
	}
	Inform_Expert(ConvertComponent ^CC){
		CTTree = CC;
	}
	
	int WaitingCount(ConvertComponent ^CC){
		if (dynamic_cast<ConvertTask ^>(CC) == nullptr){
			int sum = 0;
			for (int i=0;i<CC->ChildCount();i++){
				sum += WaitingCount(CC->GetChild(i));
			}
			return sum;
		}
		if (CC->GetState() == ConvertComponent::StateWaiting) return 1;
		return 0;
	}
	int WaitingCount(){
		return WaitingCount(CTTree);
	}

	int SuccessCount(ConvertComponent ^CC){
		if (dynamic_cast<ConvertTask ^>(CC) == nullptr){
			int sum = 0;
			for (int i=0;i<CC->ChildCount();i++){
				sum += SuccessCount(CC->GetChild(i));
			}
			return sum;
		}
		if (CC->GetState() == ConvertComponent::StateSuccess) return 1;
		return 0;
	}
	int SuccessCount(){
		return SuccessCount(CTTree);
	}

	int ErrorCount(ConvertComponent ^CC){
		if (dynamic_cast<ConvertTask ^>(CC) == nullptr){
			int sum = 0;
			for (int i=0;i<CC->ChildCount();i++){
				sum += ErrorCount(CC->GetChild(i));
			}
			return sum;
		}
		if (CC->GetState() == ConvertComponent::StateError) return 1;
		return 0;
	}
	int ErrorCount(){
		return ErrorCount(CTTree);
	}

	int TotalCount(ConvertComponent ^CC){
		if (dynamic_cast<ConvertTask ^>(CC) == nullptr){
			int sum = 0;
			for (int i=0;i<CC->ChildCount();i++){
				sum += TotalCount(CC->GetChild(i));
			}
			return sum;
		}
		return 1;
	}
	int TotalCount(){
		return TotalCount(CTTree);
	}
};