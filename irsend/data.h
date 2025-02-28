#include <array>
#include <cstdlib>


namespace TecoData {
  void power(int degree, std::array<uint8_t, 9> &data) {
    switch (degree) {
      case 25:
        data = {0x30, 0xFF, 0x3F, 0x3F, 0x1F, 0x19, 0x3F, 0x20, 0x83};
        break;
      case 26:
        data = {0x30, 0xFF, 0x3F, 0x3F, 0x1F, 0x1A, 0x3F, 0x20, 0x80};
        break;
      case 27:
        data = {0x30, 0xFF, 0x3F, 0x3F, 0x1F, 0x1B, 0x3F, 0x20, 0x81};
        break;
      case 28:
        data = {0x30, 0xFF, 0x3F, 0x3F, 0x1F, 0x1C, 0x3F, 0x20, 0x86};
        break;
      default:  // degree = 26
        data = {0x30, 0xFF, 0x3F, 0x3F, 0x1F, 0x1A, 0x3F, 0x20, 0x80};
        break;
    }
  }
}
