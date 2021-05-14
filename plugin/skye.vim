vim9script

# Skye.vim 
# Maintainer: Olivia (github/LunarWatcher)
# License: MIT

g:SkyeVersion = 1


# Commands {{{
command -nargs=? SkyeList call skye#ListIssues(<f-args>)


# }}}
# Keybinds {{{
nnoremap <leader>sl :SkyeList<cr>
# }}}
