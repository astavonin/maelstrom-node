# maelstrom-node

C++ library for Maelstrom challenge.

## How To Use it

The most straightforward way to use the library is `bazel_dep` + `git_override`. Simply add to your `MODULE.bazel`:

```bazel
bazel_dep(name = "maelstrom_node", version = "1.0.2")	# version <- anything here
git_override(
    module_name = "maelstrom_node",
    commit = "latest_commit_hash",
    remote = "https://github.com/astavonin/maelstrom-node.git",
)
```

where `commit` is SHA for latest commit.

And update a corresponding `cc_binary` rule:

```bazel
cc_binary(
...
    deps = [
        "@maelstrom_node",
...
    ],
)
```

Feel free to use [this commit](https://github.com/astavonin/maelstrom-challenges/commit/cc146678095a183a41ef9f4bac37691c70476825) as a reference.

## How to use with CMake?

CMake is not supported.