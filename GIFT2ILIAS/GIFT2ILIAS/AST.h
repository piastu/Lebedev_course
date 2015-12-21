#pragma once
#include "data_struct.h"
#include "Parser.h"

using namespace System::IO;
using namespace System::IO::Packaging;
using namespace DocumentFormat::OpenXml::Packaging;
using namespace DocumentFormat::OpenXml::Wordprocessing;



public ref class AST_Parser:
	public Parser{
		System::String ^md5hash(System::String ^input){
			System::Security::Cryptography::MD5 ^md5Hash = System::Security::Cryptography::MD5::Create();
			array<unsigned char>^ data = md5Hash->ComputeHash(System::Text::Encoding::UTF8->GetBytes(input));
			System::Text::StringBuilder ^sBuilder = gcnew System::Text::StringBuilder();
			for (int i = 0; i < data->Length; i++)
			{
				sBuilder->Append(data[i].ToString("x2"));
			}
			return sBuilder->ToString();
		}
	AST_Parser(): Parser(){
		MaxQuestion = 0;
		Q = gcnew System::Collections::Generic::List<Data_Question ^>();
		problem = false;
	}
protected:
	static AST_Parser ^self = nullptr;
public:
	int MaxQuestion;
	System::Collections::Generic::List<Data_Question ^> ^Q;
	bool problem;

	virtual System::Collections::Generic::List<Data_Question ^> ^GetQuestions() override{
		return Q;
	};
	virtual bool Problem() override{
		return problem;
	};
	virtual void Parse(System::String ^Input, System::Text::Encoding ^Encod) override{
		Package ^wordPackage = Package::Open(Input, FileMode::Open, FileAccess::Read);
		WordprocessingDocument ^wordDocument = WordprocessingDocument::Open(wordPackage);
		Body ^body = wordDocument->MainDocumentPart->Document->Body;

		int P = -1;
		Data_Question ^QQ = nullptr;
		int plus_count = 0;
		int minus_count = 0;
		bool order_flag = false;
		bool match_flag = false;

		for (int i=0;i<body->ChildElements->Count;i++){
			if (body->ChildElements[i]->GetType()->ToString() != "DocumentFormat.OpenXml.Wordprocessing.Paragraph") continue; 
			Paragraph ^p = (Paragraph ^)body->ChildElements[i]; 
			System::String ^Line = p->InnerText;//NC[i]->GetText();

			if (Line->IndexOf("Документ подготовлен системой обработки банков") >= 0) continue;
			if (Line->IndexOf("Независимый центр тестирования качества обучения") >= 0) continue;
			if (Line->Length == 12 && Line->IndexOf("PAGE") == 2 && Line->IndexOf("17") == 8) continue;
			if (Line->IndexOf("I:") == 0){
				P = 0;
				if (QQ != nullptr){
					if (order_flag){
						QQ->an_type = TYPE_ORDERING;
					}else if (match_flag){
						QQ->an_type = TYPE_MATCHING;
					}else if (plus_count == 1){
						QQ->an_type = TYPE_SINGLE;
					}else if (minus_count > 0){
						QQ->an_type = TYPE_MULTIPLE;
					}else if (minus_count == 0){
						QQ->an_type = TYPE_TEXTBOX;
					}
					QQ->plus_count = plus_count;
					Q->Add(QQ);
				}
				QQ = gcnew Data_Question();
				plus_count=0;
				minus_count=0;
				order_flag=false;
				match_flag = false;

				System::Text::RegularExpressions::Match ^TimeMatch = System::Text::RegularExpressions::Regex::Match(Line,"(T|t|Т|т)=(\\d+);");
				System::String ^TimeRes = TimeMatch->Result("$2");
				int TimeInt = System::Convert::ToInt32(TimeRes);
				QQ->time_sec = TimeInt%60;
				QQ->time_min = (TimeInt/60)%60;
				QQ->time_hour = (TimeInt/3600)%24;
				
				System::Text::RegularExpressions::Match ^EkMatch = System::Text::RegularExpressions::Regex::Match(Line,"(Ek|ek|eK|EK)=(\\d+);");
				System::String ^EkRes = EkMatch->Result("$2");
				int EkInt = System::Convert::ToInt32(EkRes);
				if (EkInt > 0){
					QQ->temp_weight = EkInt;
				}

			}else if (Line->IndexOf("S:") == 0){
				P = 1;
				if (QQ != nullptr){
					QQ->Title = Line->Substring(2);
					QQ->Text = Line->Substring(2);
				}
			}else if (Line->IndexOf("+:") == 0){
				P = 2;
				if (QQ != nullptr){
					QQ->Add_Answer(Line->Substring(2), "", 100);
					plus_count++;
				}
			}else if (Line->IndexOf("-:") == 0){
				P = 2;
				if (QQ != nullptr){
					QQ->Add_Answer(Line->Substring(2), "", 0);
					minus_count++;
				}
			}else if (System::Text::RegularExpressions::Regex::IsMatch(Line,"^[0-9]:")){
				P = 2;
				if (QQ != nullptr){
					order_flag = true;
					QQ->Add_Answer(Line->Substring(Line->IndexOf(":")+1), "", 100);
					QQ->Answers[QQ->Answers->Count-1]->order = System::Convert::ToInt32(Line->Substring(0,Line->IndexOf(":")));
					QQ->Answers[QQ->Answers->Count-1]->order--;
				}
			}else if (System::Text::RegularExpressions::Regex::IsMatch(Line,"^L[0-9]:")){ // match_grp = 0
				P = 2;
				if (QQ != nullptr){
					match_flag = true;
					QQ->Add_Answer(Line->Substring(Line->IndexOf(":")+1), "", 100);
					QQ->Answers[QQ->Answers->Count-1]->order = System::Convert::ToInt32(Line->Substring(1,Line->IndexOf(":")-1));
					QQ->Answers[QQ->Answers->Count-1]->match_grp = 0;
				}
			}else if (System::Text::RegularExpressions::Regex::IsMatch(Line,"^R[0-9]:")){ // match_grp = 1
				P = 2;
				if (QQ != nullptr){
					match_flag = true;
					QQ->Add_Answer(Line->Substring(Line->IndexOf(":")+1), "", 100);
					QQ->Answers[QQ->Answers->Count-1]->order = System::Convert::ToInt32(Line->Substring(1,Line->IndexOf(":")-1));
					QQ->Answers[QQ->Answers->Count-1]->match_grp = 1;
				}
			}else{
				if (P == 1){
					if (QQ != nullptr){
						QQ->Title += Line;
						QQ->Text += Line;
					}
				}
				if (P == 2){
					if (QQ != nullptr){
						if (QQ->Answers->Count > 0){
							QQ->Answers[QQ->Answers->Count-1]->Text += Line;
						}
					}
				}
			}
		}
		if (QQ != nullptr){
			if (order_flag){
				QQ->an_type = TYPE_ORDERING;
			}else if (match_flag){
				QQ->an_type = TYPE_MATCHING;
			}else if (plus_count == 1){
				QQ->an_type = TYPE_SINGLE;
			}else if (minus_count > 0){
				QQ->an_type = TYPE_MULTIPLE;
			}else if (minus_count == 0){
				QQ->an_type = TYPE_TEXTBOX;
			}
			QQ->plus_count = plus_count;
			Q->Add(QQ);
		}

		for (int i=0;i<Q->Count;i++){
			Q[i]->Title = Q[i]->Title->Trim(' ', '\r', '\n');
			Q[i]->Text = Q[i]->Text->Trim(' ', '\r', '\n');
			if (Q[i]->Title == "") Q[i]->Title = Q[i]->Text; 
			for (int j=0;j<Q[i]->Answers->Count;j++){
				Q[i]->Answers[j]->Text = Q[i]->Answers[j]->Text->Trim(' ', '\r', '\n');
				Q[i]->Answers[j]->Feedback = Q[i]->Answers[j]->Feedback->Trim(' ', '\r', '\n');

				if (Q[i]->Answers[j]->Text == ""){
					Q[i]->Answers->Remove(Q[i]->Answers[j]);
					j--;
					continue;
				}

				if (Q[i]->an_type == TYPE_TEXTBOX){
					Q[i]->Answers[j]->Add_Yes = Q[i]->temp_weight;
				}
				if ((Q[i]->an_type == TYPE_SINGLE || Q[i]->an_type == TYPE_MULTIPLE) && Q[i]->Answers[j]->Add_Yes > 0){
					Q[i]->Answers[j]->Add_Yes = (Q[i]->temp_weight)*(1.0/(Q[i]->plus_count));
				}
				if (Q[i]->an_type == TYPE_MULTIPLE && Q[i]->Answers[j]->Add_Yes == 0){
					Q[i]->Answers[j]->Add_Yes = -(Q[i]->temp_weight);
				}
				if (Q[i]->an_type == TYPE_ORDERING){
					Q[i]->Answers[j]->Add_Yes = (Q[i]->temp_weight)*(1.0/(Q[i]->Answers->Count));
				}
				if (Q[i]->an_type == TYPE_MATCHING){
					Q[i]->Answers[j]->Add_Yes = (Q[i]->temp_weight)*(0.5/(Q[i]->Answers->Count));
				}
			}
		}
		for (int i=0;i<Q->Count;i++){
			Q[i]->quest_id = i+1;
		}
		
	}
	static const AST_Parser ^Instance(){
		if (self == nullptr) self = gcnew AST_Parser();
		return self;
	}
	virtual System::String ^GetName() override{
		return "AST";
	};
};