//
// Created by Admin on 12/03/2025.
//

// TODO
// [x] 0. var
// [ ] 1. template
// [ ] 2. function
//     [x] a. basic
//     [ ] b. overload
// [ ] 3. base
// [ ] 4. enum
// [ ] 5. main arguments parse

#include <fstream>
#include <iostream>

#include "AutoRefl.h"

using namespace My::MySRefl;
using namespace std;

string ReadFileIntoString(const char* filename) {
  ifstream ifile(filename);
  if (!ifile.is_open()) return "";

  ostringstream buf;
  char ch;
  while (buf && ifile.get(ch)) buf.put(ch);
  return buf.str();
}

int main(int argc, char** argv) {
  if (argc != 3) {
    cerr << "arguments error" << endl;
    return 1;
  }

  string inputPath = argv[1];
  string outputPath = argv[2];

  auto code = ReadFileIntoString(inputPath.c_str());

  AutoRefl autorefl;
  auto result = autorefl.Parse(code);
  auto curout = ReadFileIntoString(inputPath.c_str());
  if (curout == result) return 0;

  ofstream out(outputPath);
  if (!out.is_open()) {
    cerr << "open fail : " << outputPath << endl;
    return 1;
  }

  out << result;
  out.close();

  return 0;
}
