#ifndef HFX_COMMON_H
#define HFX_COMMON_H

#ifdef HFX_BUILD
    #define HFX_API __declspec(dllexport)
#else
      #define HFX_API __declspec(dllimport)
#endif

#endif //HFX_COMMON_H
