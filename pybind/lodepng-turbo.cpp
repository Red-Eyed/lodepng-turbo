#include "lodepng.h"
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <vector>

/*
3 ways to decode a PNG from a file to RGBA pixel data (and 2 in-memory ways).
*/

//g++ lodepng.cpp example_decode.cpp -ansi -pedantic -Wall -Wextra -O3


//Example 1
//Decode from disk to raw pixels with a single function call
std::vector<unsigned char> decode1(const char* filename)
{
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;

  //decode
  unsigned error = lodepng::decode(image, width, height, filename);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
  return image;
}

//Example 2
//Load PNG file from disk to memory first, then decode to raw pixels in memory.
std::vector<unsigned char> decode2(const char* filename)
{
  std::vector<unsigned char> png;
  std::vector<unsigned char> image; //the raw pixels
  unsigned width, height;

  //load and decode
  unsigned error = lodepng::load_file(png, filename);
  if(!error) error = lodepng::decode(image, width, height, png);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
  return image;
}

namespace py = pybind11;

// wrap C++ function with NumPy array
py::array py_decode1(const std::string & filename)
{
  // call pure C++ function
  std::vector<unsigned char> result_vec = decode1(filename.c_str());

  // allocate py::array (to pass the result of the C++ function to Python)
  auto result        = py::array_t<char>(result_vec.size());
  auto result_buffer = result.request();
  unsigned char *result_ptr    = (unsigned char *) result_buffer.ptr;

  // copy std::vector -> py::array
  std::memcpy(result_ptr,result_vec.data(),result_vec.size()*sizeof(unsigned char));

  return result;
}

py::array py_decode2(const std::string & filename)
{
  // call pure C++ function
  std::vector<unsigned char> result_vec = decode2(filename.c_str());

  // allocate py::array (to pass the result of the C++ function to Python)
  auto result        = py::array_t<char>(result_vec.size());
  auto result_buffer = result.request();
  unsigned char *result_ptr    = (unsigned char *) result_buffer.ptr;

  // copy std::vector -> py::array
  std::memcpy(result_ptr,result_vec.data(),result_vec.size()*sizeof(unsigned char));

  return result;
}

// wrap as Python module
PYBIND11_MODULE(lodepng ,m)
{
  m.def("decode1", &py_decode1, "Decode PNG with SIMD");
  m.def("decode2", &py_decode2, "Decode PNG with SIMD");
}