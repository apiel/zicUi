## Installation

`sudo apt-get install fonts-roboto libsdl2-dev libsdl2-ttf-2.0-0 libsdl2-ttf-dev`

## UI

UI is fully customizable using the `./ui/main.ui` file.

> To get styling for .ui extension in VScode, use `Ctrl` + `Shift` + `P` and type `Change Language Mode`. Then select `Configure File Association for '.ui'...` and select `Properties`.

Those `.ui` files are simple configuration files using `KEY=VALUE`. Each lines is parsed one after the other, to load the UI in memory, meaning that the order of each configuration steps matter. Comment can be applied by placing a `#` at the beginning of the line (a comment, cannot be done at the end of a line).

### Key: INCLUDE

A `.ui` file can include another `.ui` file, using `INCLUDE=path/of/the/file.ui`. As soon as this `INCLUDE` is called, it will continue into the child file till his end, to finally come back to the original file.

```ini
INCLUDE=path/of/the/file.ui
```


