This is a very simple volume changer for X11. All it does is grab the keyboard volume buttons and change the ALSA master volume accordingly.

### Usage

```sh
$ ./build.sh
$ cp vold /usr/local/bin/
```

---

```
# .xinitrc

vold &

exec startfluxbox # your favorite wm here
```
