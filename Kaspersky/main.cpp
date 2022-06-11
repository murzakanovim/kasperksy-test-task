#include <filesystem>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;


std::string convert_single_line(const std::string& gmi_line)
{

  std::map<std::string, std::string> tags_map;
  tags_map.insert(std::make_pair("^# (.*)", "h1"));
  tags_map.insert(std::make_pair("^## (.*)", "h2"));
  tags_map.insert(std::make_pair("^### (.*)", "h3"));
  tags_map.insert(std::make_pair("^\\* (.*)", "li"));
  tags_map.insert(std::make_pair("^> (.*)", "blockquote"));
  tags_map.insert(std::make_pair("^=>\\s*(\\S+)(\\s+.*)?", "a"));
  std::string result;

  for (auto tags_pair : tags_map) {
    if (std::regex_match(gmi_line, std::regex(tags_pair.first)))
    {
      std::string tag = tags_map[tags_pair.first];
      std::stringstream ss(gmi_line);
      if (tag == "a")
      {
        std::string href;
        std::string reference_name;
        ss.get(); // TO DO как красивее
        ss.get(); // 
        ss >> href;
        getline(ss >> std::ws, reference_name);
        
        result = "<" + tag + " href=\"" + href + "\">" + reference_name + "</" + tag + ">";
        
        return result;
      }
      std::string text;
      ss >> text; // пропустить первый символ
      getline(ss >> std::ws, text); // прочитать до конца
      result = "<" + tag + ">" + text + "</" + tag + ">";
      return result;
    }
  }
  result = "<p>" + gmi_line + "</p>";
  return result;
}

void translateFromGmiToHtml(const fs::path& src, const fs::path& target)
{
  std::ifstream gmi_file(src);
  std::ofstream html_file(target);

  if (!gmi_file.is_open())
  {
    throw std::runtime_error("Aboba");
  }

  std::string gmi_line;
  std::string html_line;
  bool isPreformatted = false;
  bool is_list = false;
  while (std::getline(gmi_file, gmi_line)) {
    if (gmi_line.length() >= 3 && gmi_line[0] == '`' && gmi_line[1] == '`' && gmi_line[2] == '`')
    {
      isPreformatted = !isPreformatted;
      std::string pre_tag = isPreformatted ? "<pre>" : "</pre>";
      html_file << pre_tag << "\n";
    }
    else if (isPreformatted) {
      html_file << gmi_line << "\n";
    }
    else
    {
      html_line = convert_single_line(gmi_line);
      if (html_line[1] == 'l' && html_line[2] == 'i') // TO DO::
      {
        if (!is_list) {
          is_list = true;
          html_file << "<ul>\n";
        }
      }
      else if (is_list)
      {
        is_list = false;
        html_file << "</ul>\n";
      }
      html_file << html_line << "\n";
    }
  }
  html_file.close();
}

void generateSite(const fs::path& input_directory, const fs::path& output_directory)
{
  try
  {
    for (const auto& dirEntry : fs::recursive_directory_iterator(input_directory))
    {
      
      const fs::path current_path = dirEntry.path();
      const fs::path relative_src = fs::relative(current_path, input_directory);
      const fs::path target_path = output_directory;
      if (dirEntry.is_directory()) {
        fs::create_directories(target_path / relative_src);
      }
      else
      {
        if (current_path.extension() == ".gmi")
        {
          fs::path relative_html_file = fs::relative(current_path, input_directory).parent_path();
          std::string filename = current_path.stem().string() + ".html";
          translateFromGmiToHtml(current_path, target_path / relative_html_file / filename);
        }
        else
        {
          fs::copy(current_path, target_path / relative_src, fs::copy_options::overwrite_existing);
        }
      }
    }
  }
  catch (const std::exception& exc)
  {
    std::cerr << exc.what();
  }
}

int main() {

  fs::path input_directory(fs::current_path() / "InputDirectory");
  fs::path output_directory(fs::current_path() / "OutputDirectory");
  // std::cin >> input_directory;
  // std::cin >> output_directory;
  generateSite(input_directory, output_directory); 
}