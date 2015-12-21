#pragma once
#include "data_struct.h"
#include "Generator.h"

public ref class ILIAS_Generator:
	public Generator{
	System::String ^XMLescape(System::String ^str){
		System::String ^res = gcnew System::String(str);
		res = res->Replace("&", "&amp;");
		res = res->Replace("\"", "&quot;");
		res = res->Replace("<", "&lt;");
		res = res->Replace(">", "&gt;");
		return res;
	}
	System::String ^QuestionToXML(Data_Question ^DQ, int pool_id){
		System::String ^out = "<item ident=\"il_0_qst_"+DQ->quest_id+"\" title=\""+XMLescape(DQ->Title)+"\" maxattempts=\""+DQ->maxattempts+"\">";
		out += "<qticomment/>";
		out += "<duration>P"+DQ->time_year+"Y"+DQ->time_month+"M"+DQ->time_day+"DT"+DQ->time_hour+"H"+DQ->time_min+"M"+DQ->time_sec+"S</duration>";
		out += "<itemmetadata><qtimetadata>";
		out += "<qtimetadatafield><fieldlabel>ILIAS_VERSION</fieldlabel><fieldentry>5.0.0 2015-02-06</fieldentry></qtimetadatafield>";
		if (DQ->an_type == TYPE_SINGLE) out += "<qtimetadatafield><fieldlabel>QUESTIONTYPE</fieldlabel><fieldentry>SINGLE CHOICE QUESTION</fieldentry></qtimetadatafield>";
		if (DQ->an_type == TYPE_MULTIPLE) out += "<qtimetadatafield><fieldlabel>QUESTIONTYPE</fieldlabel><fieldentry>MULTIPLE CHOICE QUESTION</fieldentry></qtimetadatafield>";
		if (DQ->an_type == TYPE_TEXTBOX) out += "<qtimetadatafield><fieldlabel>QUESTIONTYPE</fieldlabel><fieldentry>CLOZE QUESTION</fieldentry></qtimetadatafield>";
		if (DQ->an_type == TYPE_ORDERING) out += "<qtimetadatafield><fieldlabel>QUESTIONTYPE</fieldlabel><fieldentry>ORDERING QUESTION</fieldentry></qtimetadatafield>";
		if (DQ->an_type == TYPE_MATCHING) out += "<qtimetadatafield><fieldlabel>QUESTIONTYPE</fieldlabel><fieldentry>MATCHING QUESTION</fieldentry></qtimetadatafield>";
		out += "<qtimetadatafield><fieldlabel>AUTHOR</fieldlabel><fieldentry>"+XMLescape(DQ->Author)+"</fieldentry></qtimetadatafield>";
		out += "<qtimetadatafield><fieldlabel>additional_cont_edit_mode</fieldlabel><fieldentry>default</fieldentry></qtimetadatafield>";
		out += "<qtimetadatafield><fieldlabel>externalId</fieldlabel><fieldentry>il_0_qst_"+DQ->quest_id+"</fieldentry></qtimetadatafield>";
		out += "<qtimetadatafield><fieldlabel>thumb_size</fieldlabel><fieldentry/></qtimetadatafield>";
		out += "<qtimetadatafield><fieldlabel>feedback_setting</fieldlabel><fieldentry>"+DQ->feedback_setting+"</fieldentry></qtimetadatafield>";
		if (DQ->an_type == 3){
			out += "<qtimetadatafield><fieldlabel>thumb_geometry</fieldlabel><fieldentry>100</fieldentry></qtimetadatafield>";
			out += "<qtimetadatafield><fieldlabel>element_height</fieldlabel><fieldentry/></qtimetadatafield>";
			int points = 0;
			for (int i=0;i<DQ->Answers->Count;i++){
				points += DQ->Answers[i]->Add_Yes;
			}
			out += "<qtimetadatafield><fieldlabel>points</fieldlabel><fieldentry>"+points+"</fieldentry></qtimetadatafield>";
		}
		if (DQ->an_type == 4){
			out += "<qtimetadatafield><fieldlabel>thumb_geometry</fieldlabel><fieldentry>100</fieldentry></qtimetadatafield>";
			out += "<qtimetadatafield><fieldlabel>matching_mode</fieldlabel><fieldentry>1:1</fieldentry></qtimetadatafield>";
		}
		out += "</qtimetadata></itemmetadata>";

		out += "<presentation label=\""+XMLescape(DQ->Title)+"\"><flow>";
		out += "<material><mattext texttype=\"text/plain\">"+XMLescape(DQ->Text)+"</mattext></material>";
		if (DQ->an_type == TYPE_SINGLE) out += "<response_lid ident=\"MCSR\" rcardinality=\"Single\">";
		if (DQ->an_type == TYPE_MULTIPLE) out += "<response_lid ident=\"MCMR\" rcardinality=\"Multiple\">";
		if (DQ->an_type == TYPE_SINGLE || DQ->an_type == TYPE_MULTIPLE){
		if (DQ->shuffle) out += "<render_choice shuffle=\"Yes\">";
		else out += "<render_choice shuffle=\"No\">";
		for (int i=0;i<DQ->Answers->Count;i++){
			out += "<response_label ident=\""+(DQ->Answers[i]->an_id)+"\"><material><mattext texttype=\"text/plain\">"+XMLescape(DQ->Answers[i]->Text)+"</mattext></material></response_label>";
		}
		out += "</render_choice></response_lid>";
		}
		if (DQ->an_type == TYPE_TEXTBOX) out += "<response_str ident=\"gap_0\" rcardinality=\"Single\"><render_fib fibtype=\"String\" prompt=\"Box\" columns=\"0\"></render_fib></response_str><material><mattext texttype=\"text/plain\"></mattext></material>";
		
		if (DQ->an_type == TYPE_ORDERING){
			out += "<response_lid ident=\"OQT\" rcardinality=\"Ordered\" output=\"javascript\"><render_choice shuffle=\"Yes\">";
			for (int i=0;i<DQ->Answers->Count;i++){
				out += "<response_label ident=\""+(DQ->Answers[i]->an_id)+"\"><material><mattext texttype=\"text/plain\">"+XMLescape(DQ->Answers[i]->Text)+"</mattext></material><material><mattext label=\"answerdepth\">0</mattext></material></response_label>";
			}
			out += "</render_choice></response_lid>";
		}

		if (DQ->an_type == TYPE_MATCHING){
			System::String ^matchgroup = "";
			for (int i=1;i<DQ->Answers->Count;i++){
				if (DQ->Answers[i]->match_grp == 1) matchgroup += ","+DQ->Answers[i]->order;
			}
			matchgroup = matchgroup->Substring(1);

			out += "<response_grp ident=\"MQ\" rcardinality=\"Multiple\"><render_choice shuffle=\"Yes\">";
			for (int i=0;i<DQ->Answers->Count;i++){
				if (DQ->Answers[i]->match_grp == 0){
					out += "<response_label ident=\""+((DQ->Answers->Count)+(DQ->Answers[i]->order))+"\"  match_max=\"1\" match_group=\""+matchgroup+"\"><material><mattext texttype=\"text/plain\">"+DQ->Answers[i]->Text+"</mattext></material></response_label>";
				}
			}
			for (int i=0;i<DQ->Answers->Count;i++){
				if (DQ->Answers[i]->match_grp == 1){
					out += "<response_label ident=\""+DQ->Answers[i]->order+"\"><material><mattext texttype=\"text/plain\">"+DQ->Answers[i]->Text+"</mattext></material></response_label>";
				}
			}
			out += "</render_choice></response_grp>";
		}
		
		out += "</flow></presentation>";

		out += "<resprocessing><outcomes><decvar></decvar></outcomes>";
		for (int i=0;i<DQ->Answers->Count;i++){
			if (DQ->an_type == TYPE_SINGLE){
				out += "<respcondition continue=\"Yes\">";
				out += "<conditionvar><varequal respident=\"MSCR\">"+(DQ->Answers[i]->an_id)+"</varequal></conditionvar>";
				out += "<setvar action=\"Add\">"+(DQ->Answers[i]->Add_Yes)+"</setvar>";
				out += "<displayfeedback feedbacktype=\"Response\" linkrefid=\"response_"+(DQ->Answers[i]->an_id)+"\"/>";
				out += "</respcondition>";
			}
			if (DQ->an_type == TYPE_MULTIPLE){
				out += "<respcondition continue=\"Yes\">";
				out += "<conditionvar><varequal respident=\"MCMR\">"+(DQ->Answers[i]->an_id)+"</varequal></conditionvar>";
				out += "<setvar action=\"Add\">"+(DQ->Answers[i]->Add_Yes)+"</setvar>";
				out += "<displayfeedback feedbacktype=\"Response\" linkrefid=\"response_"+(DQ->Answers[i]->an_id)+"\"/>";
				out += "</respcondition>";
				out += "<respcondition continue=\"Yes\">";
				out += "<conditionvar><not><varequal respident=\"MCMR\">"+(DQ->Answers[i]->an_id)+"</varequal></not></conditionvar>";
				out += "<setvar action=\"Add\">"+(DQ->Answers[i]->Add_No)+"</setvar>";
				out += "<displayfeedback feedbacktype=\"Response\" linkrefid=\"response_"+(DQ->Answers[i]->an_id)+"\"/>";
				out += "</respcondition>";
			}
			if (DQ->an_type == TYPE_TEXTBOX){
				out += "<respcondition continue=\"Yes\">";
				out += "<conditionvar><varequal respident=\"gap_0\">"+XMLescape(DQ->Answers[i]->Text)+"</varequal></conditionvar>";
				out += "<setvar action=\"Add\">"+(DQ->Answers[i]->Add_Yes)+"</setvar>";
				out += "<displayfeedback feedbacktype=\"Response\" linkrefid=\"0_Response_"+(DQ->Answers[i]->an_id)+"\"/>";
				out += "</respcondition>";
			}
			if (DQ->an_type == TYPE_ORDERING){
				out += "<respcondition continue=\"Yes\">";
				out += "<conditionvar><varequal respident=\"OQT\" index=\""+(DQ->Answers[i]->an_id)+"\">"+(DQ->Answers[i]->order)+"</varequal></conditionvar>";
				out += "<setvar action=\"Add\">"+(DQ->Answers[i]->Add_Yes)+"</setvar>";
				out += "<displayfeedback feedbacktype=\"Response\" linkrefid=\"link_"+(DQ->Answers[i]->an_id)+"\"/>";
				out += "</respcondition>";
			}
			if (DQ->an_type == TYPE_MATCHING){
				if (DQ->Answers[i]->match_grp == 1){
					int matcher = (DQ->Answers->Count)+(DQ->Answers[i]->order);
					out += "<respcondition continue=\"Yes\">";
					out += "<conditionvar><varsubset respident=\"MQ\">"+(DQ->Answers[i]->order)+","+matcher+"</varsubset></conditionvar>";
					out += "<setvar action=\"Add\">"+(DQ->Answers[i]->Add_Yes)+"</setvar>";
					out += "<displayfeedback feedbacktype=\"Response\" linkrefid=\"correct_"+(DQ->Answers[i]->order)+"_\"/>";
					out += "</respcondition>";
				}
			}
		}
		out += "</resprocessing>";
		for (int i=0;i<DQ->Answers->Count;i++){
			if (DQ->an_type == TYPE_SINGLE || DQ->an_type == TYPE_MULTIPLE){
				out += "<itemfeedback ident=\"response_"+(DQ->Answers[i]->an_id)+"\" view=\"All\"><flow_mat><material>";
				out += "<mattext texttype=\"text/plain\">"+XMLescape(DQ->Answers[i]->Feedback)+"</mattext></material></flow_mat></itemfeedback>";
			}
			if (DQ->an_type == TYPE_TEXTBOX){
				out += "<itemfeedback ident=\"0_Response_"+(DQ->Answers[i]->an_id)+"\" view=\"All\"><flow_mat><material>";
				out += "<mattext texttype=\"text/plain\">"+XMLescape(DQ->Answers[i]->Feedback)+"</mattext></material></flow_mat></itemfeedback>";
			}
			if (DQ->an_type == TYPE_ORDERING){
				out += "<itemfeedback ident=\"link_"+(DQ->Answers[i]->an_id)+"\" view=\"All\"><flow_mat><material>";
				out += "<mattext texttype=\"text/plain\">"+XMLescape(DQ->Answers[i]->Feedback)+"</mattext></material></flow_mat></itemfeedback>";
			}
			if (DQ->an_type == TYPE_MATCHING){
				if (DQ->Answers[i]->match_grp == 1){
					out += "<itemfeedback ident=\"correct_"+(DQ->Answers[i]->an_id)+"_"+((DQ->Answers->Count)+(DQ->Answers[i]->an_id))+"\" view=\"All\"><flow_mat><material>";
					out += "<mattext texttype=\"text/plain\">"+XMLescape(DQ->Answers[i]->Feedback)+"</mattext></material></flow_mat></itemfeedback>";
				}
			}
		}
		out += "</item>";
		return out;
	}
	System::String ^PoolToXMLqpl(Data_Pool ^DP){
		System::String ^out = "<?xml version=\"1.0\" encoding=\"utf-8\"?><!DOCTYPE Test SYSTEM \"http://www.ilias.uni-koeln.de/download/dtd/ilias_co.dtd\"><!--Export of ILIAS Test Questionpool 529 of installation .--><ContentObject Type=\"Questionpool_Test\"><MetaData><General Structure=\"Hierarchical\">";
		out += "<Identifier Catalog=\"ILIAS\" Entry=\"il_0_qpl_"+DP->pool_id+"\"/>";
		out += "<Title Language=\""+XMLescape(DP->Language)+"\">"+XMLescape(DP->Title)+"</Title>";
		out += "<Language Language=\""+XMLescape(DP->Language)+"\"/>";
		out += "<Description Language=\""+XMLescape(DP->Language)+"\">"+XMLescape(DP->Description)+"</Description>";
		if (DP->Keywords->Count == 0){
			out += "<Keyword Language=\""+XMLescape(DP->Language)+"\"/>";
		}else{
			for (int i=0;i<DP->Keywords->Count;i++){
				out += "<Keyword Language=\""+XMLescape(DP->Language)+"\">"+XMLescape(DP->Keywords[i])+"</Keyword>";
			}
		}
		out += "</General></MetaData>";
		for (int i=0;i<DP->Questions->Count;i++){
			out += "<PageObject><PageContent PCID=\""+DP->pool_id+"00000"+i+"\">";
			out += "<Question QRef=\"il_0_qst_"+(DP->Questions[i]->quest_id)+"\"/></PageContent></PageObject>";
		}
		out += "</ContentObject>";
		return out;
	}
	System::String ^PoolToXMLqti(Data_Pool ^DP){
		System::String ^out = "<?xml version=\"1.0\" encoding=\"utf-8\"?><!DOCTYPE questestinterop SYSTEM \"ims_qtiasiv1p2p1.dtd\"><!--Generated by ILIAS XmlWriter--><questestinterop>";
		for (int i=0;i<DP->Questions->Count;i++){
			out += QuestionToXML(DP->Questions[i], DP->pool_id);
		}
		out += "</questestinterop>";
		return out;
	}
	ILIAS_Generator(): Generator(){};
protected:
	static ILIAS_Generator ^self = nullptr;
public:
	static ILIAS_Generator ^theSingleInstance;
	virtual System::String ^Generate(Data_Pool ^NewPool,System::String ^OutputDir) override{
		System::String ^OutputFilePath = "";
		System::String ^nomer = System::DateTime::Now.Ticks.ToString();
		System::String ^dir = nomer+"__0__qpl_"+(NewPool->pool_id);
		System::String^ qplfile = dir+"\\"+dir+"\\"+nomer+"__0__qpl_"+(NewPool->pool_id)+".xml";
		System::String^ qtifile = dir+"\\"+dir+"\\"+nomer+"__0__qti_"+(NewPool->pool_id)+".xml";
		
		if (System::IO::Directory::Exists(dir)){
			System::IO::Directory::Delete(dir,true);
		}
		System::IO::Directory::CreateDirectory(dir);
		System::IO::Directory::CreateDirectory(dir+"\\"+dir);
		System::IO::Directory::CreateDirectory(dir+"\\"+dir+"\\objects");
		try
		{
			if (System::IO::File::Exists(qplfile)) System::IO::File::Delete(qplfile);
			System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(qplfile);
			try{
				sw->WriteLine(PoolToXMLqpl(NewPool));
			}
			finally
			{
				delete sw;
			}
		}
		catch ( System::Exception^ e ){}
		try
		{
			if (System::IO::File::Exists(qtifile)) System::IO::File::Delete(qtifile);
			System::IO::StreamWriter^ sw = gcnew System::IO::StreamWriter(qtifile);
			try{
				sw->WriteLine(PoolToXMLqti(NewPool));
			}
			finally
			{
				delete sw;
			}
		}
		catch ( System::Exception^ e ){}
		
		Ionic::Zip::ZipFile ^ zip;
		try{
			zip = gcnew Ionic::Zip::ZipFile();
			zip->AddDirectory(dir);
			if (System::IO::File::Exists(OutputDir+"\\"+dir+".zip")){
				System::IO::File::Delete(OutputDir+"\\"+dir+".zip");
			}
			zip->Save(OutputDir+"\\"+dir+".zip");
			OutputFilePath = OutputDir+"\\"+dir+".zip";
		}
		finally
		{
			delete zip;
		}
		System::IO::Directory::Delete(dir,true);
		return OutputFilePath;
	};
	static const ILIAS_Generator ^Instance(){
		if (self == nullptr) self = gcnew ILIAS_Generator();
		return self;
	}
	virtual System::String ^GetName() override{
		return "ILIAS";
	};
};