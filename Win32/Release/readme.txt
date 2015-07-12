This FanController project  was created by my desire to to have a custom FanController-Hardware, which has 6 channels,
is usb-controlled, and gets its temperatues from the hardware-sensors instead of some crappy temperature fingers you attach to a heatsink
this is the controll application for my usb fan controller.
You can find the hardware-part here: https://github.com/Alia5/atmega328p_6_channel_usb_fancontroller

if you got this application from the original author, you most likeley know everything about this there is to know
else, if you have any questions you can contact me for any help.

You can find compiled binaries, or and .exe-installer package in "Win32/Release" directory

This software uses a variety of third party libraries:
Qt ( http://www.qt.io/ ) for the GUI, which is licensed under LGPLv3 / LGPLv2
You can read about the license in "qt-license.txt"

qcustomplot ( http://www.qcustomplot.com/ ), which is licensed under GPLv3
You can find the orginal licensefiles as well as the sourcefiles in "qcustomplot" in the source-directory, or in the "license.txt" of the installed software
hidapi ( https://github.com/signal11/hidapi ), which is licensed under either:
1. The GNU Public License, version 3.0, in LICENSE-gpl3.txt
2. A BSD-Style License, in LICENSE-bsd.txt.
3. The more liberal original HIDAPI license. LICENSE-orig.txt

This software also uses an optional proprietary first-party plugin to read sensor-values from HWiNFO ( http://www.hwinfo.com/ ), nameley "HWiNFO64.dll"
It could live as a standalone and gets dynamically invoked, which should be under terms of the gpl
If you've got this plugin from any other source than the ones provided by the original author, please contact him! ( https://github.com/Alia5 ) 

This software is licensed under the GPLv3, you can find the GPLv3 license in "license.txt"