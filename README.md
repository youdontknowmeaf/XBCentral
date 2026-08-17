# XbCentral
**The better homebrew tool**

XbCentral is a multi-purpose app for all-things Xbox 360 (for now) homebrew.

# Functions
- Convert Xbox 360 .iso games to GOD format
- Create USBs with selected Xbox 360 Payload (BadUpdate or ABadAvatar, any version)

# How 2 run
Go to the releases page and download the newest release.tar.gz.
Extract it anywhere and run the RunMe.sh that's inside.

# How 2 compile
Prerequisites: Install libraylib.a on your system along with C++17 and GNU/Make

Clone the repository and use one of the following make commands:
- ``make`` builds a dynamic executable
- ``make static`` makes a (semi) staticly linked executable
- ``make release`` turns the made executable into a release.tar.gz

# Manual
1. Make sure you run the script via RunMe.sh or else the relative paths WILL break
2. If you want to make a .desktop file then here's a simple template:
```
[Desktop Entry]

```
[ TBD ]
