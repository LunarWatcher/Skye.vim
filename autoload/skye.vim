vim9script

# TODO: figure out which extension is used on Mac
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

        setlocal ft=markdown

        setlocal nomodifiable
        
        # Set the cached buffer ID for internal management
        if switch == 0
            listBuffID = bufnr()
        else
            issueBuffID = bufnr()
        endif

        nmap <buffer> <leader>fi <Plug>(ShowIssue)
    else
        # If the buffer exists, and is in the current window (and in the
        # current tab, because Vim), we go to the buffer
        # Not sure why this clusterfuck is required, but win_gotoid(listBuffId)
        # doesn't work. Guess Vim is special :dontfeelsoblob:
        win_gotoid(get(win_findbuf(id), 0))
    endif

enddef

def skye#ListIssues(url = g:SkyeIssueUrl)
    if type(url) != v:t_string
        echoerr "URL isn't a string; aborting function"
        return
    endif
    g:SkyeActiveUrl = url
    # TODO: fix token portability. Early determining which token to use might
    # be tricky
    # TODO: caching (only relevant when buffer management has been done
    # properly. Probably anyway)
    var issues = libcall(binary, 'getIssues', SeparateStrings(url, g:SkyeGitHubAccessToken, "?state=all"))

    skye#ManageScratchBuffer(0)

    set modifiable
    silent! setline(1, split(issues, "\n"))
    set nomodifiable
    
enddef

def skye#ShowIssue(issue: string = "-1")
    var mIssue = issue
    if issue == "-1"
        # This shouldn't be a false positive, ever.
        var currLine = getline('.')
        if currLine =~ '\v^# #'
            mIssue = currLine[3 : stridx(currLine, ' ', 3) - 2]
        else
            var currToken = expand('<cWORD>')
            if currToken =~ '\v^#[a-zA-Z\-0-9]+$'
                mIssue = currToken[1 :]
            else
                # Final fallback; no token found
                echom "Failed to find an issue reference in the current line or token"
                return
            endif
        endif
    endif
    var issuesAndComments = libcall(binary, 'getIssue', SeparateStrings(g:SkyeActiveUrl, g:SkyeGitHubAccessToken, mIssue))

    skye#ManageScratchBuffer(1)

    set modifiable
    setline(1, split(issuesAndComments, "\n"))
enddef

# }}}

