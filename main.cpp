#include <cstddef>
#include <cstdlib>
#include <string>

#include "Vector.h"
#include "LinkedList.h"
#include "time_tests.h"
#include <iostream>
namespace
{

template <typename T>
using LinearCollection = aisdi::LinkedList<T>;
template <typename Typ>
using myVector = aisdi::Vector<Typ>;

void perfomTest()
{
  LinearCollection<std::string> collection;
  collection.append("TODO");
}

}

int main(int argc, char *argv[])
{

  const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 10000;
  for (std::size_t i = 0; i < repeatCount; ++i)
    perfomTest();

    test_all();
  return 0;
}
