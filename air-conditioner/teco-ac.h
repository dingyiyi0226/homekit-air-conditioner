#ifndef _TECO_AC_H_
#define _TECO_AC_H_

#include <array>
#include <mutex>

#include <IRsend.h>


class TecoAC {
public:
  using TecoMsg = std::array<uint8_t, 9>;  // 18 nibbles, last nibble is the checksum

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

  TecoAC(IRsend* ir);

  void Send();
  void Send(TecoMsg& message);

  void Power();
  void SetTemperature(int temperature);
  void SetFan(FanType type);
  void SetSwing(SwingType type);

  const uint16_t kHdrMark = 3300;
  const uint32_t kHdrSpace = 1600;
  const uint16_t kBitMark = 400;
  const uint32_t kOneSpace = 1200;
  const uint32_t kZeroSpace = 400;
  const uint16_t kFreq = 38000;

private:
  void UpdateMessage(int nibble, uint8_t value);
  TecoMsg GetNewMessage(int nibble, uint8_t value) const;

  IRsend* ir_;
  TecoMsg default_message_;
  TecoMsg current_message_;
  std::mutex message_lock_;
};

#endif  // _TECO_AC_H_
