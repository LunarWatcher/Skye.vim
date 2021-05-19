vim9script

# Skye.vim 
# Maintainer: Olivia (github/LunarWatcher)
# License: MIT

g:SkyeVersion = 1


# Commands {{{
command -nargs=? SkyeList call skye#ListIssues(<f-args>)
command -nargs=? SkyeListClosed call skye#ListIssues(<f-args>, '?state=closed')
command -nargs=? SkyeListAll call skye#ListIssues(<f-args>, "?state=all")
command -nargs=? SkyeShow call skye#ShowIssue(<f-args>)

# }}}
# Keybinds {{{
# Plugs {{{
nnoremap <Plug>(SkyeList) :SkyeList<CR>
nnoremap <Plug>(ShowIssue) :SkyeShow<CR>
# }}}

nnoremap <leader>sl :SkyeList<cr>

# }}}
# Autocmds {{{
augroup SkyeAutocmds
    au!
    autocmd FileType SkyeGithub call skye#InitListBuffer()
augroup END
# }}}
