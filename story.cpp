#include "story.hpp"

void Story::make(std::string directory) {
  std::string file_name;
  file_name = directory + "/story.txt";
  std::ifstream f;
  f.open(file_name.c_str());
  assert(f.is_open());
  std::string line;
  while (getline(f, line)) {
    size_t atpos = line.find('@');
    size_t firstColon = line.find(':');
    size_t secondColon = line.find(':', firstColon + 1);
    size_t frontBracket = line.find('[');
    size_t backBracket = line.find(']');
    size_t dollarpos = line.find('$');
    if (line.size() == 0) {
      continue;
    }
    if (dollarpos != std::string::npos && dollarpos < atpos && dollarpos < firstColon) {
      addVariableToStory(line, dollarpos);
      continue;
    }
    if (atpos != std::string::npos && firstColon != std::string::npos && atpos < firstColon) {
      addPage(line, atpos, firstColon);
      continue;
    }
    if (firstColon != std::string::npos && secondColon != std::string::npos) {
      page_addChoice(line, firstColon, secondColon, frontBracket, backBracket);
      continue;
    }
    std::cerr << "Invalid story format." << std::endl;
    exit(EXIT_FAILURE);
  }
  f.close();
}
void Story::addPage(std::string & line, size_t atpos, size_t colonpos) {
  assert(line[atpos + 1] == 'N' || line[atpos + 1] == 'L' || line[atpos + 1] == 'W');
  assert(checkUnsignedNum(line.substr(0, atpos).c_str()));
  size_t page_num = strtoul(line.substr(0, atpos).c_str(), NULL, 0);
  if (line[atpos + 1] == 'N') {
    page_vec.push_back(new normalPage(page_num, line.substr(colonpos + 1)));
  }
  else if (line[atpos + 1] == 'L') {
    page_vec.push_back(new losePage(page_num, line.substr(colonpos + 1)));
  }
  else {
    page_vec.push_back(new winPage(page_num, line.substr(colonpos + 1)));
  }
  assert(page_num == page_vec.size() - 1);
}
void Story::addVariableToStory(std::string & line, size_t dollarpos) {
  assert(checkUnsignedNum(line.substr(0, dollarpos).c_str()));
  size_t page_num = strtoul(line.substr(0, dollarpos).c_str(), NULL, 0);
  (*page_vec[page_num]).addVariable(createPair(line.substr(dollarpos + 1).c_str()));
}
void Story::page_addChoice(std::string & line, size_t firstColon, size_t secondColon, size_t frontBracket, size_t backBracket) {
  size_t page_num;
  std::pair<std::string, long> variable;
  if (frontBracket < firstColon) {
    assert(checkUnsignedNum(line.substr(0, frontBracket).c_str()));
    assert(backBracket < firstColon);
    page_num = strtoul(line.substr(0, frontBracket).c_str(), NULL, 0);
    variable =
        createPair(line.substr(frontBracket + 1, backBracket - frontBracket - 1).c_str());
  }
  else {
    assert(checkUnsignedNum(line.substr(0, firstColon).c_str()));
    page_num = strtoul(line.substr(0, firstColon).c_str(), NULL, 0);
  }
  assert(checkUnsignedNum(
      line.substr(firstColon + 1, secondColon - firstColon - 1).c_str()));
  size_t choiceJumpPage =
      strtoul(line.substr(firstColon + 1, secondColon - firstColon - 1).c_str(), NULL, 0);
  std::string name = line.substr(secondColon + 1);
  assert(page_num < page_vec.size());
  assert((*page_vec[page_num]).getType() == 'N');
  (*page_vec[page_num]).addChoice(new Choice(name, choiceJumpPage, variable));
}
std::pair<std::string, long> Story::createPair(const std::string & line) {
  size_t equalpos = line.find('=');
  std::string variable_name = line.substr(0, equalpos);
  assert(checkAllNum(line.substr(equalpos + 1).c_str()));
  long varibale_val = strtol(line.substr(equalpos + 1).c_str(), NULL, 0);
  std::pair<std::string, long> temp;
  temp = std::make_pair(variable_name, varibale_val);
  return temp;
}
void Story::changeAllReference() {
  for (size_t i = 0; i < page_vec.size(); i++) {
    for (size_t j = 0; j < (*page_vec[i]).getC_vec().size(); j++) {
      size_t jumpPage = (*(*page_vec[i]).getC_vec()[j]).getJumpPage();
      assert(jumpPage < page_vec.size());
      (*page_vec[jumpPage]).changeReference();
    }
  }
}
void Story::checkReference() {
  for (size_t i = 0; i < page_vec.size(); i++) {
    assert((*page_vec[i]).getBeReferenced() == 1);
  }
}
bool Story::checkWinLose() {
  for (size_t i = 0; i < page_vec.size(); i++) {
    if ((*page_vec[i]).getType() == 'L' || (*page_vec[i]).getType() == 'W') {
      return true;
    }
  }
  return false;
}
void Story::print_page(std::string directory) {
  for (size_t i = 0; i < page_vec.size(); i++) {
    std::cout << "Page " << (*page_vec[i]).getNum() << std::endl;
    std::cout << "==========" << std::endl;
    print_content(directory, i);
  }
}
void Story::read_story(std::string directory) {
  size_t page_num = 0;
  print_content(directory, page_num);
  std::string line;
  size_t num;
  while ((*page_vec[page_num]).getType() != 'L' &&
         (*page_vec[page_num]).getType() != 'W') {
    assert(std::cin >> line);
    if (!checkUnsignedNum(line.c_str())) {
      num = 0;
    } else {
      num = strtoul(line.c_str(), NULL, 0);
    }
    if (num == 0 || num > (*page_vec[page_num]).getC_vec().size()) {
      std::cout << "That is not a valid choice, please try again" << std::endl;
    }
    else if (!checkAvailable(page_num, num - 1)) {
      std::cout << "That choice is not available at this time, please try again"
                << std::endl;
    }
    else {
      page_num = (*(*page_vec[page_num]).getC_vec()[num - 1]).getJumpPage();
      assert(page_num < page_vec.size());
      if ((*page_vec[page_num]).getVar_vec().size() > 0) {
        for (size_t i = 0; i < (*page_vec[page_num]).getVar_vec().size(); i++) {
          variable_map[(*page_vec[page_num]).getVar_vec()[i].first] =
              (*page_vec[page_num]).getVar_vec()[i].second;
        }
      }
      print_content(directory, page_num);
    }
  }
}
void Story::print_content(std::string directory, size_t i) {
  std::string file_name;
  file_name = directory + "/" + (*page_vec[i]).getFileName().c_str();
  std::ifstream f;
  f.open(file_name.c_str());
  assert(f.is_open());
  std::string line;
  std::stringstream output_text;
  while (getline(f, line)) {
    output_text << line << std::endl;
  }
  f.close();
  std::cout << output_text.str() << std::endl;
  if ((*page_vec[i]).getType() == 'N') {
    std::cout << "What would you like to do?"
              << "\n"
              << std::endl;
    for (size_t j = 0; j < (*page_vec[i]).getC_vec().size(); j++) {
      if ((*page_vec[i]).getC_vec()[j]->getVariable().first != "") {
        if (!checkAvailable(i, j)) {
          std::cout << " " << j + 1 << ". "
                    << "<UNAVAILABLE>" << std::endl;
        }
        else {
          std::cout << " " << j + 1 << ". " << (*(*page_vec[i]).getC_vec()[j]).getName()
                    << std::endl;
        }
      }
      else {
        std::cout << " " << j + 1 << ". " << (*(*page_vec[i]).getC_vec()[j]).getName()
                  << std::endl;
      }
    }
  }
  else if ((*page_vec[i]).getType() == 'L') {
    std::cout << "Sorry, you have lost. Better luck next time!" << std::endl;
  }
  else {
    std::cout << "Congratulations! You have won. Hooray!" << std::endl;
  }
}
int Story::checkAvailable(size_t i, size_t j) {
  std::string name = (*page_vec[i]).getC_vec()[j]->getVariable().first;
  long value = (*page_vec[i]).getC_vec()[j]->getVariable().second;
  if (name == "") {
    return true;
  }
  if (variable_map.find(name) != variable_map.end()) {
    if (value == variable_map[name]) {
      return true;
    }
  }
  else {
    if (value == 0) {
      return true;
    }
  }
  return false;
}
void Story::findPath() {
  std::queue<std::pair<size_t, std::string> > choices;
  std::vector<size_t> visited_page;
  std::pair<size_t, std::string> curr_page;
  curr_page = std::make_pair(0, "");
  addToQueue(choices, curr_page);
  while (choices.size() > 0) {
    curr_page = choices.front();
    choices.pop();
    if ((*page_vec[curr_page.first]).getType() == 'W') {
      std::stringstream ss;
      ss << curr_page.first;
      std::cout << curr_page.second << ss.str() << "(win)" << std::endl;
    }
    else {
      if (!checkInVector(curr_page.first, visited_page)) {
        addToQueue(choices, curr_page);
        visited_page.push_back(curr_page.first);
      }
    }
  }
}
int Story::checkInVector(size_t num, std::vector<size_t> & vector) {
  for (size_t i = 0; i < vector.size(); i++) {
    if (num == vector[i]) {
      return true;
    }
  }
  return false;
}
void Story::addToQueue(std::queue<std::pair<size_t, std::string> > & choices,
                       std::pair<size_t, std::string> & curr_page) {
  for (size_t i = 0; i < (*page_vec[curr_page.first]).getC_vec().size(); i++) {
    std::pair<size_t, std::string> temp;
    size_t jump_page_num = (*(*page_vec[curr_page.first]).getC_vec()[i]).getJumpPage();
    std::stringstream ss;
    ss << curr_page.second << curr_page.first << "(" << i + 1 << "),";
    temp = std::make_pair(jump_page_num, ss.str());
    choices.push(temp);
  }
}
int Story::countColon(const std::string & s) {
  int count = 0;
  for (size_t i = 0; i < s.length(); i++) {
    if (s[i] == ':') {
      count++;
    }
  }
  return count;
}
int Story::countDollarSign(const std::string & s) {
  int count = 0;
  for (size_t i = 0; i < s.length(); i++) {
    if (s[i] == '$') {
      count++;
    }
  }
  return count;
}
int Story::countEqualSign(const std::string & s) {
  int count = 0;
  for (size_t i = 0; i < s.length(); i++) {
    if (s[i] == '=') {
      count++;
    }
  }
  return count;
}
bool Story::checkAllNum(const std::string & s) {
  size_t i = 0;
  if (s.size() == 0) {
    return false;
  }
  while (i < s.size()) {
    if (!isdigit(s[i])) {
      if (i == 0 && (s[i] == '-' || s[i] == '+')) {
        continue;
      }
      return false;
    }
    i++;
  }
  return true;
}
bool Story::checkUnsignedNum(const std::string & s) {
  size_t i = 0;
  if (s.size() == 0) {
    return false;
  }
  while (i < s.size()) {
    if (!isdigit(s[i])) {
      if (i == 0 && s[i] == '+') {
        continue;
      }
      return false;
    }
    i++;
  }
  return true;
}
