#!/usr/bin/env bash

install_linux_dependencies() {
    sudo add-apt-repository ppa:webupd8team/java
    sudo apt-get update
    sudo apt-get install oracle-java8-installer
    sudo apt-get install pkg-config zip g++ zlib1g-dev unzip
}

install_linux_bazel() {
    wget https://github.com/bazelbuild/bazel/releases/download/0.1.4/bazel-0.1.4-installer-linux-x86_64.sh
    chmod +x bazel-0.1.4-installer-linux-x86_64.sh
    ./bazel-0.1.4-installer-linux-x86_64.sh --user
    export PATH="$PATH:$HOME/bin"
}

install_osx_bazel() {
    wget https://github.com/bazelbuild/bazel/releases/download/0.1.4/bazel-0.1.4-installer-darwin-x86_64.sh
    chmod +x bazel-0.1.4-installer-darwin-x86_64.sh
    ./bazel-0.1.4-installer-darwin-x86_64.sh --user
    export PATH="$PATH:$HOME/bin"
}

if [ $(uname -s) == "Linux" ]; then
    install_linux_dependencies()
    install_linux_bazel()
else
    install_osx_bazel()
fi
