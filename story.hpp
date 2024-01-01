#ifndef __STORY_HPP__
#define __STORY_HPP__
#include <algorithm>
#include <map>
#include <queue>

#include "page.hpp"

class Story {
 private:
  std::vector<Page *> page_vec;
  std::vector<std::string> paths;
  std::map<std::string, long> variable_map;

 public:
  Story() {}
  Story(const Story & other) { page_vec = other.page_vec; }
  Story & operator=(const Story & other) {
    if (this != &other) {
      page_vec = other.page_vec;
    }
    return *this;
  }
  ~Story() {
    for (size_t i = 0; i < page_vec.size(); i++) {
      delete page_vec[i];
    }
    page_vec.clear();
  }
  // Make the story's pages based on the input directory.
  void make(std::string directory);
  // The helper function to add pages to the story.
  void addPage(std::string & line, size_t atpos, size_t colonpos);
  // The helper function to add variables to the story.
  void addVariableToStory(std::string & line, size_t dollarpos);
  // The helper function to add choices to pages.
  void page_addChoice(std::string & line, size_t firstColon, size_t secondColon, size_t frontBracket, size_t backBracket);
  // The helper function to create variable pairs.
  std::pair<std::string, long> createPair(const std::string & line);
  // Change all the pages' reference based on the pages' choices.
  void changeAllReference();
  // Check if all the pages has been referenced by others.
  void checkReference();
  // Check if there exists at least one win page and one lose page.
  bool checkWinLose();
  // Print all the pages based on the input directory.
  void print_page(std::string directory);
  // Print the story based on the input directory and the user's input.
  void read_story(std::string directory);
  // The helper function to help the print the contents.
  void print_content(std::string directory, size_t i);
  // The helper function to check whether the current choice is available or not.
  int checkAvailable(size_t i, size_t j);
  // Find all paths to win pages.
  void findPath();
  // The helper function to check whether the current page_num has already been visited or not.
  int checkInVector(size_t num, std::vector<size_t> & vector);
  // The helper function to add all choices of the current page to queue including its current path.
  void addToQueue(std::queue<std::pair<size_t, std::string> > & choices,
                  std::pair<size_t, std::string> & curr_page);
  // The helper function to count how many ":" exist in current string.
  int countColon(const std::string & s);
  // The helper function to count how many "$" exist in current string.
  int countDollarSign(const std::string & s);
  // The helper function to count how many "=" exist in current string.
  int countEqualSign(const std::string & s);
  // The helper function to check whether the string is an integer.
  bool checkAllNum(const std::string & s);
  // The helper fucntion to check whether the string is a non-negative integer.
  bool checkUnsignedNum(const std::string & s);
};
#endif
