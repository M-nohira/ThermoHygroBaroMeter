# ThermoHygroBaroMeter
![ThermoHygroBaroMeter](/image/DSC_0850.jpg "温湿度気圧計")

スモークアクリルを使って少しおしゃれな置物風

## 概要
`ThermoHygroBaroMeter`は温度,湿度,気圧を表示するマルチ計測器です.

## 回路図
![ThermoHygorBaroMeterCircuit](/image/ThermoHygorBaroMeter.pdf "回路図")

いたって普通の回路図です。

ハードウェアSPIを使うことで通信速度を向上させています。

## ソフトウェア
使用するATmega328p-puはArduinoブートローダを焼きこんで使用してください.

また各ピン配置は` define `ステートメントで変更可能です.

adafuitsのBME280ライブラリを使用しています. インストールしてください.
