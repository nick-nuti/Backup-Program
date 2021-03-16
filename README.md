# Backup-Program - need to write the README at some point

Current implementation flow chart: https://drive.google.com/file/d/1tuUqqRTy3E-e5zS4yjjNlmbozpua7xeO/view?usp=sharing

Writing the to-do list here for accessibility - need to add these as "post-it" notes in this github

*********Current Place We're At:
  •	Start copying files based on what the json file says
  •	Start integrating usability for windows + mac os
  •	Error reporting framework that will be expandable
  •	How do we plan to deal with files that the os blocks (like system32)? Skip them? Ask?
  •	What to do if the program is interrupted while in the process of backing up files? (what to do if the program is interrupted in a future backup?)

*********Bigger Ideas to work on/discuss:
  •	Find efficient ways to split up information for data being fed into the program
  •	minimize how much data is held in DRAM before writing it to the backup json file (While also minimizing the number of writes to the file)
  •	I tested the program with 100 GB worth of directories which were condensed into 4 top level directories... and the program ran for 5 minutes (unsure if it hung) before I manually exited
  •	Find a way to replace ofstream with "FILE *" (much faster and we don't require much parsing at this stage)
  •	Replace nlohmann json with rapid json
  •	Start backup process (of copying files and copying data) in multiple methods (so as to test usability and efficiency of multiple measures) [**Research question**]
      o	method 1: just copy the file over and run an acknowledgement?
      o	method 2: open a file, copy chunks of the file over and acknowledge what was just sent
  •	Different methods from a data flow point of view [**Research question**]
      o	method 1: create and write to files from roots to branch ends?
      o	method 2: create and write to files from branch ends back to roots? (this is potentially a bad idea due to multithreading collisions)
      o	method 3: other methods?
  •	Reoccurance through adjustable OS scheduling (can't do this until -> start integrating usability for windows + mac os) <- which is on the To Do List above
  •	Search for places to speed up certain processes (add const's where possible.... etc...)

*********Later materials:
  •	Simple UI + curses implementation
  •	Multithreading through the use of pthreads
  •	Cloud connection (in and out)
  •	Possibly create bootable ISO for OS backup (MAYBE to get working with windows and mac os "go back to previous backup" software
  •	Possibly mobile platforms
  •	Compressing backed-up data
  •	Using a UI to view backed-up data (that's possibly compressed) with viable information included
  •	Maybe think about a method to search for: files, file attributes, strings within files...
