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

def skye#Load()
enddef

