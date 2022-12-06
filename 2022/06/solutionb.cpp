#include <iostream>
#include <string>
#include <unordered_set>

#define WINDOW_SIZE 14

int main()
{
  std::string line;
  std::getline(std::cin, line);

  if (line.length() < WINDOW_SIZE) {
    std::cout << "The line must be at least " << WINDOW_SIZE
      << " characters long" << std::endl;
    return EXIT_FAILURE;
  }

  auto it = line.begin() + WINDOW_SIZE;
  std::unordered_set<char> window(it - WINDOW_SIZE, it);

  for (; it != line.end() && window.size() != WINDOW_SIZE; ++it) {
    window.clear();
    window.insert(it - WINDOW_SIZE, it);
  }

  std::cout << it - line.begin() - 1 << std::endl;
  return EXIT_SUCCESS;
}
