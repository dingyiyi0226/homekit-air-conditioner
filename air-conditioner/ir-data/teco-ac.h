#include <array>
#include <IRsend.h>


class TecoAC {
public:
  enum FanType {
    FAN_AUTO,
    FAN_LOW,
    FAN_MIDIUM,
    FAN_HIGH,
  };

  enum SwingType {
    SWING_AUTO,
    SWING_HIGHEST,
    SWING_HIGH,
    SWING_MIDDLE,
    SWING_LOW,
    SWING_LOWEST,
  };

  TecoAC(IRsend* ir): ir_(ir), data_({}) {}

  void Send();

  void SetPower(int degree);
  void SetTemperature(int degree);
  void SetFan(FanType type);
  void SetSwing(SwingType type);

  const uint16_t kHdrMark = 3300;
  const uint32_t kHdrSpace = 1600;
  const uint16_t kBitMark = 400;
  const uint32_t kOneSpace = 1200;
  const uint32_t kZeroSpace = 400;
  const uint16_t kFreq = 38000;

private:
  IRsend* ir_;
  std::array<uint8_t, 9> data_;
};
