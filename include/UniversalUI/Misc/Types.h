//  Types.h - UniversalUI
//  first created by seb hall on 13/09/2023
//
//  UniversalUI is a project aiming to bring
//  highly flexible and efficient rendering
//  to all platforms simultaneously with a 
//  cross-platform API.
//	
//	This file contains type definitions for 
//	a few different numbers and other types, 
//	purely for the sake of convenience and
//	improving readability of code.

#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <vector>
#include <string>

//	NUMBERS:

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

//	OTHER:

typedef std::string string;

template<typename T>
using vector = std::vector<T>;

#endif // TYPES_H