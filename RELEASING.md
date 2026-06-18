# Releasing TAKT OS

Firmware and source archives are published as **[GitHub Releases](https://github.com/Developer-RU/Takt-OS/releases)**. There is no separate package registry (npm/Maven) — ESP32 binaries ship as release attachments.

## Versioning

Follow [Semantic Versioning](https://semver.org/) and update [CHANGELOG.md](CHANGELOG.md) before each release.

## Create a release

1. Ensure `main` is green in [CI](https://github.com/Developer-RU/Takt-OS/actions).
2. Bump `version` in `docs/_config.yml` and badges in `README.md` if needed.
3. Commit changelog for the new version.
4. Tag and push:

```bash
git tag -a v0.2.1 -m "TAKT OS v0.2.1"
git push origin v0.2.1
```

The [Release workflow](.github/workflows/release.yml) builds `demo_controller` and `recovery_app`, then uploads `.bin` files to the GitHub Release.

## Release artifacts

| File | Description |
|------|-------------|
| `demo_controller.bin` | Main factory application |
| `recovery_app.bin` | Recovery partition firmware |
| `demo_controller-bootloader.bin` | Bootloader (optional reflash) |
| `demo_controller-partitions.bin` | Partition table binary |
| `partitions.csv` | Human-readable partition layout |

Flash with ESP-IDF `idf.py flash` or `esptool.py` using offsets from `partitions.csv`.

## GitHub Pages

Documentation deploys via [.github/workflows/pages.yml](.github/workflows/pages.yml).

**Repository → Settings → Pages → Build and deployment:** select **GitHub Actions** (not legacy `/docs` only).

Live site: https://developer-ru.github.io/Takt-OS/

## Manual release (fallback)

```bash
gh release create v0.2.1 --title "TAKT OS v0.2.1" --notes-file CHANGELOG.md
```

Attach built binaries from `examples/*/build/` manually if CI is unavailable.
