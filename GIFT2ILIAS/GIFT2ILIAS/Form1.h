#pragma once
#include "data_struct.h"
#include "Converter.h"
#include "ILIAS_Generator.h"
#include "MoodleXML.h"
#include "AST.h"
#include "ConvertTask.h"
#include "Facade.h"

#include <Windows.h>


System::String ^GetEncod(System::String ^path){
	System::IO::BinaryReader ^instr;
	array<byte> ^data;
	try{
		instr = gcnew System::IO::BinaryReader(System::IO::File::OpenRead(path));
		data = instr->ReadBytes((int)instr->BaseStream->Length);
		instr->Close();
	}catch(System::Exception ^e){
		return "";
	}
	
	// определяем BOM (EF BB BF)
	if (data.Length > 2 && data[0] == 0xef && data[1] == 0xbb && data[2] == 0xbf) {
		if (data.Length != 3) return "UTF-8";
		else return "";
	}
	
	int i = 0;
	while (i < data.Length - 1) {
		if (data[i] > 0x7f) { // не ANSI-символ
        if ((data[i] >> 5) == 6) {
            if ((i > data.Length - 2) || ((data[i + 1] >> 6) != 2))
                return "Windows-1251";
            i++;
        } else if ((data[i] >> 4) == 14) {
            if ((i > data.Length - 3) || ((data[i + 1] >> 6) != 2) || ((data[i + 2] >> 6) != 2))
                return "Windows-1251";
            i += 2;
        } else if ((data[i] >> 3) == 30) {
            if ((i > data.Length - 4) || ((data[i + 1] >> 6) != 2) || ((data[i + 2] >> 6) != 2) || ((data[i + 3] >> 6) != 2))
                return "Windows-1251";
            i += 3;
        } else {
            return "Windows-1251";
        }
    }
    i++;
}

return "UTF-8";
}


namespace GIFT2ILIAS {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace Ionic::Zip;

	/// <summary>
	/// Сводка для Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Facade ^F;

	public: 


	public: 





	private: System::Windows::Forms::Button^  button5;

	private: System::Windows::Forms::Button^  button_RunOne;

	private: System::Windows::Forms::ImageList^  imageList_State;
	private: System::Windows::Forms::Button^  button_Statistics;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::ListBox^  listBox2;
	private: System::Windows::Forms::ListBox^  listBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox_OutputDir;
	private: System::Windows::Forms::TextBox^  textBox_PoolDescr;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  textBox_PoolTitle;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ComboBox^  comboBox_Encod;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TreeView^  treeView_Tasks;

	private: System::Windows::Forms::Button^  button_RunAll;
	private: System::Windows::Forms::Button^  button_AddGroup;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Button^  button_OKGroup;
	private: System::Windows::Forms::TextBox^  textBox_GroupTitle;
	private: System::Windows::Forms::Label^  label_GroupTitle;





	private: System::Windows::Forms::TextBox^  textBox_InputFile;

	
	
	public: Form1(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 

	protected: 

	protected: 


	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;












	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
	private: System::Windows::Forms::Button^  button3;


	private: System::ComponentModel::IContainer^  components;


	protected: 

	private:
		/// <summary>
		/// Требуется переменная конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Обязательный метод для поддержки конструктора - не изменяйте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button_RunOne = (gcnew System::Windows::Forms::Button());
			this->imageList_State = (gcnew System::Windows::Forms::ImageList(this->components));
			this->button_Statistics = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->listBox2 = (gcnew System::Windows::Forms::ListBox());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox_OutputDir = (gcnew System::Windows::Forms::TextBox());
			this->textBox_PoolDescr = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox_PoolTitle = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_Encod = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox_InputFile = (gcnew System::Windows::Forms::TextBox());
			this->treeView_Tasks = (gcnew System::Windows::Forms::TreeView());
			this->button_RunAll = (gcnew System::Windows::Forms::Button());
			this->button_AddGroup = (gcnew System::Windows::Forms::Button());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->button_OKGroup = (gcnew System::Windows::Forms::Button());
			this->textBox_GroupTitle = (gcnew System::Windows::Forms::TextBox());
			this->label_GroupTitle = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->panel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// button3
			// 
			this->button3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->button3->Location = System::Drawing::Point(831, 12);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(119, 23);
			this->button3->TabIndex = 12;
			this->button3->Text = L"Добавить задачу";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button5
			// 
			this->button5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->button5->Location = System::Drawing::Point(956, 12);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(105, 23);
			this->button5->TabIndex = 17;
			this->button5->Text = L"Удалить";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button_RunOne
			// 
			this->button_RunOne->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->button_RunOne->Location = System::Drawing::Point(12, 12);
			this->button_RunOne->Name = L"button_RunOne";
			this->button_RunOne->Size = System::Drawing::Size(107, 23);
			this->button_RunOne->TabIndex = 18;
			this->button_RunOne->Text = L"Выполнить";
			this->button_RunOne->UseVisualStyleBackColor = true;
			this->button_RunOne->Click += gcnew System::EventHandler(this, &Form1::button_RunOne_Click);
			// 
			// imageList_State
			// 
			this->imageList_State->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageList_State.ImageStream")));
			this->imageList_State->TransparentColor = System::Drawing::Color::Transparent;
			this->imageList_State->Images->SetKeyName(0, L"ConvertGroup");
			this->imageList_State->Images->SetKeyName(1, L"ConvertTask");
			// 
			// button_Statistics
			// 
			this->button_Statistics->Location = System::Drawing::Point(282, 12);
			this->button_Statistics->Name = L"button_Statistics";
			this->button_Statistics->Size = System::Drawing::Size(91, 23);
			this->button_Statistics->TabIndex = 20;
			this->button_Statistics->Text = L"Статистика";
			this->button_Statistics->UseVisualStyleBackColor = true;
			this->button_Statistics->Click += gcnew System::EventHandler(this, &Form1::button_Statistics_Click);
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel1->Controls->Add(this->button4);
			this->panel1->Controls->Add(this->listBox2);
			this->panel1->Controls->Add(this->listBox1);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->textBox_OutputDir);
			this->panel1->Controls->Add(this->textBox_PoolDescr);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->textBox_PoolTitle);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->comboBox_Encod);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->button1);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->textBox_InputFile);
			this->panel1->Location = System::Drawing::Point(13, 223);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(1048, 181);
			this->panel1->TabIndex = 21;
			this->panel1->Visible = false;
			// 
			// button4
			// 
			this->button4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->button4->Location = System::Drawing::Point(878, 70);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(145, 100);
			this->button4->TabIndex = 31;
			this->button4->Text = L"ОК";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// listBox2
			// 
			this->listBox2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->listBox2->FormattingEnabled = true;
			this->listBox2->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"в папку с программой", L"в другую папку"});
			this->listBox2->Location = System::Drawing::Point(115, 75);
			this->listBox2->Name = L"listBox2";
			this->listBox2->Size = System::Drawing::Size(120, 43);
			this->listBox2->TabIndex = 30;
			this->listBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBox2_SelectedIndexChanged);
			// 
			// listBox1
			// 
			this->listBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Moodle XML", L"AST"});
			this->listBox1->Location = System::Drawing::Point(878, 12);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(145, 43);
			this->listBox1->TabIndex = 29;
			// 
			// button2
			// 
			this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button2->Location = System::Drawing::Point(788, 70);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 28;
			this->button2->Text = L"выбрать";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Visible = false;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(49, 75);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(60, 13);
			this->label5->TabIndex = 27;
			this->label5->Text = L"Сохранить";
			// 
			// textBox_OutputDir
			// 
			this->textBox_OutputDir->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_OutputDir->Location = System::Drawing::Point(241, 75);
			this->textBox_OutputDir->Name = L"textBox_OutputDir";
			this->textBox_OutputDir->Size = System::Drawing::Size(541, 20);
			this->textBox_OutputDir->TabIndex = 26;
			this->textBox_OutputDir->Visible = false;
			// 
			// textBox_PoolDescr
			// 
			this->textBox_PoolDescr->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_PoolDescr->Location = System::Drawing::Point(115, 150);
			this->textBox_PoolDescr->Name = L"textBox_PoolDescr";
			this->textBox_PoolDescr->Size = System::Drawing::Size(748, 20);
			this->textBox_PoolDescr->TabIndex = 25;
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(26, 152);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(83, 13);
			this->label4->TabIndex = 24;
			this->label4->Text = L"Описание пула";
			// 
			// textBox_PoolTitle
			// 
			this->textBox_PoolTitle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_PoolTitle->Location = System::Drawing::Point(115, 124);
			this->textBox_PoolTitle->Name = L"textBox_PoolTitle";
			this->textBox_PoolTitle->Size = System::Drawing::Size(748, 20);
			this->textBox_PoolTitle->TabIndex = 23;
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(26, 126);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(83, 13);
			this->label3->TabIndex = 22;
			this->label3->Text = L"Название пула";
			// 
			// comboBox_Encod
			// 
			this->comboBox_Encod->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->comboBox_Encod->FormattingEnabled = true;
			this->comboBox_Encod->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"windows-1251", L"utf-8", L"utf-16", L"utf-32"});
			this->comboBox_Encod->Location = System::Drawing::Point(661, 36);
			this->comboBox_Encod->Name = L"comboBox_Encod";
			this->comboBox_Encod->Size = System::Drawing::Size(121, 21);
			this->comboBox_Encod->TabIndex = 21;
			this->comboBox_Encod->Text = L"windows-1251";
			this->comboBox_Encod->Visible = false;
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(593, 39);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(62, 13);
			this->label2->TabIndex = 20;
			this->label2->Text = L"Кодировка";
			this->label2->Visible = false;
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Location = System::Drawing::Point(788, 10);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 19;
			this->button1->Text = L"выбрать";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label1
			// 
			this->label1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(31, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(78, 13);
			this->label1->TabIndex = 18;
			this->label1->Text = L"Входной файл";
			// 
			// textBox_InputFile
			// 
			this->textBox_InputFile->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_InputFile->Location = System::Drawing::Point(115, 10);
			this->textBox_InputFile->Name = L"textBox_InputFile";
			this->textBox_InputFile->Size = System::Drawing::Size(667, 20);
			this->textBox_InputFile->TabIndex = 17;
			// 
			// treeView_Tasks
			// 
			this->treeView_Tasks->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->treeView_Tasks->FullRowSelect = true;
			this->treeView_Tasks->HideSelection = false;
			this->treeView_Tasks->Location = System::Drawing::Point(13, 42);
			this->treeView_Tasks->Name = L"treeView_Tasks";
			this->treeView_Tasks->Size = System::Drawing::Size(1048, 175);
			this->treeView_Tasks->TabIndex = 22;
			this->treeView_Tasks->BeforeCollapse += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &Form1::treeView_Tasks_BeforeCollapse);
			this->treeView_Tasks->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &Form1::treeView_Tasks_AfterSelect);
			this->treeView_Tasks->NodeMouseDoubleClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &Form1::treeView_Tasks_NodeMouseDoubleClick);
			// 
			// button_RunAll
			// 
			this->button_RunAll->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->button_RunAll->Location = System::Drawing::Point(125, 12);
			this->button_RunAll->Name = L"button_RunAll";
			this->button_RunAll->Size = System::Drawing::Size(151, 23);
			this->button_RunAll->TabIndex = 19;
			this->button_RunAll->Text = L"Выполнить все";
			this->button_RunAll->UseVisualStyleBackColor = true;
			this->button_RunAll->Click += gcnew System::EventHandler(this, &Form1::button_RunAll_Click);
			// 
			// button_AddGroup
			// 
			this->button_AddGroup->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->button_AddGroup->Location = System::Drawing::Point(710, 12);
			this->button_AddGroup->Name = L"button_AddGroup";
			this->button_AddGroup->Size = System::Drawing::Size(115, 23);
			this->button_AddGroup->TabIndex = 23;
			this->button_AddGroup->Text = L"Добавить группу";
			this->button_AddGroup->UseVisualStyleBackColor = true;
			this->button_AddGroup->Click += gcnew System::EventHandler(this, &Form1::button_AddGroup_Click);
			// 
			// panel2
			// 
			this->panel2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel2->Controls->Add(this->button_OKGroup);
			this->panel2->Controls->Add(this->textBox_GroupTitle);
			this->panel2->Controls->Add(this->label_GroupTitle);
			this->panel2->Location = System::Drawing::Point(12, 223);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(1048, 95);
			this->panel2->TabIndex = 33;
			this->panel2->Visible = false;
			// 
			// button_OKGroup
			// 
			this->button_OKGroup->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button_OKGroup->Location = System::Drawing::Point(879, 17);
			this->button_OKGroup->Name = L"button_OKGroup";
			this->button_OKGroup->Size = System::Drawing::Size(144, 42);
			this->button_OKGroup->TabIndex = 2;
			this->button_OKGroup->Text = L"ОК";
			this->button_OKGroup->UseVisualStyleBackColor = true;
			this->button_OKGroup->Click += gcnew System::EventHandler(this, &Form1::button_OKGroup_Click);
			// 
			// textBox_GroupTitle
			// 
			this->textBox_GroupTitle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->textBox_GroupTitle->Location = System::Drawing::Point(93, 19);
			this->textBox_GroupTitle->Name = L"textBox_GroupTitle";
			this->textBox_GroupTitle->Size = System::Drawing::Size(770, 20);
			this->textBox_GroupTitle->TabIndex = 1;
			// 
			// label_GroupTitle
			// 
			this->label_GroupTitle->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label_GroupTitle->AutoSize = true;
			this->label_GroupTitle->Location = System::Drawing::Point(27, 22);
			this->label_GroupTitle->Name = L"label_GroupTitle";
			this->label_GroupTitle->Size = System::Drawing::Size(60, 13);
			this->label_GroupTitle->TabIndex = 0;
			this->label_GroupTitle->Text = L"Название:";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1073, 416);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->button_AddGroup);
			this->Controls->Add(this->treeView_Tasks);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->button_Statistics);
			this->Controls->Add(this->button_RunAll);
			this->Controls->Add(this->button_RunOne);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button3);
			this->Cursor = System::Windows::Forms::Cursors::Default;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;
			this->Name = L"Form1";
			this->Text = L"Конвертер файлов тестовых заданий";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->ResumeLayout(false);

		}

#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 ConvertTasksPool ^CTPool = (ConvertTasksPool ^)ConvertTasksPool::Instance();
				 ConvertComponent ^CTTree = gcnew ConvertGroup();
				 
				 treeView_Tasks->ImageList = imageList_State;

				 listBox1->SelectedIndex = 0;
				 listBox2->SelectedIndex = 0;

				 F = gcnew Facade();
				 F->SetCTPool(CTPool);
				 F->SetCTTree(CTTree);
				 F->SetTVTasks(treeView_Tasks);

				 F->DrawTasks();

				 textBox_OutputDir->Text = System::IO::Directory::GetCurrentDirectory();
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 Windows::Forms::DialogResult ^OFDR = openFileDialog1->ShowDialog();
				 if (OFDR->ToString() != "Cancel") textBox_InputFile->Text = openFileDialog1->FileName;
			 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 Windows::Forms::DialogResult ^FBDR = folderBrowserDialog1->ShowDialog();
			 if (FBDR->ToString() != "Cancel") textBox_OutputDir->Text = folderBrowserDialog1->SelectedPath;
		 }

private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 QF_Converter ^C = gcnew QF_Converter();
			 C->SetParser(gcnew Parser());
			 C->SetGenerator(gcnew Generator());
			 ConvertTask ^CT = F->CTPool->Get(C,"","","","","");

			 ConvertComponent ^CParent;
			 if (treeView_Tasks->SelectedNode != nullptr){
				 CParent = (ConvertComponent ^)(treeView_Tasks->SelectedNode->Tag);
			 }else{
				 CParent = F->GetCTTree();
			 }

			 try{
				 F->AddConvertComponent(CT,CParent);
			 }catch(System::Exception ^e){
				 CParent = CParent->Parent;
				 F->AddConvertComponent(CT,CParent);
			 }
		 }
private: System::Void listBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (listBox2->SelectedIndex == 0){
				 textBox_OutputDir->Visible = false;
				 button2->Visible = false;
			 }else{
				 textBox_OutputDir->Visible = true;
				 button2->Visible = true;
			 }
		 }
private: System::Void listView_Tasks_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 
		 }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (treeView_Tasks->SelectedNode == nullptr){
				 MessageBox::Show("Выберите задачу из списка.");
				 return;
			 }
			 panel1->Visible = false;
			 panel2->Visible = false;

			 String ^outputdir;
			 if (listBox2->SelectedIndex == 0){
				 outputdir = System::IO::Directory::GetCurrentDirectory();
			 }else{
				 outputdir = textBox_OutputDir->Text;
			 }
			 QF_Converter ^C = gcnew QF_Converter();
			 if (listBox1->SelectedIndex == 0){
				 C->SetParser((Parser ^)MoodleXML_Parser::Instance());
			 }else if (listBox1->SelectedIndex == 1){
				 C->SetParser((Parser ^)AST_Parser::Instance());
			 }
			 C->SetGenerator((Generator ^)ILIAS_Generator::Instance());
			 ConvertTask ^CT = F->CTPool->Get(C,textBox_InputFile->Text,outputdir,GetEncod(textBox_InputFile->Text),textBox_PoolTitle->Text,textBox_PoolDescr->Text);
			 
			 ConvertTask ^COld = (ConvertTask ^)(treeView_Tasks->SelectedNode->Tag);

			 F->UpdateConvertTask(CT,COld);

			 F->CTPool->Put(CT);
		 }
private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (treeView_Tasks->SelectedNode == nullptr){
				 MessageBox::Show("Выберите элемент из списка.");
				 return;
			 }
			 ConvertComponent ^CC = (ConvertComponent ^)(treeView_Tasks->SelectedNode->Tag);
			 if (CC->GetParent() == nullptr) return;

			 panel1->Visible = false;
			 panel2->Visible = false;

			 F->DelConvertComponent(CC);
		 }
private: System::Void button_RunOne_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (treeView_Tasks->SelectedNode == nullptr){
				 MessageBox::Show("Выберите задачу из списка.");
				 return;
			 }
			 panel1->Visible = false;
			 panel2->Visible = false;

			 ConvertComponent ^CC = (ConvertComponent ^)(treeView_Tasks->SelectedNode->Tag);
			 
			 this->Cursor = System::Windows::Forms::Cursors::WaitCursor;
			 CC->Convert();
			 F->ShowState(CC);
			 this->Cursor = System::Windows::Forms::Cursors::Default;
			 treeView_Tasks->SelectedNode = nullptr;
		 }
private: System::Void button_RunAll_Click(System::Object^  sender, System::EventArgs^  e) {
			 panel1->Visible = false;
			 panel2->Visible = false;
			 this->Cursor = System::Windows::Forms::Cursors::WaitCursor;
			 F->DoTasks();
			 this->Cursor = System::Windows::Forms::Cursors::Default;
			 treeView_Tasks->SelectedNode = nullptr;
		 }
private: System::Void button_Statistics_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::String ^StatisticsText = "";
			 StatisticsText += "Всего заданий:\t"+(F->IE->TotalCount())+"\r\n";
			 StatisticsText += "~ На очереди:\t"+(F->IE->WaitingCount())+"\r\n";
			 StatisticsText += "~ Успешно:\t"+(F->IE->SuccessCount())+"\r\n";
			 StatisticsText += "~ С ошибками:\t"+(F->IE->ErrorCount());
			 MessageBox::Show(StatisticsText);
		 }
		 void NodeSelected(){
			 if (treeView_Tasks->SelectedNode == nullptr){
				 panel1->Visible = false;
				 panel2->Visible = false;
				 return;
			 }

			 if (dynamic_cast<ConvertTask ^>(treeView_Tasks->SelectedNode->Tag) != nullptr){
				 panel1->Visible = true;
				 panel2->Visible = false;
				 ConvertTask ^CC = (ConvertTask ^)(treeView_Tasks->SelectedNode->Tag);

				 listBox2->SelectedIndex = 1;
				 textBox_InputFile->Text = CC->InputFile;
				 comboBox_Encod->Text = CC->Encod;
				 textBox_OutputDir->Text = CC->OutputDir;
				 textBox_PoolTitle->Text = CC->PoolTitle;
				 textBox_PoolDescr->Text = CC->PoolDescr;
				 if (CC->C->ParserDelegate->GetName() == "MoodleXML"){
					 listBox1->SelectedIndex = 0;
				 }else if (CC->C->ParserDelegate->GetName() == "AST"){
					 listBox1->SelectedIndex = 1;
				 }
			 }else if (dynamic_cast<ConvertGroup ^>(treeView_Tasks->SelectedNode->Tag) != nullptr){
				 ConvertGroup ^CC = (ConvertGroup ^)(treeView_Tasks->SelectedNode->Tag);
				 if (CC->GetParent() == nullptr) return;
				 panel1->Visible = false;
				 panel2->Visible = true;
				 textBox_GroupTitle->Text = CC->Title;
			 }
		 }
private: System::Void treeView_Tasks_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
			 NodeSelected();
		 }
private: System::Void button_AddGroup_Click(System::Object^  sender, System::EventArgs^  e) {
			 ConvertGroup ^CG = gcnew ConvertGroup();

			 ConvertComponent ^CParent;
			 if (treeView_Tasks->SelectedNode != nullptr){
				 CParent = (ConvertComponent ^)(treeView_Tasks->SelectedNode->Tag);
			 }else{
				 CParent = F->GetCTTree();
			 }

			 try{
				 F->AddConvertComponent(CG,CParent);
			 }catch(System::Exception ^e){
				 CParent = CParent->Parent;
				 F->AddConvertComponent(CG,CParent);
			 }
		 }
private: System::Void treeView_Tasks_BeforeCollapse(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e) {
			 //e->Cancel = true;
		 }
private: System::Void button_OKGroup_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (treeView_Tasks->SelectedNode == nullptr){
				 MessageBox::Show("Выберите задачу из списка.");
				 return;
			 }
			 panel1->Visible = false;
			 panel2->Visible = false;

			 ConvertGroup ^CG = gcnew ConvertGroup();
			 CG->Title = textBox_GroupTitle->Text;
			 
			 ConvertGroup ^COld = (ConvertGroup ^)(treeView_Tasks->SelectedNode->Tag);

			 F->UpdateConvertGroup(CG,COld);
		 }
private: System::Void treeView_Tasks_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) {
			 
		 }
private: System::Void treeView_Tasks_NodeMouseDoubleClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) {
			 NodeSelected();
		 }
};
}

