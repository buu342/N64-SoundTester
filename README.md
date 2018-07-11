# N64 Sound Tester
A sample ROM that allows you to switch out sample banks and tune them directly on your N64/Emulator. This is meant as a workaround for having to recompile and blindly test out different "Coarse" value with the N64 Sound Tools, as playback is impossible on them without the proper hardware.
</br></br>

## How To Setup For the First Time
1) Place the files in a directory on your development machine, prefferably in a location without spaces or special characters in the name.
2) (optional) Open the source folder and create a shortcut for makeme.bat, replacing the original shortcut outside the source folder.
3) Execute makeme.bat to ensure that everything is properly configured. It will spit out errors until everything has been corrected. 
4) Once a proper compile has been made, the ROM can be found in the source/out folder, or the folder configured in makeme.bat
5) Test the ROM to ensure that everything is working. If an error occured during compilation, ensure you have the Sound Tools installed.
</br></br>

## How to replace the sound bank
1) Replace n64st1.ptr, n64st1.wbk, sfx.bfx with your exported files. If you name the files differently, change the spec file.
2) Execute the makeme shortcut to compile the ROM
3) Test on an emulator/hardware.
</br></br>

## FAQ
**Q:** Some of my sounds don't play after a certain amount of coarse
**A:** The only known solution is to lower the frequency of your sound and re-export the bank.
</br></br>
**Q:** My sound cuts off really early!
**A:** Make sure you change the length of the sound in the SFX Editor. 48 is equivalent to half a second
</br></br>

Any other information should be available in this document: 
https://docs.google.com/document/d/1d1qKxMh3q_89w9N76xL9bXRqkXe1ylcDnAtg3cgu5s8/edit#
