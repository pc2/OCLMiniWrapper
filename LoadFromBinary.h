
#ifndef OCLWRAP_LOADFROMBINARY_H
#define OCLWRAP_LOADFROMBINARY_H


#include <iostream>
#include <string>
#include <CL/cl.hpp>
#include <stdexcept>
#include <vector>

#ifdef _WIN32 // Windows
#include <windows.h>
#else         // Linux
#include <stdio.h> 
#include <unistd.h> // readlink, chdir
#endif

#include "OCLErrorToException.h"

namespace OCLWRAP{

// Loads a file in binary form.
inline std::vector<unsigned char> loadBinaryFile(const char *file_name) {
  // Open the File
  FILE* fp;
#ifdef _WIN32
  if(fopen_s(&fp, file_name, "rb") != 0) {
    return NULL;
  }
#else
  fp = fopen(file_name, "rb");
  if(fp == 0) {
    throw std::runtime_error("unable to load binary file: "+std::string(file_name));
  }
#endif

  // Get the size of the file
  fseek(fp, 0, SEEK_END);
  auto size = ftell(fp);

  std::vector<unsigned char> binary(size);

  // Go back to the file start
  rewind(fp);

  // Read the file into the binary
  if(fread((void*)binary.data(), size, 1, fp) == 0) {
    fclose(fp);
    throw std::runtime_error("unable to load binary file: "+std::string(file_name));
  }

  return binary;
}

inline bool fileExists(const char *file_name) {
#ifdef _WIN32 // Windows
  DWORD attrib = GetFileAttributesA(file_name);
  return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
#else         // Linux
  return access(file_name, R_OK) != -1;
#endif
}


// Create a program for all devices associated with the context.
inline cl_program loadFromBinary(cl_context context, const char *binary_file_name, const cl_device_id *devices) {
  // Early exit for potentially the most common way to fail: AOCX does not exist.
  if(!fileExists(binary_file_name)) {
    std::cout<<( "Failed to load binary file (does not exist?): "+std::string(binary_file_name));
    OCLErrorToException(CL_INVALID_PROGRAM);
  }

  // Load the binary.
  std::vector<unsigned char> binary = loadBinaryFile(binary_file_name);
  const unsigned char* binary_data = binary.data();


  size_t binary_size = binary.size();


  cl_int status;
  cl_program program = clCreateProgramWithBinary(context, 1, devices, &binary_size,
      &binary_data, &status, &status);
  if (status != 0) {
    std::cout<<"createProgramFromBinary file exists, but contents are invalid ("<<binary_file_name<<")\n";
  }
  OCLErrorToException(status);

  return program;
}


}


#endif