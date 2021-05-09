# Skye.vim

Connects to GitHub (pull requests for other commonly used VCS sites are welcome as long as there's an API) to display issues and comments in Vim.

## Requirements:
* C++20 compiler
* CMake
* Python 3 with Conan (https://conan.io)
* Modern Vim (8.2.2845+ guaranteed; this plugin uses Vim9, meaning there's limited version support as long as Vim9 is under active development)

**Note:** As I don't use Windows or Mac (and I can't be arsed to set up tests - testing networking is a pain), I have no idea whether or not this works on Mac or Windows. Support is guaranteed for Linux, however, as long as it's a distro with support for modern compilers, Python 3, and pip.

