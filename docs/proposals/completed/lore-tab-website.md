# Design Proposal: Lore Tab for ACKMUD Website

**Status:** Draft
**Date:** 2026-03-19

---

## 1. Problem

The website (`web/web_who_server.py`) currently exposes Help and SHelp tabs for browsing in-game reference text. The `lore/` directory contains 144 plain-text lore files describing factions, locations, cultures, characters, and history — but there is no web interface to browse or read them.

---

## 2. Lore File Format

Files in `lore/` follow this structure:

```
keywords KEYWORD1 KEYWORD2 ...
---
[first entry — plain prose, no flags prefix]

flags FLAG1 FLAG2 ...
---
[second entry — city/faction-specific text]

flags FLAG3 ...
---
[third entry — another city/faction-specific text]
```

Each `---` separator divides entries. The **first entry** is the section immediately after the initial `---` (following the `keywords` header) and before the first `flags` line. This is the universal lore content, not specific to any city or faction.

---

## 3. Approach

Mirror the existing help/shelp pattern exactly:

### 3.1 New routes in `web_who_server.py`

| Route | Behaviour |
|---|---|
| `/lore/` | Index page listing all lore topics, searchable via `?q=` |
| `/lore` | Redirect to `/lore/` |
| `/lores/` | Redirect to `/lore/` |
| `/lores/TOPIC` | Display the first entry of the named lore file |

### 3.2 First-entry extraction

A new helper function `_extract_first_lore_entry(content: str) -> str` will:
1. Split on `---` lines.
2. Skip the header block (the block containing `keywords ...`).
3. Return the next block of text as the first entry.
4. If no first entry exists, return the full content as a fallback.

### 3.3 `LORE_DIR` constant

Add `LORE_DIR = ROOT_DIR / "lore"` alongside the existing `HELP_DIR` and `SHELP_DIR`.

### 3.4 Changes to `base.html`

- Add `<a href='/lore/'>Lore</a>` to the `<nav>` bar (after SHelp, before Maps).
- Add a third search form to `.help-forms`:
  ```html
  <form method='get' action='/lore/'>
  <label for='lore-q'>Lore:</label>
  <input id='lore-q' name='q' placeholder='topic'>
  <button type='submit'>Open lore</button>
  </form>
  ```

---

## 4. Affected Files

| File | Change |
|---|---|
| `web/web_who_server.py` | Add `LORE_DIR`, 4 new route handlers, `_extract_first_lore_entry()` helper, `_send_lore_topic_page()` method |
| `web/templates/base.html` | Add Lore nav link and Lore search form |

---

## 5. What Is NOT Displayed

Only the first entry of each lore file is shown — the universal, unflagged prose block. City- and faction-specific entries (those preceded by a `flags` line) are deliberately excluded from the web display.

---

## 6. Trade-offs

| Decision | Rationale |
|---|---|
| First entry only, no flags | Keeps presentation clean; flag-specific entries presuppose context a web reader may not have |
| Reuses `_build_topic_index_page()` unchanged | The lore index (listing + search) is identical in behaviour to help/shelp — no duplication needed |
| Separate `_send_lore_topic_page()` method | Lore topics require content extraction logic that help/shelp don't, so a dedicated method is cleaner than adding a parameter to the generic one |
| No new template file | The lore topic page uses the same `<pre>` block layout as help/shelp — no new HTML template is needed |

---

## 7. No New Tests Required

This is a purely presentational web layer with no game-logic changes. The existing integration test covers server boot; the web routes are not exercised by the integration test. No unit test additions are planned.
