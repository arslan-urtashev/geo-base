#!/usr/bin/env bash

function install_linux_bazel() {
    wget https://github.com/bazelbuild/bazel/releases/download/0.1.4/bazel-0.1.4-installer-linux-x86_64.sh
    chmod +x bazel-0.1.4-installer-linux-x86_64.sh
    ./bazel-0.1.4-installer-linux-x86_64.sh --user
    export PATH="$PATH:$HOME/bin"
}

function install_osx_bazel() {
    wget https://github.com/bazelbuild/bazel/releases/download/0.1.4/bazel-0.1.4-installer-darwin-x86_64.sh
    chmod +x bazel-0.1.4-installer-darwin-x86_64.sh
    ./bazel-0.1.4-installer-darwin-x86_64.sh --user
    export PATH="$PATH:$HOME/bin"
}

function install_osx_java8() {
    wget --no-cookies --no-check-certificate --header \"Cookie: gpw_e24=http%3A%2F%2Fwww.oracle.com%2F; oraclelicense=accept-securebackup-cookie\" http://download.oracle.com/otn-pub/java/jdk/8u66-b17/jdk-8u66-macosx-x64.dmg
    hdiutil mount jdk-8u66-macosx-x64.dmg
    sudo installer -pkg /Volumes/JDK\ 8\ Update\ 66/JDK\ 8\ Update\ 66.pkg -target LocalSystem
}

if [ $(uname -s) == "Linux" ]; then
    install_linux_bazel
else
    install_osx_java8
    install_osx_bazel
fi
