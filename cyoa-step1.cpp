#include "story.hpp"

int main(int argc, char ** argv) {
  assert(argc == 2);
  Story story;
  std::string directory = argv[1];
  story.make(directory);
  story.print_page(directory);
  return EXIT_SUCCESS;
}
