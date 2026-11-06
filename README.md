# XIAO Smart IR Mate — AUX/YKR-L iFeel sender

Manual iFeel temperature sender for AUX Q-SMART (YKR-L/101E, ELECTRA_AC)
on Seeed XIAO Smart IR Mate (ESP32-C3, TX = GPIO3).

## Files

- `xiao-ifeel-single.yaml` — **copy-paste into HA ESPHome.** No extra files.
  Number slider 16–30 °C + button sending silent ELECTRA_AC sensor update.
- `xiao-ifeel-esphome.yaml` + `electra_ifeel.h` — two-file version using
  IRremoteESP8266 `IRElectraAc` directly (full ON + silent update).

## Use as GitHub package

Push this folder to a repo, then in HA ESPHome create an empty device
and paste only:

```yaml
packages:
  - url: https://github.com/YOU/xiao-ifeel
    file: xiao-ifeel-single.yaml
    ref: main
```

Edit `YOU/xiao-ifeel` to your repo. The package pulls everything else.

## Copy-paste single file

Open `xiao-ifeel-single.yaml`, copy all, HA > ESPHome Builder >
New Device > Seeed XIAO ESP32C3 > Edit > replace all > Install >
`Plug into this computer` > pick COM7 (USB VID_303A PID_1001).

You need in ESPHome secrets:

```yaml
wifi_ssid: "Your_2.4GHz_SSID"
wifi_password: "YourPassword"
```

## Protocol notes

- Silent update template (104 bit, LSB-first per byte):
  `C3 5F E0 40 60 00 88 [sensor+0x4A] 00 30 00 1E [sum&0xFF]`
- Timings: hdr 9166/4470, bit 646, one 1647, zero 547, 38 kHz.
- Checksum = sum of bytes 0–11 & 0xFF (verified against
  IRremoteESP8266 dumps: 27 °C → `...65...DD`, 28 °C → `...66...DE`).
- AC must have IFeel ON first (press IFeel on YKR-L remote once),
  then silent updates change the regulating temp with no beep.
