# Blasteroids

## Description

This blasteroids game is written in C using the [Allegro](https://github.com/liballeg/allegro5) library.

You can control the spaceship with the arrow keys and shoot with SPACE. When the asteroids are hit they split into to. For every asteroid you hit you get 100 added to your score. The game can be exited by closing the game window which will show a stop screen. Then you can just press Q to exit.

For handling user input and video display this game is using basic multithreading: one thread for the user input and movement calculations and the other for displaying everything on screen.

## Compilation

Compile with (after [Allegro](https://github.com/liballeg/allegro5) has been installed):

```
$ make blasteroids
```

or just

```
$ make
```

Now the game can be started with (assuming you are in the blasteroids directory)

```
$ ./blasteroids
```
