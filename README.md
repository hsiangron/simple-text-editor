# simple-text-editor
simple-text-editor is a lightweight command-line text editor implemented in C/C++. It is inspired by the classic Vim editor and provides essential editing functionalities in a simplified form. This project is ideal for learning about text editor internals, command parsing, and buffer management.

## Features

- **Modes**:
  - **Normal Mode** (default): Interpret input as commands.
  - **Insert Mode**: Input characters are inserted at the cursor location.

- **Normal Mode Commands**:
  - `u`: Undo the last operation (insertions or deletions). Multiple-step undo is supported.
  - `x`: Delete the character under the cursor.
  - `/pattern`: Search forward for a string `pattern` from the current cursor position.
  - `h`, `j`, `k`, `l`: Move the cursor left, down, up, and right, respectively.
  - Commands starting with a symbol (like `:`) are displayed in the status line and executed after pressing Enter.

- **Insert Mode**:
  - Insert characters at the current cursor location.
  - All input characters are displayed as they are typed.

- **Bonus Features**:
  - Support for scrolling when text exceeds the screen size.
  - Multi-step undo/redo: undo operations can be redone.
