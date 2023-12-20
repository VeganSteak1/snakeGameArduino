# Arduino Snake Game

This Arduino sketch implements a simple Snake game using various components, including an LCD display, a joystick, a buzzer, and LED matrix display. The game allows the player to navigate a snake on the LED matrix using the joystick, consume apples to grow in length, and avoid collisions with the snake's own body.

## Components Used

- Arduino Board: This code is designed to run on an Arduino board.

- LCD Display (16x2): Provides visual feedback, displaying the game's welcome message and current/high score during gameplay.

- Joystick: Used for user input to control the direction of the snake.

- Buzzer: Produces sound effects during gameplay.

- LED Matrix Display (8x8): Displays the game grid, snake, and apples.

## Wiring Diagram

- **LCD Display:** Connect the LCD to the Arduino using the specified pins (rs, en, d4, d5, d6, d7).

- **Joystick:** Connect the X-axis and Y-axis of the joystick to the specified analog pins (Analog_X_pin, Analog_Y_pin). Connect the button of the joystick to pin 2.

- **Buzzer:** Connect the buzzer to pin 3.

- **LED Matrix Display:** Connect the LED matrix using the specified pins (DATA_IN_PIN, CHIP_SELECT_PIN, CLOCK_PIN).

- **Other Connections:** Follow the pin connections for other components, such as the button pin (pin 13) and the analog pins for volume control.

## Setup

1. **Install Libraries:** Make sure to install the required libraries (LiquidCrystal and LedControl) for the LCD display and LED matrix.

2. **Upload Code:** Upload the provided Arduino sketch to your Arduino board.

3. **Hardware Setup:** Connect the components as per the wiring diagram.

## Game Controls

- **Joystick:** Control the snake's direction by moving the joystick.
- **Joystick Button:** Confirm menu selections and start the game.
- **Volume Control:** Use the button (pin 13) to toggle volume on/off.

## Game Menus

- **Settings:** Adjust volume (ON/OFF).
- **About:** Display information about the game.
- **Start Game:** Begin playing the Snake game.

## Game Play

- Navigate the snake using the joystick.
- Consume apples to increase your score.
- Avoid collisions with the snake's body.
- The game displays current and high scores on the LCD.

## Sound Effects

- The game includes sound effects through the buzzer. Adjust the volume settings in the "Settings" menu.

## Additional Notes

- The LCD displays a welcome message at the start.
- The game supports a high-score system.
- The LED matrix visually represents the snake, apples, and the game grid.

## Author

This code was created by [Author Name]. Feel free to modify and enhance it for your own projects.

## Acknowledgments

Special thanks to the Arduino community and the creators of the LiquidCrystal and LedControl libraries for making this project possible.
