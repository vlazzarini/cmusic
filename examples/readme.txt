The cmusic command produces 32-bit float samples:

cmusic score.sc > test.raw

to create a self-describing soundfile, you should pipe its output 
to 'tosf' as in this example: 

cmusic score.sc | tosf -if -of -R9600 -c2 test.irc

This creates a soundfile in the ~/audio directory (in the top-level
user directory). You should create this directory first.

VL, 2009
