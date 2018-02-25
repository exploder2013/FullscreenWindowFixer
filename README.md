# FullscreenWindowFixer
Fixes the problem of whenever a full-screen application (e.g. games) is not running in native resolution and in result, when minimizing it resizes all background windows to wrong sizes.

# Usage:
  * It takes one paramater when lanuching which is the specific game executable name.
    * For example, if a games executable is called ABC.exe, you would launch it by specified format: **fullscreenwindowfixer.exe ABC.exe**
    
  * By default it already uses small default games list if the argument is not provided.
    * Default games: **RainbowSix.exe, witcher3.exe, bf4.exe, csgo.exe**
    * Additional games can be added to default list in [source code](https://github.com/exploder2013/FullscreenWindowFixer/blob/4e84351cac2ca6ad0276b18e57ea5a5b235039ac/Source/Main.cpp#L60). 
