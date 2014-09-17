xcom-hacks
==========

This is an ugly, nasty hack which should prevent XCOM:Enemy within  (Steam/Linux) from crashing more often than UFOs.
Although it looks, smells and tastes pretty hacky, it worked surprisingly well for me (not a single segfault using this hack). I finished nearly the whole "enemy within" using it and tested three consecutive "enemy unknown" missions (without the extension).
Some background information about the crash: http://psydk.org/archives/2014-07-05-221-introducing-plasma-fence


#### Step 1 (both versions) : compile
> `gcc -ldl -shared -fPIC -o libxcomfix.so -std=gnu11 xcomfix.c`

#### Step 2: patch the enemy unknown start script
apply this patch (replace /path/to/ ):
>```
--- SteamLibrary/SteamApps/common/XCom-Enemy-Unknown/binaries/linux/xcom.sh	2014-09-17 23:43:58.312490143 +0200
+++ SteamLibrary/SteamApps/common/XCom-Enemy-Unknown/binaries/linux/xcom.sh	2014-09-17 23:43:26.263492934 +0200
@@ -30,6 +30,8 @@
 	# Launch the game and grab the status
 	STATUS=42
 	while [ $STATUS -eq 42 ]; do
+		echo launching with libxcomfix.so
+		export LD_PRELOAD=/path/to/libxcomfix.so
 		${DEBUGGER} "${GAMEBINARY}" $@
 		STATUS=$?
 	done
```

#### Step 3a (Enemy Unkown): launch the game 
launch it normally

#### Step 3b (Enemy Within): launch the game
launch it like this (after steam is started):
>```
export LD_LIBRARY_PATH="/path/to/SteamLibrary/SteamApps/common/XCom-Enemy-Unknown/xew/binaries/linux/x86_64/"
cd /path/to/SteamLibrary/SteamApps/common/XCom-Enemy-Unknown/xew
LD_PRELOAD=/path/to/libxcomfix.so binaries/linux/game.x86_64
```
 
 
