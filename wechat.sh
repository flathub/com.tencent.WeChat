#!/bin/bash

setup_ime_env() {
  if [[ "$XMODIFIERS" =~ fcitx && -z "$WAYLAND_DISPLAY" ]]; then
    [[ -z "$QT_IM_MODULE" ]] && export QT_IM_MODULE=fcitx
    [[ -z "$GTK_IM_MODULE" ]] && export GTK_IM_MODULE=fcitx
  elif [[ "$XMODIFIERS" =~ ibus && -z "$WAYLAND_DISPLAY" ]]; then
    [[ -z "$QT_IM_MODULE" ]] && export QT_IM_MODULE=ibus
    [[ -z "$GTK_IM_MODULE" ]] && export GTK_IM_MODULE=ibus
  fi
}

setup_cursor_env() {
  if [[ -z "$WAYLAND_DISPLAY" ]]; then
    [[ -z "$XCURSOR_PATH" ]] && export XCURSOR_PATH="${XDG_DATA_DIRS//://icons:}/icons"
  else
    # xdg-desktop-portal-gtk needs to be installed.
    theme=$(gdbus call --session --dest org.freedesktop.portal.Desktop \
      --object-path /org/freedesktop/portal/desktop \
      --method org.freedesktop.portal.Settings.Read \
      org.gnome.desktop.interface cursor-theme 2>/dev/null | cut -d"'" -f2)
    [[ -z "$XCURSOR_THEME" && -n "$theme" ]] && export XCURSOR_THEME="$theme"
  fi
}

setup_ime_env
setup_cursor_env

ulimit -c 0

exec /app/extra/wechat/wechat "$@"
