# Reeper Release 2.0 #
# September 2014 #

Reeper 2.0 is a python plugin for use in Cinema 4D R14 R15 R16 only.

------------
Installation
------------
Unzip and drop the folder "Reeper 2.xxx" in your C4d plugins folder.
Remove any older versions of Reeper from your plugins folder.

--------
Updating
--------
If an updated Version of Reeper won't run as expected, you should delete the "symbolcache" file in your prefs folder first and restart C4D.
You can find your prefs folder from C4D Menue->Edit->Preferences see button "Open Preferences Folder ...".

-------
License
-------
Reeper 2.0 is free for commercial and non-commercial use, without any restrictions. Any redistribution is prohibted.
For use at your own risk, any guarantee or liability for any damages or losses, which result from a use, is impossible.

-----------------
Parameter & Usage
-----------------

Tab General
-----------------
- Coils:
Determines the amount of windings around the input spline.

- Strands
Determines the amount of strands around the input spline.

- Radius
Determines the radius (thickness) of the individual strands.

- Distance
Determines the distance of the strands relative to your input spline.


Tab Options
-----------------
- Rotation / Offset / From / To
Similar to Cinemas "Spline Wrap" deformer options.

- Multishade Mode
When turned on, each strand got an individual shade of grey as a basic color (editor view only).
What is it good for?
Well, if you own the Mograph Module (Studio Version), you can now use a Multishader Material to texturize your strands individual. See example file for setup.

Tab Strands
-----------------
Similar to Cinemas "Sweep Nurbs" options.

Tab Strandcaps
-----------------
Similar to Cinemas "Sweep Nurbs" caps options.
Note: Some parameter are only accessible when using R16.
.
Tab Interpolation
-----------------
- Strands-Path
Similar to Cinemas spline interpolation options.
Note: When using "Coiled Rope" mode, these options are limited.

- Radius-Path
Determines the amount of sides used for radius (thickness) of the individual strands.


have fun
vrom