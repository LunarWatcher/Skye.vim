vim9script

# TODO: figure out which extension is used on Mac
var ext = "so"
if has("win32")
    ext = "dll"
endif

var binary = expand("<sfile>:p:h") .. "/../build/lib/libskye." .. ext
var forceLoad = libcall(binary, "verifyLoaded", 0)
if (forceLoad != "Dynamic library successfully loaded")
    echoerr "Failed to load dynamic library"
    finish
endif

def skye#Verify()
    libcall(binary, 'verifyLoaded', 0)
enddef

# API {{{

def skye#SetUrl(url: string)
    var response = libcall(binary, 'setUrl', url)
    if response != ""
        echo response
    endif
enddef

def skye#DetectUrl(url = "origin")
    var detectedUrl = libcall(binary, 'detectUrlFromRemote', url)
    echo "Detected URL: " .. detectedUrl
enddef

def skye#ListIssues()
    var issues = libcall(binary, 'getIssues', 0)
    echo issues 
enddef

def skye#SetToken(token: string)
    libcall(binary, 'loadGHToken', token)
enddef

# }}}

if exists('g:SkyeGitHubAccessToken')
    skye#SetToken(g:SkyeGitHubAccessToken)
    # Unlet the token. Not strictly speaking necessary, but makes access
    # harder for other plugins.
    unlet g:SkyeGitHubAccessToken
endif
