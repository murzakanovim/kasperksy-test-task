#include <filesystem>
#include <iostream>
#include "main.h"


namespace fs = std::filesystem;

fs::path translateFromGmiToHtml(fs::path p) {
  return p;
}

void generateSite(fs::path input_directory, fs::path output_directory) {

}

void goInDirectory(fs::path path) {
  for (auto& p : fs::recursive_directory_iterator(path)) {
  
  }
}

int main() {
  fs::path input_directory;
  fs::path output_directory;
  std::cin >> input_directory >> output_directory;
  generateSite(input_directory, output_directory);
}