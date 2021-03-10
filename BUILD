cc_library(
    name = "bitboard",
    srcs = ["src/bitboard.cpp"],
    hdrs = ["src/bitboard.hpp"],
)

cc_library(
    name = "eval",
    srcs = ["src/eval.cpp"],
    hdrs = ["src/eval.hpp"],
)

cc_library(
    name = "funcs",
    srcs = ["src/funcs.cpp"],
    hdrs = ["src/funcs.hpp"],
)

cc_library(
    name = "options",
    srcs = ["src/options.cpp"],
    hdrs = ["src/options.hpp"],

)
cc_library(
    name = "search",
    srcs = ["src/search.cpp"],
    hdrs = ["src/search.hpp"],
)

cc_library(
    name = "uci",
    srcs = ["src/uci.cpp"],
    hdrs = ["src/uci.hpp"],
)

cc_binary(
    name = "Megalodon",
    srcs = ["src/main.cpp"],
    deps = [
        ":bitboard",
        ":eval",
        ":funcs",
        ":options",
        ":search",
        ":uci",
    ],
)
