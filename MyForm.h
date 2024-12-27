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
  Button ^ clearButton;

  TextBox ^ searchBox;  // Поле для ввода поискового запроса
  ComboBox ^ searchCriteriaBox;  // Выпадающий список для выбора параметра
  Button ^ searchButton;  // Кнопка для поиска
  Button ^ resetButton;

 public:
  Form1(void) {
    InitializeComponent();
    manager = new ComputerManager();

    dataGridView->SelectionChanged +=
        gcnew EventHandler(this, &Form1::OnSelectionChanged);

    // Загрузка данных из XML
    try {
      manager->loadFromFile(L"computers.xml");
      LoadDataToGrid();
    } catch (const std::exception& e) {
      MessageBox::Show(
          "Файла базы даннх нет. Он будет создан автоматически, после "
          "сохранения данных.",
          "Информация", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }
  }

 protected:
  ~Form1() { delete manager; }

 private:
  bool changesMade = false;  // Флаг, показывающий, были ли изменения

  static bool MatchInventoryNumber(const Computer& comp,
                                   const std::wstring& inventoryId) {
    return comp.inventoryNumber == inventoryId;
  }

  void InitializeComponent(void) {
    this->Text = "Учёт компьютеров";
    this->Width = 1135;
    this->Height = 500;
    this->FormBorderStyle =
        System::Windows::Forms::FormBorderStyle::FixedSingle;  // Фиксированный
                                                               // размер окна
    this->MaximizeBox = false;  // Отключить кнопку максимизации

    this->FormClosing +=
        gcnew FormClosingEventHandler(this, &Form1::OnFormClosing);
    // Дополнительный код инициализации формы

    // DataGridView
    dataGridView = gcnew DataGridView();
    dataGridView->Location = Point(10, 10);
    dataGridView->Size = Drawing::Size(750, 380);
    dataGridView->ReadOnly = true;
    dataGridView->AllowUserToAddRows = true;
    this->Controls->Add(dataGridView);

    // Поля ввода
    typeBox = CreateTextBox("Тип компьютера", 770, 40);
    inventoryNumberBox = CreateTextBox("Инв. номер", 770 + 110, 40);
    manufacturerBox = CreateTextBox("Производитель", 770 + 110 * 2, 40);
    modelBox = CreateTextBox("Модель", 770, 100);
    macAddressBox = CreateTextBox("MAC адрес", 770 + 110, 100);
    osBox = CreateTextBox("ОС", 770 + 110 * 2, 100);
    dateBox = CreateTextBox("Дата", 770, 160);

    // Кнопки
    clearButton = CreateButton("Очистить поля", 770, 200, Color::Gray);
    clearButton->Click += gcnew EventHandler(this, &Form1::OnClearButtonClick);

    addButton = CreateButton("Добавить", 770, 240, Color::MediumSeaGreen);
    addButton->Click += gcnew EventHandler(this, &Form1::OnAddButtonClick);

    editButton = CreateButton("Редактировать", 770, 280, Color::Orange);
    editButton->Click += gcnew EventHandler(this, &Form1::OnEditButtonClick);

    deleteButton = CreateButton("Удалить", 770, 320, Color::Tomato);
    deleteButton->Click +=
        gcnew EventHandler(this, &Form1::OnDeleteButtonClick);

    saveButton = CreateButton("Сохранить", 770, 360, Color::DodgerBlue);
    saveButton->Click += gcnew EventHandler(this, &Form1::OnSaveButtonClick);

    // Кнопка сброса
    resetButton = CreateButton("Сбросить", 600, 420, Color::LightGray);
    resetButton->Size = System::Drawing::Size(150, 30);
    resetButton->Click += gcnew EventHandler(this, &Form1::OnResetButtonClick);

    // Поле поиска
    searchBox = CreateTextBox("Поиск", 10, 420);
    searchCriteriaBox = gcnew ComboBox();
    searchCriteriaBox->Location = Point(120, 420);
    searchCriteriaBox->Size = Drawing::Size(150, 25);
    searchCriteriaBox->DropDownStyle = ComboBoxStyle::DropDownList;
    searchCriteriaBox->Items->AddRange(
        gcnew array<String ^>{"Тип компьютера", "Инв. номер", "Производитель",
                              "Модель", "MAC адрес", "ОС", "Дата"});
    searchCriteriaBox->SelectedIndex = 0;
    this->Controls->Add(searchCriteriaBox);

    searchButton = CreateButton("Искать", 280, 420, Color::LightBlue);
    searchButton->Click +=
        gcnew EventHandler(this, &Form1::OnSearchButtonClick);

  }

  // Создание TextBox с Label
  TextBox ^
      CreateTextBox(String ^ label, int x, int y) {
        Label ^ lbl = gcnew Label();
        lbl->Text = label;
        lbl->Location = Point(x, y - 25);
        lbl->ForeColor = Color::Black;
        this->Controls->Add(lbl);

        TextBox ^ box = gcnew TextBox();
        box->Location = Point(x, y);
        box->Size = Drawing::Size(100, 30);
        box->BackColor = Color::White;
        this->Controls->Add(box);
        return box;
      }

      // Создание Button
      Button
      ^
      CreateButton(String ^ text, int x, int y, Color color) {
        Button ^ btn = gcnew Button();
        btn->Text = text;
        btn->Location = Point(x, y);
        btn->Size = System::Drawing::Size(320, 30);
        btn->BackColor = color;
        btn->ForeColor = Color::Black;
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

  // Метод для кнопки "Добавить"
  void OnAddButtonClick(Object ^ sender, EventArgs ^ e) {
    try {
      // Проверяем, заполнены ли все поля
      if (typeBox->Text->Trim() == "" ||
          inventoryNumberBox->Text->Trim() == "" ||
          manufacturerBox->Text->Trim() == "" || modelBox->Text->Trim() == "" ||
          macAddressBox->Text->Trim() == "" || osBox->Text->Trim() == "" ||
          dateBox->Text->Trim() == "") {
        MessageBox::Show("Все поля должны быть заполнены.", "Ошибка",
                         MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;  // Прерываем выполнение, если есть пустые поля
      }

      Computer computer;
      computer.type = msclr::interop::marshal_as<std::wstring>(typeBox->Text);
      computer.inventoryNumber =
          msclr::interop::marshal_as<std::wstring>(inventoryNumberBox->Text);
      computer.manufacturer =
          msclr::interop::marshal_as<std::wstring>(manufacturerBox->Text);
      computer.model = msclr::interop::marshal_as<std::wstring>(modelBox->Text);
      computer.macAddress =
          msclr::interop::marshal_as<std::wstring>(macAddressBox->Text);
      computer.os = msclr::interop::marshal_as<std::wstring>(osBox->Text);
      computer.date = msclr::interop::marshal_as<std::wstring>(dateBox->Text);

      manager->addComputer(computer);
      LoadDataToGrid();
      // Пример обработки добавления, который изменяет данные
      changesMade = true;
    } catch (const std::exception& e) {
      MessageBox::Show(gcnew String(e.what()), "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Error);
    }
  }

  // Метод для кнопки "Очистить"
  void OnClearButtonClick(Object ^ sender, EventArgs ^ e) {
    // Очищаем содержимое всех текстовых полей
    typeBox->Clear();
    inventoryNumberBox->Clear();
    manufacturerBox->Clear();
    modelBox->Clear();
    macAddressBox->Clear();
    osBox->Clear();
    dateBox->Clear();

    // Сообщение об успешной очистке
    // MessageBox::Show("Все поля очищены!", "Информация",
    // MessageBoxButtons::OK,
    //               MessageBoxIcon::Information);
  }

  // Метод для внесения данных в TextBoxes для выбранного элемента
  void OnSelectionChanged(Object ^ sender, EventArgs ^ e) {
    if (dataGridView->SelectedRows->Count > 0) {
      String ^ inventoryIdCLI = dataGridView->SelectedRows[0]
                                    ->Cells["InventoryNumber"]
                                    ->Value->ToString();
      std::wstring inventoryId =
          msclr::interop::marshal_as<std::wstring>(inventoryIdCLI);

      auto& computers = manager->getComputers();
      for (const auto& comp : computers) {
        if (comp.inventoryNumber == inventoryId) {
          typeBox->Text = gcnew String(comp.type.c_str());
          inventoryNumberBox->Text = gcnew String(comp.inventoryNumber.c_str());
          manufacturerBox->Text = gcnew String(comp.manufacturer.c_str());
          modelBox->Text = gcnew String(comp.model.c_str());
          macAddressBox->Text = gcnew String(comp.macAddress.c_str());
          osBox->Text = gcnew String(comp.os.c_str());
          dateBox->Text = gcnew String(comp.date.c_str());
          break;
        }
      }
    }
  }

  // Метод для кнопки "Удалить"
  void OnDeleteButtonClick(Object ^ sender, EventArgs ^ e) {
    try {
      // Проверяем, выбрана ли строка в таблице
      if (dataGridView->SelectedRows->Count == 0) {
        MessageBox::Show("Пожалуйста, выберите строку для удаления.", "Ошибка",
                         MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;
      }

      // Получаем индекс выбранной строки
      int selectedIndex = dataGridView->SelectedRows[0]->Index;
      String ^ inventoryNumberStr = dataGridView->SelectedRows[0]
                                        ->Cells["InventoryNumber"]
                                        ->Value->ToString();
      std::wstring inventoryNumber =
          msclr::interop::marshal_as<std::wstring>(inventoryNumberStr);

      // Проверяем, что индекс в допустимом диапазоне
      if (selectedIndex < 0 || selectedIndex >= manager->getComputersCount()) {
        MessageBox::Show("Неверный индекс выбранной строки.", "Ошибка",
                         MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
      }
      // Подтверждение удаления
      auto result =
          MessageBox::Show("Вы уверены, что хотите удалить выбранную запись?",
                           "Подтверждение удаления", MessageBoxButtons::YesNo,
                           MessageBoxIcon::Question);
      if (result == System::Windows::Forms::DialogResult::Yes) {
        // Удаляем компьютер из менеджера
        manager->deleteComputer(inventoryNumber);
        // Обновляем данные в таблице
        LoadDataToGrid();
        changesMade = true;
        MessageBox::Show("Данные успешно удалены!", "Успех",
                         MessageBoxButtons::OK, MessageBoxIcon::Information);
      }
    } catch (const std::exception& e) {
      MessageBox::Show(gcnew String(e.what()), "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Error);
    }
  }

  // Метод для кнопки "Редактировать"
  void OnEditButtonClick(Object ^ sender, EventArgs ^ e) {
    try {
      // Проверяем, выбрана ли строка в таблице
      if (dataGridView->SelectedRows->Count == 0) {
        MessageBox::Show("Пожалуйста, выберите строку для редактирования.",
                         "Ошибка", MessageBoxButtons::OK,
                         MessageBoxIcon::Warning);
        return;  // Прерываем выполнение, если ничего не выбрано
      }
      // Проверяем, заполнены ли все поля
      if (typeBox->Text->Trim() == "" ||
          inventoryNumberBox->Text->Trim() == "" ||
          manufacturerBox->Text->Trim() == "" || modelBox->Text->Trim() == "" ||
          macAddressBox->Text->Trim() == "" || osBox->Text->Trim() == "" ||
          dateBox->Text->Trim() == "") {
        MessageBox::Show("Все поля должны быть заполнены.", "Ошибка",
                         MessageBoxButtons::OK, MessageBoxIcon::Warning);
        return;  // Прерываем выполнение, если есть пустые поля
      }

      // Получаем индекс выбранной строки
      int selectedIndex = dataGridView->SelectedRows[0]->Index;

      String ^ inventoryNumberStr = dataGridView->SelectedRows[0]
                                        ->Cells["InventoryNumber"]
                                        ->Value->ToString();
      std::wstring inventoryNumber =
          msclr::interop::marshal_as<std::wstring>(inventoryNumberStr);

      String ^ MacAddressStr =
          dataGridView->SelectedRows[0]->Cells["MACAddress"]->Value->ToString();
      std::wstring MacAddress =
          msclr::interop::marshal_as<std::wstring>(MacAddressStr);

      // Проверяем, что индекс находится в допустимом диапазоне
      if (selectedIndex < 0 || selectedIndex >= manager->getComputersCount()) {
        MessageBox::Show("Неверный индекс выбранной строки.", "Ошибка",
                         MessageBoxButtons::OK, MessageBoxIcon::Error);
        return;
      }

      // Получаем данные из текстовых полей
      Computer computer;
      computer.type = msclr::interop::marshal_as<std::wstring>(typeBox->Text);
      computer.inventoryNumber =
          msclr::interop::marshal_as<std::wstring>(inventoryNumberBox->Text);
      computer.manufacturer =
          msclr::interop::marshal_as<std::wstring>(manufacturerBox->Text);
      computer.model = msclr::interop::marshal_as<std::wstring>(modelBox->Text);
      computer.macAddress =
          msclr::interop::marshal_as<std::wstring>(macAddressBox->Text);
      computer.os = msclr::interop::marshal_as<std::wstring>(osBox->Text);
      computer.date = msclr::interop::marshal_as<std::wstring>(dateBox->Text);

      // Обновляем компьютер в менеджере (в коллекции)
      manager->editComputer(inventoryNumber, MacAddress, computer);

      // Обновляем данные в таблице
      LoadDataToGrid();
      changesMade = true;

      MessageBox::Show("Данные успешно обновлены!", "Успех",
                       MessageBoxButtons::OK, MessageBoxIcon::Information);
    } catch (const std::exception& e) {
      MessageBox::Show(gcnew String(e.what()), "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Error);
    }
  }

  // Метод для кнопки "Сохранить"
  void OnSaveButtonClick(Object ^ sender, EventArgs ^ e) {
    try {
      manager->saveToFile(L"computers.xml");
      MessageBox::Show("Данные сохранены!", "Успех", MessageBoxButtons::OK,
                       MessageBoxIcon::Information);
      changesMade = false;
    } catch (const std::exception& e) {
      MessageBox::Show(gcnew String(e.what()), "Ошибка", MessageBoxButtons::OK,
                       MessageBoxIcon::Error);
    }
  }

  // Метод для обработки события закрытия формы
  void OnFormClosing(Object ^ sender, FormClosingEventArgs ^ e) {
    // Проверяем, были ли изменения
    if (changesMade) {
      // Показываем диалог с запросом на сохранение изменений
      auto result = MessageBox::Show(
          "Есть несохраненные изменения. Сохранить?", "Подтверждение",
          MessageBoxButtons::YesNoCancel, MessageBoxIcon::Question);

      if (result == System::Windows::Forms::DialogResult::Yes) {
        // Сохраняем данные
        manager->saveToFile(L"computers.xml");
        MessageBox::Show("Данные сохранены.", "Успех", MessageBoxButtons::OK,
                         MessageBoxIcon::Information);
      } else if (result == System::Windows::Forms::DialogResult::Cancel) {
        // Отменяем закрытие формы, если пользователь нажал "Отмена"
        e->Cancel = true;
      }
    }
  }

  // Метод кнопки "Искать"
  void OnSearchButtonClick(Object ^ sender, EventArgs ^ e) {
    String ^ query = searchBox->Text->Trim();
    String ^ selectedCriteria = searchCriteriaBox->SelectedItem->ToString();

    if (query->Length == 0) {
      MessageBox::Show("Введите текст для поиска.", "Ошибка",
                       MessageBoxButtons::OK, MessageBoxIcon::Warning);
      return;
    }

    // Переводим строку поиска и критерий в std::wstring
    std::wstring queryW = msclr::interop::marshal_as<std::wstring>(query);
    std::wstring criteriaW =
        msclr::interop::marshal_as<std::wstring>(selectedCriteria);

    // Фильтрация данных
    auto filteredComputers = manager->filterComputers(criteriaW, queryW);

    // Обновляем DataGridView с отфильтрованными данными
    dataGridView->Rows->Clear();
    for (const auto& computer : filteredComputers) {
      dataGridView->Rows->Add(gcnew String(computer.type.c_str()),
                              gcnew String(computer.inventoryNumber.c_str()),
                              gcnew String(computer.manufacturer.c_str()),
                              gcnew String(computer.model.c_str()),
                              gcnew String(computer.macAddress.c_str()),
                              gcnew String(computer.os.c_str()),
                              gcnew String(computer.date.c_str()));
    }
  }

  // Метод для кнопки "Сбросить"
  void OnResetButtonClick(Object ^ sender, EventArgs ^ e) {
    searchBox->Text = "";
    searchCriteriaBox->SelectedIndex = -1;
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

    MessageBox::Show("Результаты поиска сброшены.", "Информация", MessageBoxButtons::OK,
                     MessageBoxIcon::Information);
  }

};
}  // namespace ComputerManagementApp