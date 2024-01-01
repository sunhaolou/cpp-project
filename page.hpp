#ifndef __PAGE_HPP__
#define __PAGE_HPP__
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Choice {
 private:
  std::string name;
  size_t page_num;
  std::pair<std::string, long> variable;

 public:
  Choice() {}
  Choice(std::string _name, size_t _page_num, const std::pair<std::string, long> & pair) {
    name = _name;
    page_num = _page_num;
    variable = pair;
  }
  Choice(const Choice & other) {
    name = other.name;
    page_num = other.page_num;
  }
  Choice & operator=(const Choice & other) {
    if (this != &other) {
      name = other.name;
      page_num = other.page_num;
    }
    return *this;
  }
  ~Choice() {}
  std::string getName() const { return name; }
  size_t getJumpPage() const { return page_num; }
  std::pair<std::string, long> getVariable() const { return variable; }
};

class Page {
 protected:
  size_t num;
  char type;
  int be_referenced;  // Symbolize whether the page has been referenced by others or not.
  std::string file_name;
  std::vector<Choice *> c_vec;
  std::vector<std::pair<std::string, long> > variable_vec;

 public:
  Page() {}
  Page(size_t _num, char _type, std::string _file_name) {
    num = _num;
    type = _type;
    if (num == 0) {
      be_referenced = 1;
    }
    else {
      be_referenced = 0;
    }
    file_name = _file_name;
  }
  Page(const Page & other) {
    num = other.num;
    type = other.type;
    be_referenced = other.be_referenced;
    file_name = other.file_name;
  }
  Page & operator=(const Page & other) {
    if (this != &other) {
      num = other.num;
      type = other.type;
      be_referenced = other.be_referenced;
      file_name = other.file_name;
    }
    return *this;
  }
  virtual ~Page() {
    for (size_t i = 0; i < c_vec.size(); i++) {
      delete c_vec[i];
    }
    c_vec.clear();
  }
  size_t getNum() const { return num; }
  char getType() const { return type; }
  int getBeReferenced() const { return be_referenced; }
  // Change the page's referenced field to 1 if it is referenced by others.
  void changeReference() { be_referenced = 1; }
  std::string getFileName() const { return file_name; }
  const std::vector<Choice *> & getC_vec() const { return c_vec; }
  // Add choices to the current page.
  void addChoice(Choice * c) { c_vec.push_back(c); }
  const std::vector<std::pair<std::string, long> > & getVar_vec() const {
    return variable_vec;
  }
  // Add variable pair to the current page.
  void addVariable(const std::pair<std::string, long> & s) { variable_vec.push_back(s); }
};

class normalPage : public Page {
 public:
  normalPage(size_t _num, std::string _file_name) : Page(_num, 'N', _file_name) {}
  normalPage(const normalPage & other) {
    num = other.num;
    type = other.type;
    be_referenced = other.be_referenced;
    file_name = other.file_name;
    c_vec = other.c_vec;
  }
  normalPage & operator=(const normalPage & other) {
    if (this != &other) {
      num = other.num;
      type = other.type;
      be_referenced = other.be_referenced;
      file_name = other.file_name;
      c_vec = other.c_vec;
    }
    return *this;
  }
  ~normalPage() {}
};

class losePage : public Page {
 public:
  losePage(size_t _num, std::string _file_name) : Page(_num, 'L', _file_name) {}
  losePage(const losePage & other) {
    num = other.num;
    type = other.type;
    be_referenced = other.be_referenced;
    file_name = other.file_name;
  }
  losePage & operator=(const losePage & other) {
    if (this != &other) {
      num = other.num;
      type = other.type;
      be_referenced = other.be_referenced;
      file_name = other.file_name;
    }
    return *this;
  }
  ~losePage() {}
};

class winPage : public Page {
 public:
  winPage(size_t _num, std::string _file_name) : Page(_num, 'W', _file_name) {}
  winPage(const winPage & other) {
    num = other.num;
    type = other.type;
    be_referenced = other.be_referenced;
    file_name = other.file_name;
  }
  winPage & operator=(const winPage & other) {
    if (this != &other) {
      num = other.num;
      type = other.type;
      be_referenced = other.be_referenced;
      file_name = other.file_name;
    }
    return *this;
  }
  ~winPage() {}
};
#endif
