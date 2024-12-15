#pragma once
#include <msclr/marshal_cppstd.h>

#include "ComputerManager.h"
namespace ComputerManagementApp {

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

public
ref class Form1 : public System::Windows::Forms::Form {
 private:
  ComputerManager* manager;

  // Элементы интерфейса
  DataGridView ^ dataGridView;
  TextBox ^ typeBox;
  TextBox ^ inventoryNumberBox;
  TextBox ^ manufacturerBox;
  TextBox ^ modelBox;
  TextBox ^ macAddressBox;
  TextBox ^ osBox;
  TextBox ^ dateBox;

  Button ^ addButton;
  Button ^ editButton;
  Button ^ deleteButton;
  Button ^ saveButton;

 public:
  Form1(void) {
    InitializeComponent();
    manager = new ComputerManager();

    // Загрузка данных из XML
    try {
      manager->loadFromFile("computers.xml");
      LoadDataToGrid();
    } catch (const std::exception& e) {
      MessageBox::Show("Файла базы даннх нет. Он будет создан автоматически", "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Information);
    }
  }

 protected:
  ~Form1() { delete manager; }

 private:
  void InitializeComponent(void) {
    this->Text = "Учет компьютеров";
    this->Width = 1130;
    this->Height = 420;

    // DataGridView
    dataGridView = gcnew DataGridView();
    dataGridView->Location = Point(10, 10);
    dataGridView->Size = Drawing::Size(750, 350);
    dataGridView->ReadOnly = true;
    dataGridView->AllowUserToAddRows = true;
    this->Controls->Add(dataGridView);

    // Поля ввода
    int inputY = 350;
    int inputX = 10;
    int step = 150;
    typeBox = CreateTextBox("Тип компьютера", 770 , 40);
    inventoryNumberBox = CreateTextBox("Инв. номер", 770 + 110, 40);
    manufacturerBox = CreateTextBox("Производитель", 770 + 110 * 2, 40);
    modelBox = CreateTextBox("Модель", 770, 100);
    macAddressBox = CreateTextBox("MAC адрес", 770 + 110, 100);
    osBox = CreateTextBox("ОС", 770 + 110 * 2, 100);
    dateBox = CreateTextBox("Дата", 770, 160);

    // Кнопки
    addButton = CreateButton("Добавить", 770, 200);
    addButton->Click += gcnew EventHandler(this, &Form1::OnAddButtonClick);

    deleteButton = CreateButton("Удалить", 770, 280);
    deleteButton->Click +=
        gcnew EventHandler(this, &Form1::OnDeleteButtonClick);

    saveButton = CreateButton("Сохранить", 770, 320);
    saveButton->Click += gcnew EventHandler(this, &Form1::OnSaveButtonClick);

    editButton = CreateButton("Редактировать", 770, 240);
    editButton->Click += gcnew EventHandler(this, &Form1::OnEditButtonClick);
  }

  // Создание TextBox с Label
  TextBox ^
      CreateTextBox(String ^ label, int x, int y) {
        Label ^ lbl = gcnew Label();
        lbl->Text = label;
        lbl->Location = Point(x, y - 25);
        this->Controls->Add(lbl);

        TextBox ^ box = gcnew TextBox();
        box->Location = Point(x, y);
        this->Controls->Add(box);
        return box;
      }

      // Создание Button
      Button
      ^
      CreateButton(String ^ text, int x, int y) {
        Button ^ btn = gcnew Button();
        btn->Text = text;
        btn->Location = Point(x, y);
        btn->Size = System::Drawing::Size(320, 30);
        this->Controls->Add(btn);
        return btn;
      }

      // Загрузка данных в DataGridView
      void LoadDataToGrid() {
    dataGridView->Columns->Clear();
    dataGridView->Columns->Add("Type", "Тип");
    dataGridView->Columns->Add("InventoryNumber", "Инв. номер");
    dataGridView->Columns->Add("Manufacturer", "Производитель");
    dataGridView->Columns->Add("Model", "Модель");
    dataGridView->Columns->Add("MACAddress", "MAC адрес");
    dataGridView->Columns->Add("OS", "ОС");
    dataGridView->Columns->Add("Date", "Дата");
    dataGridView->Rows->Clear();
    for (const auto& computer : manager->getComputers()) {
      dataGridView->Rows->Add(gcnew String(computer.type.c_str()),
                              gcnew String(computer.inventoryNumber.c_str()),
                              gcnew String(computer.manufacturer.c_str()),
                              gcnew String(computer.model.c_str()),
                              gcnew String(computer.macAddress.c_str()),
                              gcnew String(computer.os.c_str()),
                              gcnew String(computer.date.c_str()));
    }
  }

  // Обработчики событий кнопок
  void OnAddButtonClick(Object ^ sender, EventArgs ^ e) {
    try {
      Computer computer;
      computer.type = msclr::interop::marshal_as<std::string>(typeBox->Text);
      computer.inventoryNumber =
          msclr::interop::marshal_as<std::string>(inventoryNumberBox->Text);
      computer.manufacturer =
          msclr::interop::marshal_as<std::string>(manufacturerBox->Text);
      computer.model = msclr::interop::marshal_as<std::string>(modelBox->Text);
      computer.macAddress =
          msclr::interop::marshal_as<std::string>(macAddressBox->Text);
      computer.os = msclr::interop::marshal_as<std::string>(osBox->Text);
      computer.date = msclr::interop::marshal_as<std::string>(dateBox->Text);

      manager->addComputer(computer);
      LoadDataToGrid();
    } catch (const std::exception& e) {
      MessageBox::Show(gcnew String(e.what()), "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Error);
    }
  }

  void OnDeleteButtonClick(Object ^ sender, EventArgs ^ e) {
    int selectedIndex = dataGridView->SelectedRows[0]->Index;
    if (selectedIndex == -1) {
      MessageBox::Show("ВЫберите строку ", "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Error);
      return;
    } else {
      manager->deleteComputer(selectedIndex);
      LoadDataToGrid();
    }

  }

  void OnEditButtonClick(Object ^ sender, EventArgs ^ e) {
    try {
      // Получаем индекс выбранной строки
      int selectedIndex = dataGridView->SelectedRows[0]->Index;

      // Получаем данные из текстовых полей
      Computer computer;
      if (!msclr::interop::marshal_as<std::string>(typeBox->Text).empty()) {
        computer.type = msclr::interop::marshal_as<std::string>(typeBox->Text);
      }

      computer.inventoryNumber =
          msclr::interop::marshal_as<std::string>(inventoryNumberBox->Text);
      computer.manufacturer =
          msclr::interop::marshal_as<std::string>(manufacturerBox->Text);
      computer.model = msclr::interop::marshal_as<std::string>(modelBox->Text);
      computer.macAddress =
          msclr::interop::marshal_as<std::string>(macAddressBox->Text);
      computer.os = msclr::interop::marshal_as<std::string>(osBox->Text);
      computer.date = msclr::interop::marshal_as<std::string>(dateBox->Text);

      // Обновляем компьютер в менеджере (в коллекции)
      manager->editComputer(selectedIndex, computer);

      // Обновляем данные в таблице
      LoadDataToGrid();

      MessageBox::Show("Данные успешно обновлены!", "Успех",
                       MessageBoxButtons::OK, MessageBoxIcon::Information);
    } catch (const std::exception& e) {
      MessageBox::Show(gcnew String(e.what()), "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Error);
    }
  }

  void OnSaveButtonClick(Object ^ sender, EventArgs ^ e) {
    try {
      manager->saveToFile("computers.xml");
      MessageBox::Show("Данные сохранены!", "Успех", MessageBoxButtons::OK,
                       MessageBoxIcon::Information);
    } catch (const std::exception& e) {
      MessageBox::Show(gcnew String(e.what()), "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Error);
    }
  }
};
}  // namespace ComputerManagementApp