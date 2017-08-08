## CHIP_IO API Documentation

* [CHIP_IO.GPIO](./gpio.md)
* [CHIP_IO.PWM](./pwm.md)
* [CHIP_IO.SOFTPWM](./softpwm.md)
* [CHIP_IO.SERVO](./servo.md)
* [CHIP_IO.LRADC](./lradc.md)
* [CHIP_IO.Utilities](./utilities.md)
* [CHIP_IO.OverlayManager](./overlaymanager.md)

### Allowable Pin Names and Edge Detection Capability

The following "table" is the allowable pin names that are able to be used by the library. The Name column is the normal name used on the CHIP Headers, the Alt Name column is the value used by the PocketCHIP header (if it's broken out), and the Key is the Header and Pin Number the the Pin is physically located.  Either of these 3 means is able to specify a pin in CHIP_IO.

  | CHIP (Main Name) | PocketCHIP/CHIP Pro Name | Key (Alt Name) | HW Support      | Edge Detect     |
  |:----------------:|:------------------------:|:--------------:|:---------------:|:---------------:|
  | TWI1-SDA         | KPD-I2C-SDA              | U13_9          | CHIP/CHIP PRO   | NO              |
  | TWI1-SCK         | KPD-I2C-SCL              | U13_11         | CHIP/CHIP PRO   | NO              |
  | LCD-D2           | UART2-TX                 | U13_17         | CHIP/CHIP PRO   | NO              |
  | PWM0             | PWM0                     | U13_18         | CHIP/CHIP PRO   | NO              |
  | PWM1             | PWM1                     | EINT13         | CHIP PRO        | YES             |
  | LCD-D4           | UART2-CTS                | U13_19         | CHIP/CHIP PRO   | NO              |
  | LCD-D3           | UART2-RX                 | U13_20         | CHIP/CHIP PRO   | NO              |
  | LCD-D6           | LCD-D6                   | U13_21         | CHIP            | NO              |
  | LCD-D5           | UART2-RTS                | U13_22         | CHIP/CHIP PRO   | NO              |
  | LCD-D10          | LCD-D10                  | U13_23         | CHIP            | NO              |
  | LCD-D7           | LCD-D7                   | U13_24         | CHIP            | NO              |
  | LCD-D12          | LCD-D12                  | U13_25         | CHIP            | NO              |
  | LCD-D11          | LCD-D11                  | U13_26         | CHIP            | NO              |
  | LCD-D14          | LCD-D14                  | U13_27         | CHIP            | NO              |
  | LCD-D13          | LCD-D13                  | U13_28         | CHIP            | NO              |
  | LCD-D18          | LCD-D18                  | U13_29         | CHIP            | NO              |
  | LCD-D15          | LCD-D15                  | U13_30         | CHIP            | NO              |
  | LCD-D20          | LCD-D20                  | U13_31         | CHIP            | NO              |
  | LCD-D19          | LCD-D19                  | U13_32         | CHIP            | NO              |
  | LCD-D22          | LCD-D22                  | U13_33         | CHIP            | NO              |
  | LCD-D21          | LCD-D21                  | U13_34         | CHIP            | NO              |
  | LCD-CLK          | LCD-CLK                  | U13_35         | CHIP            | NO              |
  | LCD-D23          | LCD-D23                  | U13_36         | CHIP            | NO              |
  | LCD-VSYNC        | LCD-VSYNC                | U13_37         | CHIP            | NO              |
  | LCD-HSYNC        | LCD-HSYNC                | U13_38         | CHIP            | NO              |
  | LCD-DE           | LCD-DE                   | U13_40         | CHIP            | NO              |
  | UART1-TX         | UART-TX                  | U14_3          | CHIP/CHIP PRO   | NO              |
  | UART1-RX         | UART-RX                  | U14_5          | CHIP/CHIP PRO   | NO              |
  | LRADC            | ADC                      | U14_11         | CHIP/CHIP PRO   | NO              |
  | XIO-P0           | XIO-P0                   | U14_13         | CHIP            | YES             |
  | XIO-P1           | XIO-P1                   | U14_14         | CHIP            | YES             |
  | XIO-P2           | GPIO1                    | U14_15         | CHIP            | YES             |
  | XIO-P3           | GPIO2                    | U14_16         | CHIP            | YES             |
  | XIO-P4           | GPIO3                    | U14_17         | CHIP            | YES             |
  | XIO-P5           | GPIO4                    | U14_18         | CHIP            | YES             |
  | XIO-P6           | GPIO5                    | U14_19         | CHIP            | YES             |
  | XIO-P7           | GPIO6                    | U14_20         | CHIP            | YES             |
  | AP-EINT1         | KPD-INT                  | U14_23         | CHIP/CHIP PRO   | YES             |
  | AP-EINT3         | AP-INT3                  | U14_24         | CHIP/CHIP PRO   | YES             |
  | TWI2-SDA         | I2C-SDA                  | U14_25         | CHIP/CHIP PRO   | NO              |
  | TWI2-SCK         | I2C-SCL                  | U14_26         | CHIP/CHIP PRO   | NO              |
  | CSIPCK           | SPI-SEL                  | U14_27         | CHIP/CHIP PRO   | NO              |
  | CSICK            | SPI-CLK                  | U14_28         | CHIP/CHIP PRO   | NO              |
  | CSIHSYNC         | SPI-MOSI                 | U14_29         | CHIP/CHIP PRO   | NO              |
  | CSIVSYNC         | SPI-MISO                 | U14_30         | CHIP/CHIP PRO   | NO              |
  | CSID0            | D0                       | U14_31         | CHIP/CHIP PRO   | NO              |
  | CSID1            | D1                       | U14_32         | CHIP/CHIP PRO   | NO              |
  | CSID2            | D2                       | U14_33         | CHIP/CHIP PRO   | NO              |
  | CSID3            | D3                       | U14_34         | CHIP/CHIP PRO   | NO              |
  | CSID4            | D4                       | U14_35         | CHIP/CHIP PRO   | NO              |
  | CSID5            | D5                       | U14_36         | CHIP/CHIP PRO   | NO              |
  | CSID6            | D6                       | U14_37         | CHIP/CHIP PRO   | NO              |
  | CSID7            | D7                       | U14_38         | CHIP/CHIP PRO   | NO              |
  | I2S-MCLK         | EINT19                   | 21             | CHIP PRO        | YES             |
  | I2S-BCLK         | I2S-BCLK                 | 22             | CHIP PRO        | NO              |
  | I2S-LCLK         | I2S-LCLK                 | 23             | CHIP PRO        | NO              |
  | I2S-DO           | EINT19                   | 24             | CHIP PRO        | NO              |
  | I2S-DI           | EINT24                   | 25             | CHIP PRO        | YES             |

