CMUSIC
===========================

These are the sources and makefiles for a build of the classic CARL cmusic and 
related programs. The build has been tested on OSX and Linux, and although a 
number of  warnings are issued, it compiles and links successfully (but see
'Known issues' below):

Some important notes:

1. The build locations have been modified so that the required libraries
and binaries are created locally under ./lib and ./bin, respectively.

2. The FORTRAN code is set to be built and linked using gfortran. The compiler and
libraries need to be present. This can be downloaded and installed from
 http://gcc.gnu.org/wiki/GFortran 

3. The source tree is based on the Carl-0.02 linux distribution, but it has
been pruned somewhat to remove code that is either outdated, irrelavant or
won't build for some reason.

4. The original m4-based makefile framework has been removed and
substituted for a simpler fixed set of makefiles.

5. Building is just a matter of typing

   make

   at the toplevel

6. To install headers, libraries and binaries in /usr/local/*  type
  
    sudo make install

    to install elsewhere, just edit the 'PREFIX' location in the toplevel makefile.

7. If you have portaudio installed you can build the 'todac' utility,
which will let you play your cmusic scores directly to an audio card.

    cd todac
    make 
    
   To install this extra utility, just repeat item 6 above.

Known issues:

1. On Linux, tosf is not working when taking floatsams from a pipe.
2. On Linux, 'typein' does not build because of 'struct sgttyb' not being
defined anywhere.

TODO:

1. Fix and add to the build other 'bicsf' utilities from the Carl sources. This
involves mainly adding a line to include the stdlib.h and string.h headers 
where needed and removing any 'old-style' declarations of std C lib functions 
still present in the code. Easy but tedious.

2. Fix the various warnings, which fall mainly in the category of integers
being used as pointers. Again, not difficult, but very tedious.

3. Rewrite the code from old-style K&R C to ANSI C. This is the final updating
that will be needed to future-proof the code. 

4. Add two a 'tosndfile' to send floats to a libsndfile-supported output
soundfile (replacing the problematic bicsf code).
  
A few working score examples can be found in the examples directory.


