# GitHub Wiki sources

The TAKT OS wiki is **navigation only**. Documentation text lives in [`docs/`](../) in the repository and is not duplicated here.

## Pages

| File | Purpose |
|------|---------|
| [Home.md](Home.md) | Wiki home (index with links to `docs/`) |
| [_Sidebar.md](_Sidebar.md) | GitHub Wiki sidebar menu |

## Import to GitHub

```bash
git clone https://github.com/Developer-RU/Takt-OS.wiki.git
cp docs/wiki/Home.md Takt-OS.wiki/
cp docs/wiki/_Sidebar.md Takt-OS.wiki/
cd Takt-OS.wiki
git add Home.md _Sidebar.md
git commit -m "Wiki: navigation only, no docs duplication"
git push
```

> Enable Wiki in repository settings and create the first page if the `.wiki` repository does not exist yet.

## Updates

When adding a new file under `docs/`, update the tables in `Home.md` and `_Sidebar.md` if needed. Article content is edited **only** in `docs/`.

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
