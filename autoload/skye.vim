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

def skye#TestRemote(remote: string)
    echoerr libcall(binary, 'parseUrl', remote)
enddef

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
