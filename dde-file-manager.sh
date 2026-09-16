#!/bin/bash

if [ "$1" == "--show-item" ]; then
  real_path=$(realpath -m "$2" | sed "s:^$HOME/xwechat_files/:$HOME/.var/app/$FLATPAK_ID/xwechat_files/:")

  if [ -f "$real_path" ]; then
    echo "Calling org.freedesktop.portal.OpenURI.OpenDirectory $real_path"
    gdbus call --session \
      --dest org.freedesktop.portal.Desktop \
      --object-path /org/freedesktop/portal/desktop \
      --method org.freedesktop.portal.OpenURI.OpenDirectory \
      "" 3 '{}' 3< "$real_path" >/dev/null
  else
    zenity --error --text "Path $real_path not found" --icon-name com.tencent.WeChat
  fi
fi
