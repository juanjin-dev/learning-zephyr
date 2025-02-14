# SPDX-License-Identifier: BSL-1.0

cmake_minimum_required(VERSION 3.30)

find_package(Zephyr HINTS $ENV{ZEPHYR_BASE})

project(001-vscode
        VERSION 1.0.0
        LANGUAGES C ASM
        DESCRIPTION "VSCode settings for Zephyr"
        HOMEPAGE_URL "https://github.com/jin-iot/learning-zephyr")

add_subdirectory(src)