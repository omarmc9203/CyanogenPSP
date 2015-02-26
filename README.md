CyanogenPSP Beta
=================

This is a program that allows you to do everything a PSP can do with a little more handy features, but with a whole different UI. This isn't a port of cyanogenmod from android, it's a project created from scratch. It's purpose is similar to that of "iR Shell", but has a UI inspired by CyanogenMod, Android. It will never allow you to run android apps, the psp system doesn't have the hardware capabilities to run android. If developers are intrested, feel free to contact me through my email: sauskethunderkid@gmail.com 

This is not ready for release. Kindly wait for more updates.


What's working
=================
- Cursor (analog buttons)
- Home Menu
- Touch tones, when pressing the cross button on an app/widget.
- App drawer highlighting effect. 
- Menu buttons Highlighting effects.
- Battery functions. (Displays the battery percentage, by using icons.)
- App Drawer
- CWM Based PSP Recovery Menu 1.0 (Merged) - Press square to open power menu, then select recovery.
- Lockscreen
- Notification Slider
- Navbar functions, for returning home or back to the previous menu.
- Revamped Power menu. (Allows access to exit to XMB, and launch recovery)
- Added more lock screen functions.
- Added Multitasking (W.I.P)
- Added digital time.
- Added USB Toggle in Settings->about, and recovery menu.
- Revamped Settings menu, making it much similar to Cyanogenmod's.
- Added screenshots feature. Use R + X to take a screenshot.
- Integrated RemoteJoyLite. Pressing select in Settings->about, powers it on and off.
- Added the following options in recovery menu:
[] Usb toggle
[] Shutdown Device
[] Standby
- Apollo music player
- App drawer pane navigation
- Quick toggles
- File Manager
- Internet Browser - NetFront default PSP browser
- OTA Updates - Settings>About>CyanoPSP Updates (WiFi only obviously)
- Gallery App
- Gmail app - uses the browser and loads up the mobile version of gmail for now.

What's yet to work
=================
- Fully functional calculator
- Boot animation (I'll use a set of 20 images in a zip file probably - These images can be replaced by using another zip with the same file names and format)
- Game (Homebrew and Backups) Launcher
- More settings and tweaks.
- Theme Manager
- Contacts storing application. (People App)

What's not working
=================
- Everything else that isn't on the working list.

Compiling it yourself 
=================
This method only allows you to compile on Windows. I'm not sure how you'd go about compiling it on Linux sorry.

1. Ensure you have PSP SDK installed. (I recommend the latest minimalist SDK, http://sourceforge.net/projects/minpspw/)
2. Make sure you've set your envoirnemt variables to PSPSDK

   Go to Control Panel -> System -> Advanced system settings -> Advanced tab -> Environment Variables. A window with two     tables should pop up. In the first table select Path and then click Edit. In variable value add:
   
   ;C:\pspsdk\bin
   
3. Install OsLib Mod v2 from here https://github.com/dogo/oslibmodv2 - Instructions included.
4. Dowwnload and install the headers and libs from the M33 SDK - Required for compiling the recovery menu.
5. Run compile.bat
6. Hit 1 on your keyboard, and press enter to start compiling.


License can be found here https://github.com/joel16/CyanogenMod-PSP-C/blob/master/License

