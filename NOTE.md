# TODO

- TODO make componentEncoder keep 1 to 4 sticky and ignore rootEncoderIndex...

- TODO components should be library to load

- TODO page of components list

- TODO centralized event handler
  value should callback to mainView and has params the index of the plugin +  a key if there is mutiple value in the component. Then mainView is in charge to spread the value change to all the component... Maybe this could  even be hadded directly on value host system -> hostValue function...

- TODO componentValue option rotate 90°

```cpp
float rotationAngle = 90.0f;
SDL_RenderCopyEx(renderer, texture, NULL, &destRect, rotationAngle , NULL, SDL_FLIP_NONE);
```

See also https://www.parallelrealities.co.uk/tutorials/ttf/ttf1.php
`blitRotated` is actually using `SDL_RenderCopyEx` https://www.parallelrealities.co.uk/tutorials/bad/bad2.php

- TODO should we show the grains???