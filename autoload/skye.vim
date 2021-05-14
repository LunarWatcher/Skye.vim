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

g:SkyeGitHubAccessToken = "Blank because stream"

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

# }}}
# API {{{

def skye#ShowIssue(issue: string)
    var issueAndComments = libcall(binary, 'getIssue', SeparateStrings(g:IssueUrl, g:SkyeGitHubAccessToken, issue))
enddef

def skye#ListIssues(url = g:SkyeIssueUrl)
    if type(url) != v:t_string
        echoerr "URL isn't a string; aborting function"
        return
    endif
    # TODO: fix token portability. Early determining which token to use might
    # be tricky
    # TODO: caching (only relevant when buffer management has been done
    # properly. Probably anyway)
    var issues = libcall(binary, 'getIssues', SeparateStrings(url, g:SkyeGitHubAccessToken, "?state=all"))

    # TODO: buffer management
    split
    noswapfile hide enew
    setlocal buftype=nofile
    setlocal bufhidden=hide
    file GitHub issues
    setlocal ft=markdown

    silent! setline(1, split(issues, "\n"))
    setlocal readonly
    
enddef

# }}}

