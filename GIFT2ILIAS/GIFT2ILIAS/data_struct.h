#pragma once
ref class Data_Pool;
ref class Data_Question;
ref class Data_Answer;

#define TYPE_SINGLE 0
#define TYPE_MULTIPLE 1
#define TYPE_TEXTBOX 2
#define TYPE_ORDERING 3
#define TYPE_MATCHING 4

public ref class Data_Answer{
public:
	int an_id;
	System::String ^Text;
	System::String ^Feedback;
	int Add_Yes;
	int Add_No;
	int order;
	int match_grp;

	Data_Answer(){
		an_id = -1;
		Text = "";
		Feedback = "";
		Add_Yes = 0;
		Add_No = 0;
		order = -1;
	}
};
public ref class Data_Question{
public:
	int quest_id;

	int an_type; // 0 - один ответ, 1 - несколько ответов, 2 - ввод ответа, 3 - упорядочивание, 4 - соответствие 1:1
	int time_day, time_month, time_year, time_hour, time_min, time_sec;
	int maxattempts;
	int feedback_setting;
	bool shuffle;

	int temp_weight;

	int plus_count;

	System::String ^Title;
	System::String ^Text;
	System::String ^Author;
	System::Collections::Generic::List<Data_Answer ^> ^Answers;

	Data_Question(){
		quest_id = -1;
		an_type = 0;
		time_day=0;time_month=0;time_year=0;time_hour=0;time_min=1;time_sec=0;
		maxattempts=0;
		feedback_setting=1;
		shuffle=true;
		Title="";
		Text="";
		Author="";
		Answers = gcnew System::Collections::Generic::List<Data_Answer ^>();
		temp_weight=100;
		plus_count = 0;
	}
	void Add_Answer(System::String ^AnText, System::String ^AnFeedback, int Add_Yes){
		Add_Answer(AnText,AnFeedback,Add_Yes,0);
	}
	void Add_Answer(System::String ^AnText, System::String ^AnFeedback, int Add_Yes, int Add_No){
		Data_Answer ^AnAdd = gcnew Data_Answer();
		AnAdd->an_id = Answers->Count;
		AnAdd->Text = AnText;
		AnAdd->Feedback = AnFeedback;
		AnAdd->Add_Yes = Add_Yes;
		AnAdd->Add_No = Add_No;
		Answers->Add(AnAdd);
	}
};
public ref class Data_Pool{
public:
	int pool_id;
	System::String ^Title;
	System::String ^Language;
	System::String ^Description;
	System::Collections::Generic::List<System::String ^> ^Keywords;
	System::Collections::Generic::List<Data_Question ^> ^Questions;

	Data_Pool(){
		pool_id = -1;
		Title = "";
		Language = "ru";
		Description = "";
		Keywords = gcnew System::Collections::Generic::List<System::String ^>();
		Questions = gcnew System::Collections::Generic::List<Data_Question ^>();
	}
};
