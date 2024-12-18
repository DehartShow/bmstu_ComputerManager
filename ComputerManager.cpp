#include "ComputerManager.h"

#include <fstream>
#include <stdexcept>

#include "tinyxml2.h"  // Для работы с XML

using namespace tinyxml2;

void ComputerManager::loadFromFile(const std::string& filename) {
  XMLDocument doc;
  if (doc.LoadFile(filename.c_str()) != XML_SUCCESS)
    throw std::runtime_error("Cannot load file");

  XMLElement* root = doc.FirstChildElement("Computers");
  if (!root) throw std::runtime_error("Invalid XML format");

  computers.clear();
  for (XMLElement* elem = root->FirstChildElement("Computer"); elem;
       elem = elem->NextSiblingElement("Computer")) {
    Computer computer;
    computer.type = elem->FirstChildElement("Type")->GetText();
    computer.inventoryNumber =
        elem->FirstChildElement("InventoryNumber")->GetText();
    computer.manufacturer = elem->FirstChildElement("Manufacturer")->GetText();
    computer.model = elem->FirstChildElement("Model")->GetText();
    computer.macAddress = elem->FirstChildElement("MACAddress")->GetText();
    computer.os = elem->FirstChildElement("OS")->GetText();
    computer.date = elem->FirstChildElement("Date")->GetText();
    computers.push_back(computer);
  }
}

void ComputerManager::saveToFile(const std::string& filename) {
  XMLDocument doc;
  XMLElement* root = doc.NewElement("Computers");
  doc.InsertFirstChild(root);

  for (const auto& computer : computers) {
    XMLElement* elem = doc.NewElement("Computer");
    elem->InsertNewChildElement("Type")->SetText(computer.type.c_str());
    elem->InsertNewChildElement("InventoryNumber")
        ->SetText(computer.inventoryNumber.c_str());
    elem->InsertNewChildElement("Manufacturer")
        ->SetText(computer.manufacturer.c_str());
    elem->InsertNewChildElement("Model")->SetText(computer.model.c_str());
    elem->InsertNewChildElement("MACAddress")
        ->SetText(computer.macAddress.c_str());
    elem->InsertNewChildElement("OS")->SetText(computer.os.c_str());
    elem->InsertNewChildElement("Date")->SetText(computer.date.c_str());
    root->InsertEndChild(elem);
  }

  if (doc.SaveFile(filename.c_str()) != XML_SUCCESS)
    throw std::runtime_error("Cannot save file");
}

void ComputerManager::addComputer(const Computer& computer) {
  // Проверка уникальности
  for (const auto& c : computers) {
    if (c.inventoryNumber == computer.inventoryNumber)
      throw std::runtime_error("Инвентаризационный номер должен быть уникален");
  }
  computers.push_back(computer);
}

void ComputerManager::removeComputer(const std::string& inventoryNumber) {
  computers.erase(std::remove_if(computers.begin(), computers.end(),
                                 [&inventoryNumber](const Computer& c) {
                                   return c.inventoryNumber == inventoryNumber;
                                 }),
                  computers.end());
}

const std::vector<Computer>& ComputerManager::getComputers() const {
  return computers;
}
