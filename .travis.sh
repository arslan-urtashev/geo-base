#!/usr/bin/env bash

function install_linux_bazel() {
    wget https://github.com/bazelbuild/bazel/releases/download/0.2.0/bazel-0.2.0-installer-linux-x86_64.sh
    chmod +x bazel-0.2.0-installer-linux-x86_64.sh
    ./bazel-0.2.0-installer-linux-x86_64.sh --user
    export PATH="$PATH:$HOME/bin"
}

function install_osx_bazel() {
    wget https://github.com/bazelbuild/bazel/releases/download/0.2.0/bazel-0.2.0-installer-darwin-x86_64.sh
    chmod +x bazel-0.2.0-installer-darwin-x86_64.sh
    ./bazel-0.2.0-installer-darwin-x86_64.sh --user
    export PATH="$PATH:$HOME/bin"
}

if [ $(uname -s) == "Linux" ]; then
    install_linux_bazel
else
    install_osx_bazel
fi
