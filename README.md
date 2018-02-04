# ephy-scripts
Adds userscript support to Epiphany browser (aka GNOME Web).

# What
This is a WebKit2GTK WebExtension which simply reads Javascript files from some
directories, and evaluates them for every page.

Being a WebExtension, it is isolated from the GTK UI and thus can't modify it.

# How
Just place your scripts under either:

* `/usr/local/share/epiphany/userscripts`
* `/usr/share/epiphany/userscripts`
* `~/.local/share/epiphany/userscripts`

# Requirements
* `pkg-config`
* `webkit2gtk-web-extension-4.0`

# Installation
```sh
make
sudo make install
```

# License
GPL3
