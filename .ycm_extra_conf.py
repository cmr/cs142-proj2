def FlagsForFile(unused):
    return {
        "flags": ["-std=c++11", "-Wextra", "-I", ".", "-x", "c++"],
        "do_cache": False
    }
