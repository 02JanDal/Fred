#pragma once

#include <fstream>

const char *readFile(const char *filename)
{
  std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
  file.seekg(0, std::ios::end);
  const size_t size = file.tellg();
  char *contents = new char[size];
  file.seekg(0, std::ios::beg);
  file.read(contents, size);
  file.close();
  return contents;
}
size_t fileSize(const char *filename)
{
  std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
  file.seekg(0, std::ios::end);
  const size_t size = file.tellg();
  file.close();
  return size;
}
