# Teco Air Conditioner

The IR signal of Teco A/C

## Signal

Only decode the temperature, fan and swing currently

- 18 nibbles (9 bytes), the last nibble served as the checksum
- Default signal `0x30BF3F3F1F1A3FE088`: 26˚C, fan auto mode, swing auto mode


In the explanation below, the index of the nibble starts from 0.

- On/Off: Set the 2nd nibble to `0xF`

- Temperature: Set the 11th nibble
    
    - 16 ~ 31˚C:  `0x0` ~ `0xF` 
    
- Fan: Set the 10th nibble

  | Mode   | value |
  | ------ | ----- |
  | Auto   | `0x1` |
  | Low    | `0x3` |
  | Medium | `0x5` |
  | High   | `0x7` |

- Swing: Set the 14th nibble

  | Mode    | Value |
  | ------- | ----- |
  | Auto    | `0xE` |
  | Highest | `0x2` |
  | High    | `0x4` |
  | Middle  | `0x6` |
  | Low     | `0x8` |
  | Lowest  | `0xA` |

  
- Checksum: The last nibble is the XOR of the previous 17 nibbles (from index 0 to 16)
