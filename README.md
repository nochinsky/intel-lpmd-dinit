# intel-lpmd-dinit

Intel Low Power Mode Daemon for systems using dinit. Manages CPU power states on
Intel hybrid processors — picks the right cores for the job and parks the rest.

**Requirements**: Intel hybrid CPU (12th-gen or newer), Linux with cgroup v2,
dinit as init system, D-Bus. Works on Chimera, Artix+dinit, Alpine+dinit,
Void+dinit, or any distro with dinit — just adjust the package names below.

## Quick start (Chimera Linux)

```sh
# Install build dependencies
doas apk add gcc gmake pkgconf autoconf automake autoconf-archive \
  musl-devel libatomic-chimera-devel linux-headers \
  glib-devel libxml2-devel libnl-devel upower-devel gtk-doc-tools

# Build and install
./autogen.sh
gmake
doas gmake install

# Install the dinit service
doas cp /usr/local/share/dinit.d/intel_lpmd.dinit /etc/dinit.d/intel-lpmd

# Enable and start
doas dinitctl enable intel-lpmd
doas dinitctl start intel-lpmd

# Remove build dependencies (optional — only if you won't rebuild)
doas apk del gcc gmake autoconf automake autoconf-archive \
  musl-devel libatomic-chimera-devel linux-headers \
  glib-devel libxml2-devel libnl-devel upower-devel gtk-doc-tools
```

## How it works

The daemon reads `/usr/local/etc/intel_lpmd/intel_lpmd_config.xml` and your
system's power profile (Performance / Balanced / Power Saver).

- **Performance**: lpmd stays off — zero interference
- **Balanced**: lpmd runs in auto mode, picks the best state based on workload
- **Power Saver**: lpmd forces low power mode on, keeping only the efficiency cores active

Switch profiles through your desktop's power menu or with `powerprofilesctl`.

## Usage

```sh
# Check current state
doas intel_lpmd_control STATUS

# Switch modes manually
doas intel_lpmd_control AUTO
doas intel_lpmd_control ON
doas intel_lpmd_control OFF

# Man pages
man intel_lpmd
man intel_lpmd_config.xml
man intel_lpmd_control
```

## Testing

```sh
# Run in foreground to watch what it does
doas intel_lpmd --no-daemon --dbus-enable --loglevel=debug

# In another terminal, trigger a mode change
doas intel_lpmd_control ON
```

## Uninstall

```sh
doas dinitctl stop intel-lpmd
doas dinitctl disable intel-lpmd
doas rm /etc/dinit.d/intel-lpmd
doas gmake uninstall
```

## License

GPL v2. See COPYING.

Based on [intel_lpmd](https://github.com/intel/intel_lpmd) by Intel.
