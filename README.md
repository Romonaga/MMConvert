# MMConvert
Multi-Media Converter and tool

A project to learn QT.

Have alot of movies, managment and transcoding them from teh CLI was a bit of a PITA.  As such, my daughter and I decided to learn QT and port our CLI tools to a graphical interface. This program, mostly is a wrapper for ffmpeg, while we did, and we do still use the libav API's.  I found the confusion, and the fact that ffmpeg does a far better job then I ever will, a leading factor to move away from raw api calls and just continue wrapping ffmpeg.

This program assumes that you are using nvida hardware and have the proper drivers.  For anyone interested in this code, it will not be hard to find where I build the ffmpeg command string, and modify it to your own use.  God help you.

The end result, while faulty, and I am sure QT developers would cry at some of th liberties, it was a learning experiance and the end result was decent and meets our needs.

Some of the features.
Hardware transcoding of movies with ffmpeg.
libav api calls to get meta data.
Montors directory for movies, and will process them based on preset rules.
Several tools for helping us manage our collection.
  Megaboom purchase to collection check, finds items you bought but did not d-load.
  Dupe file check, will check for dupe movies, and allow you to clean it up.
  Plex-Media DB to files check, checks to insure Plex has all the files loaded.
    This one was fun to reverse enginer the database, Amanda learned much that day. 
  
  
  


