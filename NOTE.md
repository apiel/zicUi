# TODO

- FIXME float value incrementation....

- FIXME encoder string doesnt work properly, see view...

- TODO button PRESSED_INT_VALUE

- TODO slider

- TODO encoder responsive for small size
- TODO use function pointer on encoder initialisation to assign draw function base on type

- TODO list of value, e.g.:
                file1.wav
                file2.wav
              > file3.wav
                file4.wav
                file5.wav

- TODO optimized onUpdate value...?

- TODO keyboard?

- TODO componentEncoder option rotate 90°

```cpp
float rotationAngle = 90.0f;
SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotationAngle , NULL, SDL_FLIP_NONE);
```

See also https://www.parallelrealities.co.uk/tutorials/ttf/ttf1.php
`blitRotated` is actually using `SDL_RenderCopyEx` https://www.parallelrealities.co.uk/tutorials/bad/bad2.php

- TODO should we show the grains???

- TODO make style configurable

# PCB

- https://www.kicad.org/