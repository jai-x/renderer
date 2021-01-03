# renderer

An experiment in software rendering to teach me more about it. Based on
ssloy/tinyrenderer but adapted and written in C99.

![screenshot of renderer rendering a teapot](./renderer_screenshot.png)

## todo

- [ ] Remove SDL2 and go back to using raw PPM images
- [ ] Fix inverted Y axis from OBJ file parsing
- [ ] Implement full OBJ file parsing rather than just a subset
- [ ] Texturing
- [ ] Perspective projection

## requirements

Tested to work on MacOS and Linux. Requires SDL2 to be installed

## build and run

```shell
$ mkdir build
$ make
$ ./build/renderer
```
