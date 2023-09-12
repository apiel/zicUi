# TODO

- TODO instead of needToDraw, should push a list of thing to render, if it is the same should skip it
    NOT EVEN THIS:
    make  componenent a  flag "needToRender", and if needToRender is true, then we render

- TODO make layout conifurable:
    each page have a list of componenent...

- TODO slider: rotate encoder 90°

```cpp
float rotationAngle = 90.0f;
SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotationAngle , NULL, SDL_FLIP_NONE);
```

See also https://www.parallelrealities.co.uk/tutorials/ttf/ttf1.php
`blitRotated` is actually using `SDL_RenderCopyEx` https://www.parallelrealities.co.uk/tutorials/bad/bad2.php