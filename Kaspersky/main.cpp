#include <filesystem>
#include <iostream>

int main() {
  namespace fs = std::filesystem;
  fs::directory_iterator a;
  std::cout << fs::current_path();
}