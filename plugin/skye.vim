vim9script

# Skye.vim 
# Maintainer: Olivia (github/LunarWatcher)
# License: MIT

g:SkyeVersion = 1


command -nargs=1 SkyeSetUrl call skye#SetUrl(<f-args>)
