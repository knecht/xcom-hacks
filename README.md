xcom-hacks
==========

This is an ugly, nasty hack which should prevent XCOM:Enemy within  (Steam/Linux) from crashing more often than UFOs.
Although it looks, smells and tastes pretty hacky, it worked surprisingly well for me (not a single segfault using this hack).
Only tested with "enemy within".
See also: http://psydk.org/archives/2014-07-05-221-introducing-plasma-fence

#### compile:
> `gcc -ldl -shared -fPIC -o libxcomfix.so -std=gnu11 xcomfix.c`

#### launch the game (after steam is started):
>```
export LD_LIBRARY_PATH="/path/to/SteamLibrary/SteamApps/common/XCom-Enemy-Unknown/xew/binaries/linux/x86_64/"
cd /path/to/SteamLibrary/SteamApps/common/XCom-Enemy-Unknown/xew
LD_PRELOAD=/path/to/libxcomfix.so binaries/linux/game.x86_64
```
 
 
