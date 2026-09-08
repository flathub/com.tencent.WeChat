#!/bin/bash

setup_ime_env() {
  if [[ "$XMODIFIERS" =~ fcitx && -z "$WAYLAND_DISPLAY" ]]; then
    [ -z "$QT_IM_MODULE" ] && export QT_IM_MODULE=fcitx
    [ -z "$GTK_IM_MODULE" ] && export GTK_IM_MODULE=fcitx
  elif [[ "$XMODIFIERS" =~ ibus && -z "$WAYLAND_DISPLAY" ]]; then
    [ -z "$QT_IM_MODULE" ] && export QT_IM_MODULE=ibus
    [ -z "$GTK_IM_MODULE" ] && export GTK_IM_MODULE=ibus
  fi
}

setup_ime_env

ulimit -c 0

exec /app/extra/wechat/wechat "$@"
