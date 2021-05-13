vim9script

# Skye.vim 
# Maintainer: Olivia (github/LunarWatcher)
# License: MIT

g:SkyeVersion = 1


# Commands {{{
command -nargs=0 SkyeList call skye#ListIssues()
# }}}
# Keybinds {{{
nnoremap <leader>sl :SkyeList<cr>
# }}}
