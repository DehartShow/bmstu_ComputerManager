#pragma once
#include <stdexcept>
#include <string>
#include <vector>

struct Computer {
  std::wstring type;
  std::wstring inventoryNumber;
  std::wstring manufacturer;
  std::wstring model;
  std::wstring macAddress;
  std::wstring os;
  std::wstring date;
};

class ComputerManager {
 private:
  std::vector<Computer> computers;

 public:
  void loadFromFile(const std::wstring& filename);
  void saveToFile(const std::wstring& filename);
  void addComputer(const Computer& computer);
  void removeComputer(const std::wstring& inventoryNumber);
  void deleteComputer(const std::wstring inventoryNumber) {
    for (int i = 0; i < computers.size(); ++i) {
      if (computers[i].inventoryNumber == inventoryNumber) {
        computers.erase(computers.begin() + i);
      }
    }
  }
  Computer editComputer(const std::wstring inventoryNumber,
                        const std::wstring macadress,
                        const Computer& updatedComputer) {
    for (const auto& c : computers) {
      if ((c.inventoryNumber == updatedComputer.inventoryNumber &&
           inventoryNumber != updatedComputer.inventoryNumber) ||
          (c.macAddress == updatedComputer.macAddress &&
           macadress != updatedComputer.macAddress)) {
        throw std::runtime_error(
            "Инвентаризационный номер и MAC-адрес должы быть уникальны");
      }
    }
    // Находим индекс и обновляем компьютер в коллекции
    for (int i = 0; i < computers.size(); ++i) {
      if (computers[i].inventoryNumber == inventoryNumber) {
        computers[i] = updatedComputer;
        return computers[i];  // Возвращаем обновленный объект
      }
    }
    throw std::runtime_error(
        "Компьютер с таким инвентаризационным номером не найден");
  }
  const std::vector<Computer>& getComputers() const;
  Computer getComputer(int index) const { return computers[index]; }

  size_t getComputersCount() const { return computers.size(); }

  std::vector<Computer> filterComputers(const std::wstring& criteria,
                                        const std::wstring& query) {
    std::vector<Computer> filtered;
    for (const auto& computer : computers) {
      bool match = false;
      if (criteria == L"Тип компьютера") {
        match = computer.type.find(query) != std::wstring::npos;
      } else if (criteria == L"Инв. номер") {
        match = computer.inventoryNumber.find(query) != std::wstring::npos;
      } else if (criteria == L"Производитель") {
        match = computer.manufacturer.find(query) != std::wstring::npos;
      } else if (criteria == L"Модель") {
        match = computer.model.find(query) != std::wstring::npos;
      } else if (criteria == L"MAC адрес") {
        match = computer.macAddress.find(query) != std::wstring::npos;
      } else if (criteria == L"ОС") {
        match = computer.os.find(query) != std::wstring::npos;
      } else if (criteria == L"Дата") {
        match = computer.date.find(query) != std::wstring::npos;
      }

      if (match) {
        filtered.push_back(computer);
      }
    }
    return filtered;
  }
};