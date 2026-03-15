# World Links — Cross-Area Room Connections

This document catalogs all known cross-area exits: room exits whose destination vnum falls outside the source area's own `V` envelope. Use this as the authoritative reference when adding, removing, or auditing world connections. Never verify cross-area links by reading individual `.are` files; update this document instead.

**Status key:**
- **Active** — both sides of the link are live and bidirectional.
- **One-way** — only one side of the link currently exists.
- **Deprecated** — a link that existed in a prior version and whose source side has been removed, but the destination side in the external area still exists (orphaned).
- **Planned** — a link defined in an area plan but not yet built.

---

## Midgaard (917–1116)

| From | Dir | To | Notes | Status |
|------|-----|----|-------|--------|
| `midgaard` 921 | — | `gloamvault` 5031 | Midgaard → Gloamvault | Active |
| `midgaard` 926 | — | `great_northern_forest` 3751 | Midgaard → GNF north | Active |
| `midgaard` 930 | — | `pubdun` 2294 | Midgaard → Public Dungeons | Active |
| `midgaard` 941 | — | `nightfall_catacombs` 4681 | Midgaard → Catacombs of Nightfall | Active |
| `midgaard` 946 | — | `sepulcher_pasture` 5106 | Midgaard → Sepulcher Pasture | Active |
| `midgaard` 1017 | — | `rocroad` 1117 | Midgaard → Roc Road | Active |
| `midgaard` 1026 | — | `newschool` 4564 | Midgaard → Academy of Adventure | Active |
| `midgaard` 1036 | — | `eastern_desert` 1832 | Midgaard → Eastern Desert | Active |
| **`midgaard` 1106** | **south** | **`wood` 1434** | **Southern Wild Gate → Eccentric Woodland entry** | **Active** |

Room 1106 ("The Southern Wild Gate") in `midgaard.are` has exit D2 (south) destination `0 -1 1434`. The reverse (`wood` 1434 north → 1106) must be maintained in `wood.are`.

---

## Eccentric Woodland — wood.are (1432–1831)

### Canonical Links (must be maintained)

| From | Dir | To | Notes | Status |
|------|-----|----|-------|--------|
| `wood` 1434 | north | `midgaard` 1106 | Woodland entry ↔ Southern Wild Gate | Active |
| `wood` 1830 | south | `rakuen` 4239 | Woodland south exit ↔ Rakuen North Dispatch Gate | Active |
| `rakuen` 4239 | north | `wood` 1830 | Rakuen North Dispatch Gate ↔ Woodland south exit | Active |

The north link (`1434 ↔ 1106`) is the primary canonical entry and must never be removed without updating `midgaard.are` room 1106 D2.

The south link (`1830 ↔ 4239`) is bidirectional: `wood` 1830 D2(south) → 4239 and `rakuen` 4239 D0(north) → 1830.

### Legacy Links from Old wood.are (Deprecated by Rebuild)

The old `wood.are` (vnums 1432–1531) had bidirectional cross-area connections that were removed when `wood.are` was rebuilt. Two external areas that still exist retain orphaned exits pointing into the woodland; those exits land in rooms with different names and no reverse connection.

| External Area | Ext Room | Destination | Old Woodland Room Purpose | Status |
|--------------|----------|-------------|--------------------------|--------|
| `sunken_sanctum` 2266 | → | `wood` 1455 | Old woodland side-exit (sanctum passage) | **Deprecated** |
| `kelshadra` 2232 | → | `wood` 1456 | Old woodland side-exit (Kel'Shadra passage) | **Deprecated** |

**Action required (future work):** Each external area listed above should have its inbound exit to the woodland removed or redirected. Until that cleanup occurs, players using those exits will arrive in a Zone A/B room in the new Eccentric Woodland with no return path.

---

## Roc Road (1117–1215)

| From | Dir | To | Notes | Status |
|------|-----|----|-------|--------|
| `rocroad` 1118 | — | `violet_eclipse_cathedral` 5181 | Roc Road → Cathedral | Active |
| `rocroad` 1159 | — | `kiess` 3398 | Roc Road → Kiess | Active |

---

## Other Active Cross-Area Links (summary)

| From | To | Notes |
|------|----|-------|
| `newadept` 1 | `withered_depths` 2464 (approx zone) | Newadept portal exits |
| `playerhousing` 87 | `midgaard` 917 | Housing ↔ Midgaard |
| `voidcitadel` 88 | `midgaard` 1148 | Void Citadel ↔ Midgaard |
| `voidcitadel`/`umbra_heartspire` 3064 | `midgaard` 1148 | Heartspire ↔ Midgaard |
| `withered_depths` 291 | `confusn` 2434 | Withered Depths ↔ Forest of Confusion |
| `verdantgreen` 2739 | `confusn` 2434 | Verdant Depths ↔ Forest of Confusion |
| `akhenet` 792 | `eastern_desert` 1840 | Akh'enet ↔ Eastern Desert |
| `eastern_desert` 1835 | `arroyo` 4439 | Eastern Desert ↔ Arroyo |
| `eastern_desert` 1836 | `sultans_palace` 5256 | Eastern Desert ↔ Sultan's Palace |
| `eastern_desert` 1839 | `scorched_wastes` 4881 | Eastern Desert ↔ Scorched Wastes |
| `eastern_desert` 1851 | `the_great_oasis` 1932 | Eastern Desert ↔ Great Oasis |
| `eastern_desert` 1855 | `lost_city` 5331 | Eastern Desert ↔ Lost City |
| `the_great_oasis` 1933–1941 | `the_northern_oasis` 2212–2220 | Great Oasis ↔ Northern Oasis (multi) |
| `the_great_oasis` 2031 | `the_southern_oasis` 2539 | Great Oasis ↔ Southern Oasis |
| `kelshadra` 2232 | `wood` 1456 | **Deprecated** (see above) |
| `sunken_sanctum` 2266 | `wood` 1455 | **Deprecated** (see above) |
| `pubdun` 2294 | `midgaard` 930 | Public Dungeons ↔ Midgaard |
| `northern_pyramid` 2895 | `the_northern_oasis` 2211 | Northern Pyramid ↔ Northern Oasis |
| `kiess` 3398 | `rocroad` 1159 | Kiess ↔ Roc Road |
| `kowloon` 3534 | `great_northern_forest` 4233 | Kowloon ↔ GNF |
| `newschool` 4564 | `forest_preserve` 4631 | Academy ↔ Forest Preserve |
| `nightfall_catacombs` 4681 | `midgaard` 941 | Catacombs ↔ Midgaard |
| `gloamvault` 5031 | `midgaard` 921 | Gloamvault ↔ Midgaard |
| `sepulcher_pasture` 5106 | `midgaard` 946 | Sepulcher Pasture ↔ Midgaard |
| `violet_eclipse_cathedral` 5181 | `rocroad` 1118 | Cathedral ↔ Roc Road |
| `sultans_palace` 5256 | `eastern_desert` 1836 | Sultan's Palace ↔ Eastern Desert |
| `lost_city` 5331 | `eastern_desert` 1855 | Lost City ↔ Eastern Desert |
| `wood` 1830 | `rakuen` 4239 | Eccentric Woodland ↔ Rakuen |

---

## Planned Links (not yet built)

| From | Dir | To | Notes |
|------|-----|----|-------|
| `thornwood` 441 | south | `graveyard` 11085 | Thornwood entry room → Graveyard. Exit was removed from `thornwood.are` because `graveyard.are` is not yet built. Requires a future graveyard area file with a reverse exit `11085 → 441`. |
