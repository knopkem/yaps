A flexible photo sorter tool that uses the exif information of your photos and videos to copy or move them into a structured folder hierarchy.

PURPOSE
If you are like me and have a heap of photos and videos in randomly named folders laying on your hard disk you could run the tool over it and nicely sort them into a new folder structure. Afterwards this would look like this:

year/month/timestamp.jpg 

Of course the structure can be customized to your needs. Optionally all duplicate files can be removed which helps reducing waste of disk space.

FEATURES
* highly customizable output folder structure
* the following criteria can be used for sorting: create date and time, camera model and make and media type (mime type)
* detect and ignore duplicates based on md5sums
* detect and fix file name clashes
* cross-platform (linux, windows, mac) 

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
* the tool uses the following projects behind: exiv2 and exiftool. Credits go there.

USAGE
* you need exiv2 AND exiftool available (either from path or copied into the bin folder)
* on debian linux, just install both packages
* on windows, build or download prebuild executables and put them into the same folder as the application

ROADMAP
* watch folder to easily drop photos from anywhere and auto import them into an existing archive
* deamon/service to run in background at system start (watching the watch folder)
* an additional Qt5 ported branch
