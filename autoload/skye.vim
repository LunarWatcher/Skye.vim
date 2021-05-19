vim9script

# TODO: figure out which extension is used on Mac (read: which CMake generates
# on Mac)
# (Windows disallows extensions)
var ext = ""
if has("linux")
    ext = ".so"
endif

# Assumes Windoze generates binaries in the same format anyway
var binary = expand("<sfile>:p:h") .. "/../build/lib/libskye" .. ext
var forceLoad = libcall(binary, "verifyLoaded", 0)
if (forceLoad != "Dynamic library successfully loaded")
    echoerr "Failed to load dynamic library"
    finish
endif

# Early-init API {{{
def SeparateStrings(...strings: list<string>): string
    var result = ""
    for str in strings
        if result != ""
            result ..= ";"
        endif
        result ..= str
    endfor
    return result
enddef

def skye#DetectUrl(remote = "origin")
    var url = libcall(binary, 'detectUrlFromRemote', remote)
    if (url != "")
        g:SkyeIssueUrl = url
    elseif !exists('g:SkyeIssueUrl')
        g:SkyeIssueUrl = ""
    endif

enddef

# }}}
# Default variable initialization {{{
if (!exists("g:SkyeGitHubAccessToken"))
    g:SkyeGitHubAccessToken = ""
endif

if (!exists("g:SkyeUpstreamRemote"))
    g:SkyeUpstreamRemote = "origin"
endif

if (!exists("g:SkyeIssueUrl"))
    call skye#DetectUrl(g:SkyeUpstreamRemote)
endif
g:SkyeActiveUrl = g:SkyeIssueUrl

# State management variables
# Could make SkyeLastIssue a list for a full navigation-style experience, but
# meh
g:SkyeActiveIssue = "-1"
g:SkyeLastIssue = "-1"

# Buffer for the list
var listBuffID = -1
# Buffer for the issue
var issueBuffID = -1
# }}}
# API {{{

def skye#ManageScratchBuffer(switch: number)

    var id = switch == 0 ? listBuffID : issueBuffID

    if id == -1 || bufwinid(id) < 0
        if (id >= 0)
            # Not sure if this is the best way to go about this, but I can't
            # find a way to easily restore the buffer
            silent! exec "bd " .. id
        endif
        # Make the scratch buffer

        exec "" .. (switch == 0 ? "split" : "vsplit")

        noswapfile hide enew
        setlocal buftype=nofile
        setlocal bufhidden=hide

        if switch == 0
            setlocal ft=SkyeGithub
        else
            setlocal ft=markdown
            skye#InitIssueBuffer()
        endif

        setlocal nomodifiable
        
        # Set the cached buffer ID for internal management
        if switch == 0
            listBuffID = bufnr()
        else
            issueBuffID = bufnr()
        endif

        nmap <buffer> <C-g> <Plug>(ShowIssue)
        nmap <buffer> <CR> <Plug>(ShowIssue)
    else
        # If the buffer exists, and is in the current window (and in the
        # current tab, because Vim), we go to the buffer
        # Not sure why this clusterfuck is required, but win_gotoid(listBuffId)
        # doesn't work. Guess Vim is special :dontfeelsoblob:
        win_gotoid(get(win_findbuf(id), 0))
    endif

enddef

# apiQuery defaults to empty over ?state=open because state defaults to open
# in the API itself
def skye#ListIssues(url = g:SkyeIssueUrl, apiQuery: string = "")
    if type(url) != v:t_string
        echoerr "URL isn't a string; aborting function"
        return
    endif

    # Cache variables (and utilities for refreshing)
    g:SkyeActiveUrl = url
    g:SkyeActiveFilter = apiQuery

    # This isn't portable if multiple services are added, but that's a problem
    # for later:tm: Probably best to use a function
    var issues = libcall(binary, 'getIssues', SeparateStrings(url, g:SkyeGitHubAccessToken, apiQuery))

    # Load the scratch buffer
    skye#ManageScratchBuffer(0)

    set modifiable
    # Clear the buffer
    normal! ggdG
    silent! setline(1, split(issues, "\n"))
    set nomodifiable
    
enddef

def skye#ShowIssue(issue: string = "-1")
    var mIssue = issue
    # This shouldn't be a false positive, ever. I don't think any issue
    # systems include IDs like -1
    if issue == "-1"
        # Check if there's a token under the cursor first (... in the current
        # space-separated word)
        var currToken = expand('<cWORD>')
        if currToken =~ '\v^#[a-zA-Z\-0-9]+$'
            mIssue = currToken[1 :]
        else
            var currLine = getline('.')
            # This doesn't need to be a strict search. The buffer only contains
            # auto-generated content, meaning the line either starts with #1234
            # (or #AB-1234 or whatever), or it's blank or contains the API usage
            # stat thingy
            if currLine =~ '\v^#'
                mIssue = currLine[1 : stridx(currLine, "\t") - 1]
            else
                echom "Failed to find an issue reference in the current line or token"
                return
            endif
        endif
    endif
    g:SkyeLastIssue = g:SkyeActiveIssue
    g:SkyeActiveIssue = mIssue

    var issuesAndComments = libcall(binary, 'getIssue', SeparateStrings(g:SkyeActiveUrl, g:SkyeGitHubAccessToken, mIssue))

    # Load the scratch buffer
    skye#ManageScratchBuffer(1)

    set modifiable
    normal! ggdG
    setline(1, split(issuesAndComments, "\n"))
    set nomodifiable
enddef

# }}}
# Highlights {{{
def skye#InitListBuffer()
    if !exists("g:SkyeActiveUrl")
        echom "g:SkyeActiveUrl not set yet. This shouldn't happen (unless you've been setting stuff manually :eyes:)"
        return
    endif

    # Primary line highlights
    syn match SkyeIssue '\v^#.*' contains=SkyeIssueID,SkyeState,SkyeAuthor
    syn match SkyeIssueID '\v#\d+\s' contained
    syn match SkyeState '\v\[.\]\ze\t' contained 
    syn match SkyeAuthor '\vby \zs.{-} at https?:.*$' contained contains=SkyeLink
    syn match SkyeLink '\vat https?:.*$' contained contains=normal
    # Reset the at - doesn't need to be highlighted, so match it to Normal
    syn match Normal 'at ' contained

    # Highlight the quota
    syn match QuotaLine '\v^Remaining quota:.*' contains=QuotaNumbers
    syn match QuotaNumbers '\v\d+' contained

    # Link color definitions
    hi QuotaLine guifg=purple
    hi link QuotaNumbers Number

    hi link SkyeIssueID Statement
    hi link SkyeState   Identifier
    hi link SkyeLink    Keyword
    hi link SkyeAuthor  Type

    # This should make the buffer look less like absolute garbage - there
    # shouldn't be any repos with 8 digit issues anyway
    # Might be a problem for other sources, but that's not a me problem yet
    setlocal sw=8
    setlocal ts=8
    # And disable list characters
    setlocal nolist

    # Local keybinds
    nnoremap <buffer> <F5> :call skye#ListIssues(g:SkyeActiveUrl, g:SkyeActiveFilter)<cr>

    if exists('*pandora#InitMarkdown')
        # Enable direct URL opening integration with Pandora
        call pandora#InitMarkdown()
    endif
enddef

def skye#InitIssueBuffer()
    # Keybinds
    nnoremap <buffer> <Plug>(SkyeBack) :if g:SkyeLastIssue != "-1" <bar> call skye#ShowIssue(g:SkyeLastIssue) <bar> else <bar> echo "Backstack empty" <bar> endif<CR>

    nmap <buffer> <F5> :call skye#ShowIssue(g:SkyeActiveIssue)<cr>
    nmap <buffer><silent> <BS> <Plug>(SkyeBack)
    
    if has("gui_running")
        # Browser-style mapping for gVim (doesn't work with terminal vim
        # because input processing).
        nmap <buffer><silent> <M-Left> <Plug>(SkyeBack)
    endif
enddef
# }}}

