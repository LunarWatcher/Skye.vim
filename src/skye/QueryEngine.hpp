#pragma once

#if defined(_MSC_VER)
#define NOVA_EXPORT extern "C" __declspec(dllexport)
#elif defined(__GNUC__)
#define NOVA_EXPORT extern "C" __attribute__((visibility("default")))
#else
#define NOVA_EXPORT extern "C"
#pragma warning "Couldn't determine export semantics for compiler. Please open an issue, or better, a PR"
#endif

namespace skye {

}
