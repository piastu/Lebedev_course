#pragma once
#include "data_struct.h"
#include "Parser.h"

public ref class MoodleXML_Parser:
	public Parser{
	MoodleXML_Parser(): Parser(){
		MaxQuestion = 0;
		Q = gcnew System::Collections::Generic::List<Data_Question ^>();
		problem = false;
	}
protected:
	static MoodleXML_Parser ^self = nullptr;
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
	virtual void Parse(System::String ^InputFile, System::Text::Encoding ^Encod) override{
		System::IO::Stream ^Input = gcnew System::IO::FileStream(InputFile, System::IO::FileMode::Open);
		System::IO::StreamReader ^S = gcnew System::IO::StreamReader(Input, Encod);

		System::Xml::XmlDocument ^XmlD = gcnew System::Xml::XmlDocument();
		try{
			XmlD->LoadXml(S->ReadToEnd());
			S->Close();
		}catch(System::Exception ^e){
			S->Close();
			throw;
		}

		System::Xml::XmlNode ^quiz = XmlD["quiz"];

		if (quiz->HasChildNodes){
			for (int i=0;i<quiz->ChildNodes->Count;i++){
				System::Xml::XmlNode ^question = quiz->ChildNodes[i];
				System::Xml::XmlAttributeCollection ^qattr = question->Attributes;

				System::String ^qtype = "";
				try{
					qtype = qattr["type"]->Value;
				}catch(System::Exception ^e){}

				if (qtype == "multichoice"){
					System::String ^qtitle = "";
					try{qtitle = (question["name"])["text"]->InnerText;}catch(System::Exception ^e){}
					System::String ^qtext = "";
					try{qtext = (question["questiontext"])["text"]->InnerText;}catch(System::Exception ^e){}
					System::String ^qsingle = "true";
					try{qsingle = question["single"]->InnerText;}catch(System::Exception ^e){}
					System::String ^qshuffle = "1";
					try{qshuffle = question["shuffleanswers"]->InnerText;}catch(System::Exception ^e){}

					Data_Question ^QQ = gcnew Data_Question();
					QQ->Title = qtitle;
					QQ->Text = qtext;
					if (qshuffle == "1") QQ->shuffle = true;
					else QQ->shuffle = false;
					if (qsingle == "true") QQ->an_type = TYPE_SINGLE;
					else QQ->an_type = TYPE_MULTIPLE;

					for (int j=0;j<question->ChildNodes->Count;j++){
						if (question->ChildNodes[j]->Name != "answer") continue;
						System::Xml::XmlNode ^answer = question->ChildNodes[j];
						System::String ^anfract = "";
						try{anfract = answer->Attributes["fraction"]->Value;}catch(System::Exception ^e){}
						System::String ^antext = "";
						try{antext = answer["text"]->InnerText;}catch(System::Exception ^e){}
						System::String ^anfb = "";
						try{anfb = (answer["feedback"])["text"]->InnerText;}catch(System::Exception ^e){}
						QQ->Add_Answer(antext,anfb,System::Convert::ToInt32(anfract));
					}

					Q->Add(QQ);
				}else if (qtype == "shortanswer"){
					System::String ^qtitle = "";
					try{qtitle = (question["name"])["text"]->InnerText;}catch(System::Exception ^e){}
					System::String ^qtext = "";
					try{qtext = (question["questiontext"])["text"]->InnerText;}catch(System::Exception ^e){}

					Data_Question ^QQ = gcnew Data_Question();
					QQ->Title = qtitle;
					QQ->Text = qtext;
					QQ->an_type = TYPE_TEXTBOX;

					for (int j=0;j<question->ChildNodes->Count;j++){
						if (question->ChildNodes[j]->Name != "answer") continue;
						System::Xml::XmlNode ^answer = question->ChildNodes[j];
						System::String ^anfract = "";
						try{anfract = answer->Attributes["fraction"]->Value;}catch(System::Exception ^e){}
						System::String ^antext = "";
						try{antext = answer["text"]->InnerText;}catch(System::Exception ^e){}
						System::String ^anfb = "";
						try{anfb = (answer["feedback"])["text"]->InnerText;}catch(System::Exception ^e){}
						QQ->Add_Answer(antext,anfb,System::Convert::ToInt32(anfract));
					}

					Q->Add(QQ);
				}else if (qtype == "matching"){
					System::String ^qtitle = "";
					try{qtitle = (question["name"])["text"]->InnerText;}catch(System::Exception ^e){}
					System::String ^qtext = "";
					try{qtext = (question["questiontext"])["text"]->InnerText;}catch(System::Exception ^e){}
					System::String ^qshuffle = "1";
					try{qshuffle = question["shuffleanswers"]->InnerText;}catch(System::Exception ^e){}

					Data_Question ^QQ = gcnew Data_Question();
					QQ->Title = qtitle;
					QQ->Text = qtext;
					if (qshuffle == "1") QQ->shuffle = true;
					else QQ->shuffle = false;
					QQ->an_type = TYPE_MATCHING;

					for (int j=0;j<question->ChildNodes->Count;j++){
						if (question->ChildNodes[j]->Name != "subquestion") continue;
						System::Xml::XmlNode ^subquest = question->ChildNodes[j];

						System::String ^antext1 = "";
						try{antext1 = subquest["text"]->InnerText;}catch(System::Exception ^e){}

						System::String ^antext2 = "";
						try{antext2 = (subquest["answer"])["text"]->InnerText;}catch(System::Exception ^e){}

						
						QQ->Add_Answer(antext1,"",100);
						QQ->Answers[QQ->Answers->Count-1]->match_grp = 1;
						QQ->Answers[QQ->Answers->Count-1]->order = j+1;

						QQ->Add_Answer(antext2,"",100);
						QQ->Answers[QQ->Answers->Count-1]->match_grp = 0;
						QQ->Answers[QQ->Answers->Count-1]->order = j+1;
					}

					Q->Add(QQ);
				}else if (qtype == "truefalse"){
					System::String ^qtitle = "";
					try{qtitle = (question["name"])["text"]->InnerText;}catch(System::Exception ^e){}
					System::String ^qtext = "";
					try{qtext = (question["questiontext"])["text"]->InnerText;}catch(System::Exception ^e){}

					Data_Question ^QQ = gcnew Data_Question();
					QQ->Title = qtitle;
					QQ->Text = qtext;
					QQ->shuffle = false;
					QQ->an_type = TYPE_SINGLE;

					for (int j=0;j<question->ChildNodes->Count;j++){
						if (question->ChildNodes[j]->Name != "answer") continue;
						System::Xml::XmlNode ^answer = question->ChildNodes[j];
						System::String ^anfract = "";
						try{anfract = answer->Attributes["fraction"]->Value;}catch(System::Exception ^e){}
						System::String ^antext = "";
						if (answer["text"]->InnerText == "true") antext = "Да";
						if (answer["text"]->InnerText == "false") antext = "Нет";
						System::String ^anfb = "";
						try{anfb = (answer["feedback"])["text"]->InnerText;}catch(System::Exception ^e){}
						QQ->Add_Answer(antext,anfb,System::Convert::ToInt32(anfract));
					}

					Q->Add(QQ);
				}else{
					problem = true;
				}
			}
		}

		for (int i=0;i<Q->Count;i++){
			Q[i]->Title = Q[i]->Title->Trim(' ', '\r', '\n');
			Q[i]->Text = Q[i]->Text->Trim(' ', '\r', '\n');
			if (Q[i]->Title == "") Q[i]->Title = Q[i]->Text; 
			for (int j=0;j<Q[i]->Answers->Count;j++){
				Q[i]->Answers[j]->Text = Q[i]->Answers[j]->Text->Trim(' ', '\r', '\n');
				Q[i]->Answers[j]->Feedback = Q[i]->Answers[j]->Feedback->Trim(' ', '\r', '\n');
			}
		}

		for (int i=0;i<Q->Count;i++){
			Q[i]->quest_id = i+1;
		}

	}
	static const MoodleXML_Parser ^Instance(){
		if (self == nullptr) self = gcnew MoodleXML_Parser();
		return self;
	}
	virtual System::String ^GetName() override{
		return "MoodleXML";
	};
};