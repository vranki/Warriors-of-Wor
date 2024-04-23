# BOMBERWARRIORS OF WOR
By Ville Ranki <ville.ranki@iki.fi>


BWOW is a remake and enhanced version of Commodore 64's 
Wizard of Wor game. It supports 3 game modes and up to 8 
players using Wiimotes.

It has been very successfull as a party game, although
still contains a few bugs. Consider it version 0.9 beta.

## Game modes

Original (co-op):
 - Fight different monsters in the dungeon.

Deathmatch:
 - Kill other players. After a few minutes walls start to disappear to 
   speed up the fight. 
 - Winner of each match gets one "Coin". The first player getting five 
   coins wins.

Bomberman:
 - Much like deathmatch, but with bombs instead of lazorguns.

## Adding players

Pairing wiimotes:

 - Start game
 - In player selection screen press "1" and "2" buttons on a wiimote simultaneously. 
 - Wiimote LEDs flash for a while and if the mote is connected leds light up 
   displaying player number (look at the screen). 
 - Repeat for all players. Press "+" to continue when all players have joined. 
 - Use wiimote in horizontal position. Only d-pad and button "1" are used in game.

You can press "K" on keyboard to add a single keyboard player, controlled by
arrow keys and space to fire. Yes, a second keyboard player is needed:

## Compilation

Install requires dependencies (Qt5, SDL_audio, libcwiid, ..)

```
qmake
make
```

Please inspect the file .github/workflows/c-cpp.yml for the commands
required to compile on a current release of Debian or Ubuntu.
No proper deb or rpm packaging implemented yet, sorry.

## Portability

The game is written on Ubuntu Linux but should be easily portable 
to other platforms which support Qt and SDL. Only the cwiid wiimote 
library is Linux-only, so it has to be replaced with something else.

Patches to build the game on other platforms are welcome.

## Known issues & todo

 - Performance is not as good as it shoulds, probably because
   sprites are not pre-scaled to game resolution. Needs some
   benchmarking.
 - Original game mode still has some bugs. Deathmatch should work
   pretty well.
 - More controller options needed (SDL joystick).
 - CRT rendering - see http://filthypants.blogspot.fi/2011/05/more-emulator-pixel-shaders-crt-updated.html

Patches are welcome. Use GitHub's merge requests if possible!

## See also

Several other implementations of WoW are presented on GitHub that you may want investigate:
 - https://github.com/dabadab/wizardofwor
   The reverse engineering one, also offering a series of technical insights.
   This project was developed completely independly, though.
 - https://github.com/Sanguinik/KnightOfWor/
   A Java implementation.

## Credits

Wizard of Wor arcade game by Midway Wizard of Wor C64 version by Commodore Bomberman by Hudson Soft

I consider WOW as abandonware. If you own the copyright 
for this game and want this free open source version 
to be removed, contact me but be prepared for negative 
PR in press.

Adore64 font from:
http://www.dafont.com/adore64.font

Free for personal use.

