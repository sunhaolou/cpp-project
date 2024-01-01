#include "story.hpp"

int main(int argc, char ** argv) {
  assert(argc == 2);
  Story story;
  std::string directory = argv[1];
  story.make(directory);
  story.changeAllReference();
  story.checkReference();
  assert(story.checkWinLose() == 1);
  story.read_story(directory);
  return EXIT_SUCCESS;
}
