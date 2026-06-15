# Исходники GitHub Wiki

Вики TAKT OS — **только навигация на русском**. Тексты документации лежат в [`docs/`](../) репозитория и не дублируются здесь.

## Страницы

| Файл | Назначение |
|------|------------|
| [Home.md](Home.md) | Главная вики (оглавление со ссылками на `docs/`) |
| [_Sidebar.md](_Sidebar.md) | Боковое меню GitHub Wiki |

## Импорт на GitHub

```bash
git clone https://github.com/Developer-RU/Takt-OS.wiki.git
cp docs/wiki/Home.md Takt-OS.wiki/
cp docs/wiki/_Sidebar.md Takt-OS.wiki/
cd Takt-OS.wiki
git add Home.md _Sidebar.md
git commit -m "Вики: навигация на русском, без дублирования docs"
git push
```

> Сначала включите Wiki в настройках репозитория и создайте первую страницу, если репозиторий `.wiki` ещё не существует.

## Обновление

При добавлении нового файла в `docs/` достаточно обновить таблицы в `Home.md` и при необходимости `_Sidebar.md`. Содержимое статей правится **только** в `docs/`.
