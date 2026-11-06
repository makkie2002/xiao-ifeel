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

## Protocol notes (verified by capture 2026-11-06, YKR-L/101E)

- Silent sensor update (104 bit, LSB-first per byte), template cloned
  from a real YKR-L/101E silent frame:
  `C3 8F E0 40 60 80 88 [sensor+0x4A] 00 30 00 1E [sum&0xFF]`
  Sensor value is **integer degrees only** (byte 7, no half-bit).
- Full-state setpoint **does 0.5 °C**: Temp field (byte 1, bits 3–7)
  holds `int_part − 8`, and **byte 3 bit 7 (`0x80`) = +0.5 °C**.
  Proven: 23.0/23.5 share byte 1 `0x7F` and differ only in byte 3
  `0x00`/`0x80`; same for 24.0/24.5 (`0x87`) — all `CKSUM_OK`.
  This half-degree bit is not implemented in IRremoteESP8266.
- Mode lives in byte 6 bits 5–7 (Heat=4, Cool=1, Dry=2, Auto=0,
  Fan=6), fan in byte 4 bits 5–7 (Auto=5, Low=3, Med=2, High=1),
  power is byte 9 bit 5. Template bytes cloned from real frames.
- Timings: hdr 9166/4470, bit 646, one 1647, zero 547, 38 kHz.
- Checksum = sum of bytes 0–11 & 0xFF (verified against remote +
  IRremoteESP8266 dumps: 27 °C → `...65...DD`, 28 °C → `...66...DE`).
- AC must have IFeel ON first (press IFeel on YKR-L remote once),
  then silent updates change the regulating temp with no beep.

## Entities

- Number `AC setpoint` (16–30, 0.5 step) + Selects `AC mode` /
  `AC fan` + Buttons `AC ON (apply mode + setpoint)` / `AC OFF` —
  full-state commands with .5 °C support (swing fixed off).
- Number `iFeel sensor temperature` + Button `Send sensor temp` —
  manual silent update. `room_temp` (Toon sensor) auto-sends every
  5 min when the rounded value changes.
