#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>

using password_differences = std::map<std::string, long>;

auto compare_strings(std::string const &s1, std::string const &s2,
                     long &difference) -> bool {
  bool any_common_chars = false;
  long const small_size = std::min(s1.size(), s2.size());
  long const large_size = std::max(s1.size(), s2.size());
  difference = large_size - small_size;
  for (long i = 0; i < small_size; i++) {
    if (s1.at(i) == s2.at(i)) {
      any_common_chars = true;
    } else {
      difference++;
    }
  }
  return any_common_chars;
}

auto get_similar_passwords(std::string const &password,
                           std::string const &file_name)
    -> password_differences {
  password_differences comparisons;
  std::ifstream passwords_file(file_name);
  std::string test_password = "";
  long smallest_difference = -1;
  while (passwords_file >> test_password) {
    long difference = 0;
    if (compare_strings(password, test_password, difference)) {
      if (smallest_difference == -1 || difference < smallest_difference) {
        smallest_difference = difference;
        comparisons.clear();
      }
      if (difference == smallest_difference) {
        comparisons.insert(std::make_pair(test_password, difference));
      }
    }
  }
  passwords_file.close();
  if (comparisons.empty()) {
    comparisons.insert(std::make_pair(password, long(0)));
  }
  return comparisons;
}

auto print_password_differences(password_differences const &differences)
    -> void {
  std::for_each(
      std::begin(differences), std::end(differences),
      [](std::pair<std::string, long> p) { std::cout << p.first << ", "; });
  std::cout << std::endl;
}

int main() {
  std::string user_password = "";
  std::cout << "Give me a password:";
  std::getline(std::cin, user_password);
  std::cout << std::endl;
  std::cout << "You provided a password of " << user_password << std::endl;
  password_differences similar_to_common =
      get_similar_passwords(user_password, "common_passwords.txt");
  std::cout << "The most similar passwords to " << user_password
            << " are:" << std::endl;
  print_password_differences(similar_to_common);
  long const character_difference = std::begin(similar_to_common)->second;
  std::cout << "All of which are " << character_difference
            << " character(s) different." << std::endl;
}