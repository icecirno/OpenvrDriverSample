# OpenvrDriverSample
## Installation
###1.Install Steam and SteamVR.
###2.Copy whole folder "%OpenvrDriverSample%\bin\drivers\sample" to "%Steam%\steamapps\common\SteamVR\drivers".
###3.Copy file "%OpenvrDriverSample%\bin\drivers\Register.bat" to "%Steam%\steamapps\common\SteamVR\drivers".
###4.Unregister other VR driver by run CMD command "vrpathreg removedriver "%Driver_Path%"" in path "%Steam%\steamapps\common\SteamVR\bin\win32".

## Usage
###1.Run Steam and SteamVR.(In normal case ,it will automatically open a full screen window)
###2.Press SPACEBAR to enable mouse input.(It using keyboard hook,and reject keyboard message which send to other application)

## UnInstallation
###1.Run CMD command "vrpathreg removedriver "%Steam%\steamapps\common\SteamVR\drivers\sample"" in path "%Steam%\steamapps\common\SteamVR\bin\win32".
###2.delete folder "%Steam%\steamapps\common\SteamVR\drivers\sample" and "%Steam%\steamapps\common\SteamVR\drivers\Register.bat"