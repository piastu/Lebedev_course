#pragma once
#include "data_struct.h"
#include "Converter.h"
#include "ILIAS_Generator.h"
#include "MoodleXML.h"
#include "AST.h"
#include "ConvertTask.h"
#include "Inform_Expert.h"

public ref class Facade{
public:
	Inform_Expert ^IE;
	ConvertTasksPool ^CTPool;
	ConvertComponent ^CTTree;
	System::Windows::Forms::TreeView ^treeView_Tasks;

	Facade(){
		IE = gcnew Inform_Expert();
		CTPool = nullptr;
		CTTree = nullptr;
		treeView_Tasks = nullptr;
	}
	void SetCTPool(ConvertTasksPool ^CTP){
		CTPool = CTP;
	}
	void SetCTTree(ConvertComponent ^CC){
		CTTree = CC;
		IE->CTTree = CC;
	}
	void SetTVTasks(System::Windows::Forms::TreeView ^TVT){
		treeView_Tasks = TVT;
	}

	void DrawTasks(ConvertComponent ^CC,  System::Windows::Forms::TreeNodeCollection ^TNC){
		System::Windows::Forms::TreeNode ^TN = gcnew System::Windows::Forms::TreeNode();
		TN->Tag = CC;
		CC->Tag = TN;
		if (CC->GetTitle()->Length > 0){
			TN->Text = CC->GetTitle();
		}else{
			TN->Text = "";
		}

		if (dynamic_cast<ConvertTask ^>(CC)){
			TN->ImageKey = "ConvertTask";
			TN->SelectedImageKey = "ConvertTask";
		}else{
			TN->ImageKey = "ConvertGroup";
			TN->SelectedImageKey = "ConvertGroup";
		}

		if (CC->GetState() == ConvertComponent::StateSuccess){
			TN->BackColor = System::Drawing::Color::Lime;
		}else if (CC->GetState() == ConvertComponent::StateError){
			TN->BackColor = System::Drawing::Color::Red;
		}


		TNC->Add(TN);

		for (int i=0;i<CC->ChildCount();i++){
			DrawTasks(CC->GetChild(i), TN->Nodes);
		}
	 }
	void DrawTasks(){
		treeView_Tasks->Nodes->Clear();
		System::Windows::Forms::TreeNode ^TN = gcnew System::Windows::Forms::TreeNode();
		TN->Tag = CTTree;
		CTTree->Tag = TN;
		TN->Text = "Все задачи";
		TN->ImageKey = "ConvertGroup";
		TN->SelectedImageKey = "ConvertGroup";
		if (CTTree->GetState() == ConvertComponent::StateSuccess){
			TN->BackColor = System::Drawing::Color::Lime;
		}else if (CTTree->GetState() == ConvertComponent::StateError){
			TN->BackColor = System::Drawing::Color::Red;
		}
		treeView_Tasks->Nodes->Add(TN);
		for (int i=0;i<CTTree->ChildCount();i++){
			DrawTasks(CTTree->GetChild(i), treeView_Tasks->Nodes);
		}
		treeView_Tasks->ExpandAll();
		treeView_Tasks->Invalidate();
	}

	void AddConvertComponent(ConvertComponent ^CNew, ConvertComponent ^CParent){
		 CParent->AddChild(CNew);
		 System::Windows::Forms::TreeNodeCollection ^ParentNodes = ((System::Windows::Forms::TreeNode ^)CParent->Tag)->Nodes;
		 DrawTasks(CNew, ParentNodes);
		 treeView_Tasks->SelectedNode = (System::Windows::Forms::TreeNode ^)CNew->Tag;
	}
	void UpdateConvertTask(ConvertTask ^CNew, ConvertTask ^COld){
		 COld->CopyParams(CNew);
		 System::Windows::Forms::TreeNode ^TN = ((System::Windows::Forms::TreeNode ^)(COld->Tag));
		 TN->Text = COld->GetTitle();
		 TN->BackColor = System::Drawing::Color::Empty;
		 treeView_Tasks->SelectedNode = (System::Windows::Forms::TreeNode ^)COld->Tag;
	}
	void UpdateConvertGroup(ConvertGroup ^CNew, ConvertGroup ^COld){
		 COld->CopyParams(CNew);
		 System::Windows::Forms::TreeNode ^TN = ((System::Windows::Forms::TreeNode ^)(COld->Tag));
		 TN->Text = COld->GetTitle();
		 TN->BackColor = System::Drawing::Color::Empty;
		 treeView_Tasks->SelectedNode = (System::Windows::Forms::TreeNode ^)COld->Tag;
	}
	void DelConvertComponent(ConvertComponent ^CNew){
		ConvertComponent ^CParent = CNew->Parent;
		CParent->DelChild(CNew);
		System::Windows::Forms::TreeNodeCollection ^ParentNodes = ((System::Windows::Forms::TreeNode ^)CParent->Tag)->Nodes;
		ParentNodes->Remove((System::Windows::Forms::TreeNode ^)(CNew->Tag));
		treeView_Tasks->SelectedNode = (System::Windows::Forms::TreeNode ^)CParent->Tag;
	}

	void ShowState(ConvertComponent ^CC){
		System::Windows::Forms::TreeNode ^TN = (System::Windows::Forms::TreeNode ^)(CC->Tag);
		if (CC->GetState() == ConvertComponent::StateSuccess){
			TN->BackColor = System::Drawing::Color::Lime;
		}else if (CC->GetState() == ConvertComponent::StateError){
			TN->BackColor = System::Drawing::Color::Red;
		}
		for (int i=0;i<CC->ChildCount();i++){
			ShowState(CC->GetChild(i));
		}
	}

	void DoTasks(){
		 CTTree->Convert();
		 ShowState(CTTree);
	}

	ConvertComponent ^GetCTTree(){
		return CTTree;
	}
};

/*
#pragma once
#include "data_struct.h"
#include "Converter.h"
#include "ILIAS_Generator.h"
#include "MoodleXML.h"
#include "AST.h"
#include "ConvertTask.h"
#include "Inform_Expert.h"

public ref class Facade{
public:
	Inform_Expert ^IE;
	ConvertTasksPool ^CTPool;
	System::Collections::Generic::List<ConvertTask ^> ^CTList;
	System::Windows::Forms::ListView ^listView_Tasks;

	Facade(){
		IE = gcnew Inform_Expert();
		CTPool = nullptr;
		CTList = nullptr;
		listView_Tasks = nullptr;
	}
	void SetCTPool(ConvertTasksPool ^CTP){
		CTPool = CTP;
	}
	void SetCTList(System::Collections::Generic::List<ConvertTask ^> ^CTL){
		CTList = CTL;
		IE->CTList = CTL;
	}
	void SetLVTasks(System::Windows::Forms::ListView ^LVT){
		listView_Tasks = LVT;
	}

	void DrawTasks(){
		 listView_Tasks->Items->Clear();
		 for (int i=0;i<CTList->Count;i++){
			ConvertTask ^CT = CTList[i];

			System::String ^inputfilename = ".........................................";
			if (CT->InputFile->Length > 0) inputfilename = CT->InputFile;
			System::Windows::Forms::ListViewItem^ item = gcnew System::Windows::Forms::ListViewItem(inputfilename);
			if (CT->OutputFile->Length > 0){
				item->SubItems->Add(CT->OutputFile);
			}else{
				item->SubItems->Add(CT->OutputDir);
			}
			item->SubItems->Add(CT->C->ParserDelegate->GetName());
			item->SubItems->Add(CT->C->GeneratorDelegate->GetName());
			item->SubItems->Add(CT->Encod);
			if (CT->State == ConvertTask::StateSuccess){
				item->ImageKey = "StateSuccess";
			}
			if (CT->State == ConvertTask::StateError){
				item->ImageKey = "StateError";
			}
			item->Tag = CT;
			listView_Tasks->Items->Add(item);
		 }
		 listView_Tasks->Invalidate();
	 }
	void AddConvertTask(ConvertTask ^CT){
		 CTList->Add(CT);
		 DrawTasks();
	}
	void UpdateConvertTask(ConvertTask ^CT, int idx){
		 CTPool->Put((ConvertTask ^)CTList[idx]);
		 CTList[idx] = CT;
		 DrawTasks();
	}
	void DelConvertTask(ConvertTask ^CT){
		 CTPool->Put(CT);
		 CTList->Remove(CT);
		 DrawTasks();
	}
	void DelConvertTask(int idx){
		 DelConvertTask(CTList[idx]);
	}

	int GetNextCTidx(){
		 for (int i=0;i<CTList->Count;i++){
			 if (CTList[i]->State == ConvertTask::StateWaiting){
				 return i;
			 }
		 }
		 return -1;
	}
	void DoTasks(){
		 for (int i=0;i<CTList->Count;i++){
			 CTList[i]->State = ConvertTask::StateWaiting;
		 }
		 while(true){
			 int idx = GetNextCTidx();
			 if (idx == -1) return;
			 CTList[idx]->Convert();
		 }
		 DrawTasks();
	}

	void CTList_AddItem(ConvertTask ^CT){
		CTList->Add(CT);
	}
	ConvertTask ^CTList_GetItem(int idx){
		return CTList[idx];
	}
	void CTList_SetItem(int idx, ConvertTask ^CT){
		CTList[idx] = CT;
	}
};*/