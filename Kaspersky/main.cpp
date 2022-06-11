#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

void translateFromGmiToHtml(std::ifstream& gmi_file)
{
  
}

void generateSite(const fs::path& input_directory, const fs::path& output_directory)
{
  try
  {
    for (const auto& dirEntry : fs::recursive_directory_iterator(input_directory))
    {
      const auto& path = dirEntry.path();
      const auto relativeSrc = fs::relative(path, input_directory);
      const auto targetParentPath = output_directory / relativeSrc;

      if (dirEntry.is_directory()) {  
        fs::create_directories(targetParentPath);
      }
      else {
        if (path.extension() == ".gmi") {
          translateFromGmiToHtml();
        } 
        else
        {
          fs::copy(path, targetParentPath, fs::copy_options::overwrite_existing);
        }
      }
    }
  }
  catch (const std::exception& a)
  {
    std::cerr << a.what();
  }
}

int main() {
  
  fs::path input_directory(fs::current_path() / "InputDirectory");
  //fs::path test = fs::relative(input_directory / "Images", input_directory);
  //std::cout << test;
  fs::path output_directory(fs::current_path() / "OutputDirectory");
  //std::cin >> input_directory;
  //std::cin >> output_directory;
  
  generateSite(input_directory, output_directory);
}