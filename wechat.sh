#!/bin/bash

setup_ime_env() {
  if [[ "$XMODIFIERS" =~ fcitx ]]; then
    [ -z "$QT_IM_MODULE" ] && export QT_IM_MODULE=fcitx
    [ -z "$GTK_IM_MODULE" ] && export GTK_IM_MODULE=fcitx
  elif [[ "$XMODIFIERS" =~ ibus ]]; then
    [ -z "$QT_IM_MODULE" ] && export QT_IM_MODULE=ibus
    [ -z "$GTK_IM_MODULE" ] && export GTK_IM_MODULE=ibus
  fi
}

setup_ime_env

if [[ ! -e ".xwechat/uuid" ]]; then
  mkdir -p .xwechat
  cat /proc/sys/kernel/random/uuid > .xwechat/uuid
fi

export LD_PRELOAD=/app/lib/libredirect.so
exec /app/extra/wechat/wechat "$@"
