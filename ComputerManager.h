#pragma once
#include <stdexcept>
#include <string>
#include <vector>

struct Computer {
  std::string type;
  std::string inventoryNumber;
  std::string manufacturer;
  std::string model;
  std::string macAddress;
  std::string os;
  std::string date;
};

class ComputerManager {
 private:
  std::vector<Computer> computers;

 public:
  void loadFromFile(const std::string& filename);
  void saveToFile(const std::string& filename);
  void addComputer(const Computer& computer);
  void removeComputer(const std::string& inventoryNumber);
  void deleteComputer(const std::string inventoryNumber) {
    for (int i = 0; i < computers.size(); ++i) {
      if (computers[i].inventoryNumber == inventoryNumber) {
        computers.erase(computers.begin() + i);
      }
    }
  }
  Computer editComputer(const std::string inventoryNumber, const Computer& updatedComputer) {
    for (const auto& c : computers) {
      if (c.inventoryNumber == updatedComputer.inventoryNumber &&
          inventoryNumber != updatedComputer.inventoryNumber) {
        throw std::runtime_error(
            "Инвентаризационный номер должен быть уникальным");
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
};