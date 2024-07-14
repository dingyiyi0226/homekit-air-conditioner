#include "teco-ac.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include <HomeSpan.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>


namespace {

/**
 * The last nibble is the XOR of the previous nibbles
 *
 */
void UpdateChecksum(TecoAC::TecoMsg &message) {
  uint8_t checksum = 0;

  message.back() &= 0xF0;
  for (const auto &byte: message) {
    checksum ^= (byte >> 4);
    checksum ^= (byte & 0x0F);
  }
  message.back() += checksum;
}

std::string TecoMsgToHexString(const TecoAC::TecoMsg& message) {
  std::ostringstream oss;
  oss << "0x";
  
  for (const auto &byte: message) {
    oss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  }
  
  return oss.str();
}

}  // namespace


TecoAC::TecoAC(IRsend* ir) : ir_(ir) {
  // 26 degree, fan auto, swing auto
  default_message_ = {0x30, 0xBF, 0x3F, 0x3F, 0x1F, 0x1A, 0x3F, 0xE0, 0x88};
  current_message_ = default_message_;
}

void TecoAC::Send() {
  Send(current_message_);
}

void TecoAC::Send(TecoMsg &message) {
  ir_->sendGeneric(
    kHdrMark, kHdrSpace,
    kBitMark, kOneSpace,
    kBitMark, kZeroSpace,
    kBitMark, kDefaultMessageGap,
    message.data(), message.size(),
    kFreq, true, kNoRepeat, kDutyDefault
  );
  LOG1("Send msg [%s]\n", TecoMsgToHexString(message).c_str());
}

/**
 * Power on/off
 * Set the 2nd nibble to 0xF
 *
 */
void TecoAC::Power() {
  TecoMsg message = GetNewMessage(2, 0xF);
  Send(message);
}

/**
 * Set the 11th nibble
 * Values:
 *   16: 0x0
 *   31: 0xF
 *
 */
void TecoAC::SetTemperature(int temperature) {
  temperature = std::max(temperature, 16);
  temperature = std::min(temperature, 31);

  UpdateMessage(11, static_cast<uint8_t>(temperature - 16));
  Send();
}

/**
 * Set the 10th nibble
 * Values:
 *   Auto    0x1
 *   Low     0x3
 *   Midium  0x5
 *   High    0x7
 *
 */
void TecoAC::SetFan(FanType type) {
  switch (type) {
    case TecoAC::FAN_LOW:
      UpdateMessage(10, 0x3);
      break;
    case TecoAC::FAN_MIDIUM:
      UpdateMessage(10, 0x5);
      break;
    case TecoAC::FAN_HIGH:
      UpdateMessage(10, 0x7);
      break;
    case TecoAC::FAN_AUTO:
    default:
      UpdateMessage(10, 0x1);
      break;
  }
  Send();
}


/**
 * Set the 14th nibble
 * Values:
 *   Auto     0xE
 *   Highest  0x2
 *   High     0x4
 *   Middle   0x6
 *   Low      0x8
 *   Lowest   0xA
 *
 */
void TecoAC::SetSwing(SwingType type) {
  switch (type) {
  case TecoAC::SWING_HIGHEST:
    UpdateMessage(14, 0x2);
    break;
  case TecoAC::SWING_HIGH:
    UpdateMessage(14, 0x4);
    break;
  case TecoAC::SWING_MIDDLE:
    UpdateMessage(14, 0x6);
    break;
  case TecoAC::SWING_LOW:
    UpdateMessage(14, 0x8);
    break;
  case TecoAC::SWING_LOWEST:
    UpdateMessage(14, 0xA);
    break;
  case TecoAC::SWING_AUTO:
  default:
    UpdateMessage(14, 0xE);
    break;
  }
  Send();
}

void TecoAC::UpdateMessage(int nibble, uint8_t value) {
  // TODO: add mutex

  if (nibble > 17 || nibble < 0 || value > 0xF) {
    return;
  }

  int byte = nibble / 2;
  if (nibble % 2) {
    current_message_[byte] = (current_message_[byte] & 0xF0) + value;
  } else {
    current_message_[byte] = (current_message_[byte] & 0x0F) + (value << 4);
  }

  UpdateChecksum(current_message_);
}

TecoAC::TecoMsg TecoAC::GetNewMessage(int nibble, uint8_t value) const {
  TecoMsg message = current_message_;

  if (nibble > 17 || nibble < 0 || value > 0xF) {
    return message;
  }

  int byte = nibble / 2;
  if (nibble % 2) {
    message[byte] = (message[byte] & 0xF0) + value;
  } else {
    message[byte] = (message[byte] & 0x0F) + (value << 4);
  }

  UpdateChecksum(message);

  return message;
}
