#include "ComputerManager.h"

#include <fstream>
#include <stdexcept>
#include <locale>
#include <codecvt>
#include <string>

#include "tinyxml2.h"  // Для работы с XML

using namespace tinyxml2;

// Конвертирует std::wstring в std::string
std::string wstringToUtf8(const std::wstring& wstr) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.to_bytes(wstr);
}

// Конвертирует std::string в std::wstring
std::wstring utf8ToWstring(const std::string& utf8Str) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.from_bytes(utf8Str);
}

void ComputerManager::loadFromFile(const std::wstring& filename) {
  XMLDocument doc;
  if (doc.LoadFile(wstringToUtf8(filename).c_str()) != XML_SUCCESS)
    throw std::runtime_error("Cannot load file");

  XMLElement* root = doc.FirstChildElement("Computers");
  if (!root) throw std::runtime_error("Invalid XML format");

  computers.clear();
  for (XMLElement* elem = root->FirstChildElement("Computer"); elem;
       elem = elem->NextSiblingElement("Computer")) {
    Computer computer;
    computer.type = utf8ToWstring(elem->FirstChildElement("Type")->GetText());
    computer.inventoryNumber =
        utf8ToWstring(elem->FirstChildElement("InventoryNumber")->GetText());
    computer.manufacturer = utf8ToWstring(elem->FirstChildElement("Manufacturer")->GetText());
    computer.model = utf8ToWstring(elem->FirstChildElement("Model")->GetText());
    computer.macAddress = utf8ToWstring(elem->FirstChildElement("MACAddress")->GetText());
    computer.os = utf8ToWstring(elem->FirstChildElement("OS")->GetText());
    computer.date = utf8ToWstring(elem->FirstChildElement("Date")->GetText());
    computers.push_back(computer);
  }
}

void ComputerManager::saveToFile(const std::wstring& filename) {
  XMLDocument doc;
  XMLElement* root = doc.NewElement("Computers");
  doc.InsertFirstChild(root);

  for (const auto& computer : computers) {
    XMLElement* elem = doc.NewElement("Computer");
    elem->InsertNewChildElement("Type")->SetText(wstringToUtf8(computer.type).c_str());
    elem->InsertNewChildElement("InventoryNumber")
        ->SetText(wstringToUtf8(computer.inventoryNumber).c_str());
    elem->InsertNewChildElement("Manufacturer")
        ->SetText(wstringToUtf8(computer.manufacturer).c_str());
    elem->InsertNewChildElement("Model")->SetText(wstringToUtf8(computer.model).c_str());
    elem->InsertNewChildElement("MACAddress")
        ->SetText(wstringToUtf8(computer.macAddress).c_str());
    elem->InsertNewChildElement("OS")->SetText(wstringToUtf8(computer.os).c_str());
    elem->InsertNewChildElement("Date")->SetText(wstringToUtf8(computer.date).c_str());
    root->InsertEndChild(elem);
  }

  if (doc.SaveFile(wstringToUtf8(filename).c_str()) != XML_SUCCESS)
    throw std::runtime_error("Cannot save file");
}

void ComputerManager::addComputer(const Computer& computer) {
  // Проверка уникальности
  for (const auto& c : computers) {
    if (c.inventoryNumber == computer.inventoryNumber || c.macAddress == computer.macAddress)
      throw std::runtime_error("Инвентаризационный номер и MAC-адрес должы быть уникальны");
  }
  computers.push_back(computer);
}

void ComputerManager::removeComputer(const std::wstring& inventoryNumber) {
  computers.erase(std::remove_if(computers.begin(), computers.end(),
                                 [&inventoryNumber](const Computer& c) {
                                   return c.inventoryNumber == inventoryNumber;
                                 }),
                  computers.end());
}

const std::vector<Computer>& ComputerManager::getComputers() const {
  return computers;
}



