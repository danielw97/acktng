# World Links — Cross-Area Room Connections

This document catalogs all known cross-area exits: room exits whose destination vnum falls outside the source area's own `V` envelope. Use this as the authoritative reference when adding, removing, or auditing world connections. Never verify cross-area links by reading individual `.are` files; update this document instead.

**Status key:**
- **Active** — both sides of the link are live and bidirectional.
- **One-way** — only one side of the link currently exists.
- **Deprecated** — a link that existed in a prior version and whose source side has been removed, but the destination side in the external area still exists (orphaned).
- **Planned** — a link defined in an area plan but not yet built.

---

## Midgaard (3001–3200)

| From | Dir | To | Notes | Status |
|------|-----|----|-------|--------|
| `midgaard` 3005 | — | `gloamvault` 30100 | Midgaard → Gloamvault | Active |
| `midgaard` 3010 | — | `great_northern_forest` 15512 | Midgaard → GNF north | Active |
| `midgaard` 3014 | — | `pubdun` 9551 | Midgaard → Public Dungeons | Active |
| `midgaard` 3025 | — | `nightfall_catacombs` 21000 | Midgaard → Catacombs of Nightfall | Active |
| `midgaard` 3030 | — | `sepulcher_pasture` 30175 | Midgaard → Sepulcher Pasture | Active |
| `midgaard` 3101 | — | `rocroad` 3201 | Midgaard → Roc Road | Active |
| `midgaard` 3110 | — | `newschool` 20200 | Midgaard → Academy of Adventure | Active |
| `midgaard` 3120 | — | `eastern_desert` 8500 | Midgaard → Eastern Desert | Active |
| **`midgaard` 3190** | **south** | **`wood` 7302** | **Southern Wild Gate → Eccentric Woodland entry** | **Active** |

Room 3190 ("The Southern Wild Gate") in `midgaard.are` has exit D2 (south) destination `0 -1 7302`. The reverse (`wood` 7302 north → 3190) must be maintained in `wood.are`.

---

## Eccentric Woodland — wood.are (7300–7699)

### Canonical Links (must be maintained)

| From | Dir | To | Notes | Status |
|------|-----|----|-------|--------|
| `wood` 7302 | north | `midgaard` 3190 | Woodland entry ↔ Southern Wild Gate | Active |
| `wood` 7698 | south | `rakuen` 16000 | Woodland south exit ↔ Rakuen North Dispatch Gate | Active |
| `rakuen` 16000 | north | `wood` 7698 | Rakuen North Dispatch Gate ↔ Woodland south exit | Active |

The north link (`7302 ↔ 3190`) is the primary canonical entry and must never be removed without updating `midgaard.are` room 3190 D2.

The south link (`7698 ↔ 16000`) is bidirectional: `wood` 7698 D2(south) → 16000 and `rakuen` 16000 D0(north) → 7698.

### Legacy Links from Old wood.are (Deprecated by Rebuild)

The old `wood.are` (vnums 7300–7399) had bidirectional cross-area connections that were removed when `wood.are` was rebuilt. Two external areas that still exist retain orphaned exits pointing into the woodland; those exits land in rooms with different names and no reverse connection.

| External Area | Ext Room | Destination | Old Woodland Room Purpose | Status |
|--------------|----------|-------------|--------------------------|--------|
| `sunken_sanctum` 9500 | → | `wood` 7323 | Old woodland side-exit (sanctum passage) | **Deprecated** |
| `kelshadra` 9400 | → | `wood` 7324 | Old woodland side-exit (Kel'Shadra passage) | **Deprecated** |

**Action required (future work):** Each external area listed above should have its inbound exit to the woodland removed or redirected. Until that cleanup occurs, players using those exits will arrive in a Zone A/B room in the new Eccentric Woodland with no return path.

---

## Roc Road (3201–3299)

| From | Dir | To | Notes | Status |
|------|-----|----|-------|--------|
| `rocroad` 3202 | — | `violet_eclipse_cathedral` 30250 | Roc Road → Cathedral | Active |
| `rocroad` 3243 | — | `kiess` 13059 | Roc Road → Kiess | Active |

---

## Other Active Cross-Area Links (summary)

| From | To | Notes |
|------|----|-------|
| `newadept` 166 | `withered_depths` 9725 (approx zone) | Newadept portal exits |
| `playerhousing` 311 | `midgaard` 3001 | Housing ↔ Midgaard |
| `voidcitadel` 610 | `midgaard` 3232 | Void Citadel ↔ Midgaard |
| `voidcitadel`/`umbra_heartspire` 11200 | `midgaard` 3232 | Heartspire ↔ Midgaard |
| `withered_depths` 1000 | `confusn` 9695 | Withered Depths ↔ Forest of Confusion |
| `verdantgreen` 10000 | `confusn` 9695 | Verdant Depths ↔ Forest of Confusion |
| `akhenet` 2000 | `eastern_desert` 8508 | Akh'enet ↔ Eastern Desert |
| `eastern_desert` 8503 | `arroyo` 19900 | Eastern Desert ↔ Arroyo |
| `eastern_desert` 8504 | `sultans_palace` 30325 | Eastern Desert ↔ Sultan's Palace |
| `eastern_desert` 8507 | `scorched_wastes` 22500 | Eastern Desert ↔ Scorched Wastes |
| `eastern_desert` 8519 | `the_great_oasis` 8600 | Eastern Desert ↔ Great Oasis |
| `eastern_desert` 8523 | `lost_city` 30400 | Eastern Desert ↔ Lost City |
| `the_great_oasis` 8601–8609 | `the_northern_oasis` 8880–8888 | Great Oasis ↔ Northern Oasis (multi) |
| `the_great_oasis` 8699 | `the_southern_oasis` 9800 | Great Oasis ↔ Southern Oasis |
| `kelshadra` 9400 | `wood` 7324 | **Deprecated** (see above) |
| `sunken_sanctum` 9500 | `wood` 7323 | **Deprecated** (see above) |
| `pubdun` 9551 | `midgaard` 3014 | Public Dungeons ↔ Midgaard |
| `northern_pyramid` 10156 | `the_northern_oasis` 8879 | Northern Pyramid ↔ Northern Oasis |
| `kiess` 13059 | `rocroad` 3243 | Kiess ↔ Roc Road |
| `kowloon` 14095 | `great_northern_forest` 15994 | Kowloon ↔ GNF |
| `newschool` 20200 | `forest_preserve` 20800 | Academy ↔ Forest Preserve |
| `nightfall_catacombs` 21000 | `midgaard` 3025 | Catacombs ↔ Midgaard |
| `gloamvault` 30100 | `midgaard` 3005 | Gloamvault ↔ Midgaard |
| `sepulcher_pasture` 30175 | `midgaard` 3030 | Sepulcher Pasture ↔ Midgaard |
| `violet_eclipse_cathedral` 30250 | `rocroad` 3202 | Cathedral ↔ Roc Road |
| `sultans_palace` 30325 | `eastern_desert` 8504 | Sultan's Palace ↔ Eastern Desert |
| `lost_city` 30400 | `eastern_desert` 8523 | Lost City ↔ Eastern Desert |
| `wood` 7698 | `rakuen` 16000 | Eccentric Woodland ↔ Rakuen |

---

## Planned Links (not yet built)

| From | Dir | To | Notes |
|------|-----|----|-------|
| *(none)* | — | — | All previously planned links are now active. |
