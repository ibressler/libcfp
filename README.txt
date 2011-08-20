libcfp: a library for Chemical Formula Parsing

It analyses the ASCII representation of a chemical formula. For every element 
it extracts the following characteristics:

- symbol name (first letter in upper case, all following letters in lower case)
- nucleon number (is isotope or not, optional, integer number)
- coefficient (optional, floating point number unequal 1.0)

It is written in C++, analyses syntax only and does not know anyhing about
semantics (physical characteristics). It is intended to be independent of
additional external libraries at build or runtime (namespace ::std:: only).

http://developer.berlios.de/projects/libcfp/

Documentation
-------------

- If you want to use this library in your own application, the entry point is in 

  <libcfp-directory>/doc_user/html/index.html

- If you want to contribute to the library itself or want to know how it works
  internally, the entry point is in

  <libcfp-directory>/doc_full/html/index.html

How to build
------------

CMake is used for building the library on various platforms. Just run 'cmake'
to get a list of available generators (which generate build environment
specific project files). On a common Linux system it should be like that:

cd <libcfp-directory>
mkdir build_files
cd build_files
cmake ..
make

Afterwards, the libraries (dynamic and static) can be found in 
<libcfp-directory>/lib

For advanced build settings (debug symbols, optimization, warnings, etc ...), 
adjust <libcfp-directory>/CMakeLists.txt to your needs.

In a MSYS shell on a Windows combined with MinGW, you may have to replace the
command <cmake ..> by <cmake .. -G "MSYS Makefiles"> (choosing the cmake
generator for Makefiles used in a MSYS shell).


Copyright
---------

This library is released under the GNU Lesser General Public License (LGPL).
For further information see LICENSE.txt

It was written by Ingo Bressler (libcfp at ingobressler.net)
at the Stranski-Laboratory for Physical and Theoretical Chemistry of the 
Technische Universität Berlin.
Postal address:

TU-Berlin
Stranski-Laboratorium
Institut für Chemie
Fakultät II
Sekr. TC 7
Straße des 17. Juni 124
10623 Berlin

