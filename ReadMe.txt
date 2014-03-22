A flexible photo sorting tool that uses the EXIF information of your photos and videos to copy or move them into a structured folder hierarchy.

PURPOSE
If you are like me and have a heap of photos and videos in randomly named folders laying on your hard disk this tool might be something for you  (provided you don't like it that way). It takes your photos and nicely sorts them into a new folder structure auto generated on the criteria you chose. For example like this:

year/month/timestamp.jpg 

Of course the structure can be customized to your needs. Optionally duplicate files can be removed.

FEATURES
* parses images for: create date and time, camera model and make, media type (mime type)
* highly customizable folder structure
* fixes duplicates based on md5sums
* fixes file name clashes (auto rename)
* cross-platform ready (linux, windows, mac) 

BINARIES
* grab them here: http://sourceforge.net/projects/yapsyetanotherphotosorter

BUILD INSTRUCTIONS

Requirements:
* CMAKE
* Essential build tools
* Qt4

Steps:
* compile or install Qt library + add bin directory to path
* run cmake on the project, this should detect Qt
* build

NOTES
The tool uses the following projects behind: exiv2 (http://www.exiv2.org) and exiftool (http://www.sno.phy.queensu.ca/~phil/exiftool). 
Credits go there.

USAGE
* you need exiv2 AND exiftool available (either from path or copied into the bin folder)
* on debian linux, just install both packages
* on windows, build or download prebuild executables and put them into the same folder as the application

ROADMAP
* watch folder to easily drop photos from anywhere and auto import them into an existing archive
* deamon/service to run in background at system start (watching the watch folder)
* an additional Qt5 ported branch
