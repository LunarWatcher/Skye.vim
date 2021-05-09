vim9script

# TODO: figure out which extension is used on Mac
var ext = "so"
if has("win32")
    ext = "dll"
endif

var binary = expand("<sfile>:p:h") .. "/../build/lib/libskye." .. ext
var version = libcall(binary, "getVersion", 0)

def skye#Version()
    echo version
enddef
