# ReactionTime App

Arduino reaction-time game for a 16x2 LCD keypad shield. The player waits for the "GO NOW!" prompt and presses any shield button as quickly as possible. The sketch measures and displays the reaction time in milliseconds.

## Hardware

- Arduino Uno or compatible board
- 16x2 LCD keypad shield using the standard LiquidCrystal parallel pin layout
- Built-in Arduino LED on pin 13

## Pin Mapping

The sketch uses this LCD keypad shield mapping:

| Function | Arduino Pin |
| --- | --- |
| LCD RS | 8 |
| LCD Enable | 9 |
| LCD D4 | 4 |
| LCD D5 | 5 |
| LCD D6 | 6 |
| LCD D7 | 7 |
| Keypad analog input | A0 |
| Random seed input | A1 |
| Built-in LED | 13 |

## Button Values

The keypad shield buttons are read through `analogRead(A0)`.

| Button | ADC Range Used |
| --- | --- |
| RIGHT | `< 50` |
| UP | `< 200` |
| DOWN | `< 400` |
| LEFT | `< 600` |
| SELECT | `< 850` |
| NONE | `> 1000` |

These thresholds match common Arduino LCD keypad shields. If your shield reads different analog values, adjust the thresholds in `readLCDButtons()`.

## Gameplay

1. The LCD shows `Reaction Game` and waits for SELECT.
2. Press SELECT to start a round.
3. The game shows `Get ready...` and waits for a random delay between 2 and 5 seconds.
4. Pressing too early shows `Too early!`.
5. When the delay finishes, the built-in LED turns on and the LCD shows `GO NOW!`.
6. Press any shield button.
7. The LCD displays the reaction time in milliseconds.
8. Press SELECT to return to the start screen and play again.

## State Machine

The sketch uses a small state machine:

- `WAIT_START`: waits for SELECT to begin.
- `RANDOM_WAIT`: waits through the random delay and checks for early presses.
- `SHOW_GO`: records the player's reaction after the GO signal.
- `SHOW_RESULT`: displays the measured reaction time.
- `TOO_EARLY`: displays the early-press warning.

## Files

- `reactiontime-app.ino`: main Arduino sketch.
- `README.md`: project documentation.

## Uploading

1. Open the sketch in the Arduino IDE.
2. Select the correct board, such as `Arduino Uno`.
3. Select the correct serial port.
4. Install or enable the built-in `LiquidCrystal` library if needed.
5. Upload `reactiontime-app.ino` to the board.

## Notes

- The random delay is generated with `random(2000, 5001)`, so each round waits between 2000 ms and 5000 ms.
- `randomSeed(analogRead(A1))` uses an unconnected analog input to vary the random sequence.
- The game uses edge detection so holding a button down does not count as repeated presses.
- The built-in LED turns on at the same time as the GO prompt and turns off after the player presses a button.
