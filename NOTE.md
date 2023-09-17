# TODO

- TODO show active encoder/value

- TODO midi for encoder
    - might want to make midi, osc, and all kind of external interface a plugin
    - should midi on ui also forward midi to host?

- TODO components should be library to load

- TODO page of components list

- TODO componentValue option rotate 90°

```cpp
float rotationAngle = 90.0f;
SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotationAngle , NULL, SDL_FLIP_NONE);
```

See also https://www.parallelrealities.co.uk/tutorials/ttf/ttf1.php
`blitRotated` is actually using `SDL_RenderCopyEx` https://www.parallelrealities.co.uk/tutorials/bad/bad2.php

- TODO should we show the grains???