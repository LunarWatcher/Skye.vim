vim9script

# Skye.vim 
# Maintainer: Olivia (github/LunarWatcher)
# License: MIT

g:SkyeVersion = 1


# Commands {{{
command -nargs=? SkyeList call skye#ListIssues(<f-args>, "?state=open")
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
