# Skye.vim

Connects to GitHub (pull requests for other commonly used VCS sites are welcome as long as there's an API) to display issues and comments in Vim.

See the documentation file for an extended intro, as well as the deets.

Issue creation and management may be implemented in the future, but the main goal at the moment is just to list and display.

## Requirements:
* C++20 compiler
* CMake
* Python 3 with Conan (https://conan.io)
* Modern Vim (8.2.2845+ guaranteed; this plugin uses Vim9, meaning there's limited version support as long as Vim9 is under active development). Neovim is not supported
* Git in `PATH` or otherwise accessible from [`system()`](https://en.cppreference.com/w/cpp/utility/program/system)
* A decent internet connection - support for awful internet connections relies on multithreading, which Vim doesn't support, and that I can't implement when dynamic libraries are unloaded after use (read: I will not fix issues triggered by bad internet connections. Bad here is generally defined as anything that takes ages to fetch data from the API)

**Note:** As I don't use Windows or Mac (and I can't be arsed to set up tests - testing networking is a pain), I have no idea whether or not this works on Mac or Windows. Support is guaranteed for desktop Linux, however, as long as it's a distro with support for modern compilers, Python 3, and pip.

The current requirement of requiring Git in PATH may or may not make it harder to use on Windows, which is yet another reason it may not work under Windows. If it fails and you'd like to see it work, submit a PR.

## TODO before v1.0.0:
* [x] Issue lists
* [x] Show individual issues
* [x] Reformat the list to be compact (don't include a summary at all, for clarity)
* [x] More syntax highlighting on the list page
* [ ] Filtering
* [x] Refreshing (keybind, may potentially just re-call SkyeList or SkyeShow)
* [ ] Dealing with paging served by the API
* [ ] Display pull requests (... and comments, but exclude changes)

## Future ideas:
* Issue editing and posting (full integration instead of read-only - we have the tokens for it already)

