/*
Copyright (C) 2015 Nacho Fuentes Talens
Header file with typedefs
*/
namespace NachosEngine {

#ifndef __ENUMS__
#define __ENUMS__ 1

  enum Mode {
    kPoints = 1,
    kLines,
    kLineStrip,
    kLineLoop,
    kLineStripAdjency,
    kLinesAdjency,
    kTriangleStrip,
    kTriangleFan,
    kTriangles,
    kTriangleStripAdjency,
    kTrianglesAdjency
  };

  enum AttribName {
    kAttributeName_None = 0,
    kAttributeName_Position,
    kAttributeName_Color,
    kAttributeName_Normal,
    kAttributeName_UV,
    kAttributeName_Max
  };
  enum AttribType {
    kAttributeType_None = 0,
    kAttributeType_Float,
    kAttributeType_Float2,
    kAttributeType_Float3,
    kAttributeType_Float4
  };

  typedef AttribType AttribMask[kAttributeName_Max];

#if defined(NACHOSENGINE_PLATFORM_WINDOWS)
#include <stdint.h>
  typedef int8_t int8;
  typedef int16_t int16;
  typedef int32_t int32;
  typedef int64_t int64;
  typedef uint8_t uint8;
  typedef uint16_t uint16;
  typedef uint32_t uint32;
  typedef uint64_t uint64;
  typedef float float32;
  typedef double float64;

  
#elif defined(NACHOSENGINE_PLATFORM_XBOX_ONE)
#elif defined(NACHOSENGINE_PLATFORM_VITA)
#elif defined(NACHOSENGINE_PLATFORM_PS4)
#else
  //#error "PLATFORM_XXX not defined"
#endif

#endif
}