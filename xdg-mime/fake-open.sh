#!/bin/bash

if [ -n "$1" ]; then
    real_path=$(realpath -m "$1" | sed "s:^$HOME/xwechat_files/:$HOME/.var/app/$FLATPAK_ID/xwechat_files/:")
    xdg-open "$real_path"
fi
