#pragma once
#include <string>
#include <vector>
#include <stdexcept>

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
  void deleteComputer(int index_nuber) {
      computers.erase(computers.begin() + index_nuber);
  }
  Computer editComputer( int index, const Computer& updatedComputer ){ 
      for (const auto& c : computers) {
      if (c.inventoryNumber == updatedComputer.inventoryNumber)
        throw std::runtime_error("Inventory number must be unique");
    }
      return computers[index] = updatedComputer;
  }
  const std::vector<Computer>& getComputers() const;
  Computer getComputer(int index) const { return computers[index]; }
};