#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//   fxc /Evs_main /Tvs_3_0 /FhInclude/DepthCopyVS.h /VnDepthCopyVSData
//    Source/DepthCopy.hlsl /O3
//
    vs_3_0
    def c0, 1, 0, 0, 0
    dcl_position v0
    dcl_texcoord v1
    dcl_position o0
    dcl_texcoord o1.xy
    mad o0, v0.xyzx, c0.xxxy, c0.yyyx
    mov o1.xy, v1

// approximately 2 instruction slots used
#endif

const BYTE DepthCopyVSData[] =
{
      0,   3, 254, 255, 254, 255, 
     22,   0,  67,  84,  65,  66, 
     28,   0,   0,   0,  35,   0, 
      0,   0,   0,   3, 254, 255, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0, 129,   0,   0, 
     28,   0,   0,   0, 118, 115, 
     95,  51,  95,  48,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
     32,  72,  76,  83,  76,  32, 
     83, 104,  97, 100, 101, 114, 
     32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  57,  46, 
     50,  57,  46,  57,  53,  50, 
     46,  51,  49,  49,  49,   0, 
     81,   0,   0,   5,   0,   0, 
     15, 160,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   0,   0,  15, 144, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   1,   0,  15, 144, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   0,   0,  15, 224, 
     31,   0,   0,   2,   5,   0, 
      0, 128,   1,   0,   3, 224, 
      4,   0,   0,   4,   0,   0, 
     15, 224,   0,   0,  36, 144, 
      0,   0,  64, 160,   0,   0, 
     21, 160,   1,   0,   0,   2, 
      1,   0,   3, 224,   1,   0, 
    228, 144, 255, 255,   0,   0
};
