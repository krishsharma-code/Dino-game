# Mini Dino Game — Arduino Uno + 16x2 LCD

A single-button endless runner built on an Arduino Uno and a 16x2 character LCD.
The dinosaur and cactus are drawn as custom 5×8 characters stored in the LCD's CGRAM.
Press the rotary-encoder button to start, jump and restart.

This folder is the **16x2 LCD version** of the project. The files in the parent
folder (`Dino.ino`, `TM1637Display.*`) are the older **OLED version** and will not
run on a 16x2 character LCD.

---

## Files in this folder

| File | What it is |
|---|---|
| `MiniDino.ino` | The Arduino sketch. This is the only code file you need. |
| `assembly-guide.pdf` | Step-by-step assembly, wiring and gameplay guide (5 pages). |
| `project-documentation.pdf` | Full 21-page report: hardware, wiring, code walkthrough, game logic, viva Q&A, glossary. |
| `wiring-diagram.png` | The 4 LCD wires shown as a clean diagram. |
| `connection-map.png` | LCD + breadboard + Uno layout with every wire drawn. |
| `bill-of-materials.xlsx` | Parts list with Chandni Chowk prices (Rs. 775 total). |

---

## Hardware required

| Component | Qty |
|---|---|
| Arduino Uno R3 (with USB cable) | 1 |
| 16x2 LCD with I2C backpack (PCF8574) | 1 |
| KY-040 rotary encoder module | 1 |
| Piezo buzzer (passive preferred) | 1 |
| Solderless breadboard, 400 point | 1 |
| Jumper wires (male-to-male and male-to-female) | 1 set each |

No soldering is required anywhere.

---

## Wiring

| From | To | Wire colour |
|---|---|---|
| LCD VCC | Uno **5V** | red |
| LCD GND | Uno **GND** | black |
| LCD SDA | Uno **A4** | blue |
| LCD SCL | Uno **A5** | green |
| Encoder **SW** | Uno **D3** | orange |
| Encoder **+** | Uno **5V** | red |
| Encoder **GND** | Uno **GND** | black |
| Buzzer **+** | Uno **D10** | purple |
| Buzzer **−** | Uno **GND** | black |

> **Pin-order warning:** the four pins on an I2C backpack are not in a fixed order
> across manufacturers. Always follow the word printed next to the pin on *your*
> board, never the position. The board used here is printed `SCL SDA VCC GND` on
> one face, which is the reverse of the more common `GND VCC SDA SCL`.

---

## How to run it

1. Install the **Arduino IDE**.
2. Install the library **LiquidCrystal I2C** (by Frank de Brabander) from
   `Tools > Manage Libraries`.
3. Put `MiniDino.ino` inside a folder named **`MiniDino`** — the folder name must
   match the sketch name, or the IDE will refuse to open it.
4. Open `MiniDino.ino`, then select `Tools > Board > Arduino Uno` and the correct
   `Tools > Port`.
5. Click **Verify**, then **Upload**.

If the screen glows but no text appears, turn the small blue potentiometer on the
back of the LCD until the text becomes readable. If it stays blank, change the
I2C address in the sketch from `0x27` to `0x3F` and upload again.

---

## How the game works

- The bottom row of the LCD is the play field; the top row carries the score.
- The dino sits at column 0. A cactus scrolls in from the right every 230 ms.
- Press the encoder to jump (450 ms in the air).
- Clear a cactus and the score goes up by one. Hit one and the game ends.
- Press again after game over to return to the start screen.

Two constants at the top of the sketch control the feel:

| Setting | Value | Effect |
|---|---|---|
| `MOVE_MS` | 230 | Lower is faster and harder |
| `JUMP_MS` | 450 | Longer keeps the dino in the air longer |

---

## Credits

Original project: [krishsharma-code/Dino-game](https://github.com/krishsharma-code/Dino-game) (OLED version, MIT licence).
This folder is the 16x2 LCD adaptation of the same game.
