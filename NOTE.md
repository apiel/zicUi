# TODO

- TODO hidden view that can host shared component or second state view
      - shared component could be imported using INCLUDE file to have a kind of layout file, meaning that each view could reuse a preset of component. To do we would need a place to define first this shared component. If this layout is reused in all the views, we need a hidden view to host the shared component.
      - A view can have multiple state (think about a shift mode view). While scrolling in `VIEW UI` state, to change from one view to the other, we dont want to scroll through those shifted views... Therefor, we need some hidden views, that's are not scrollable?

- TODO button PRESSED_INT_VALUE
       button can trigger NOTE or should finish note component
       button can easily set view base on name
       can button show a state
       can button show a value

- TODO documentation!!

- TODO Significantly improve sequencer UI

- TODO oscEncoder use time to make bigger step...

- TODO slider, e.g. for mixer

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

- NOTE should we use https://github.com/nlohmann/json for UI ?