# Area Index

All areas listed in `area/area.lst` order with their vnum envelopes, level ranges, and canonical keywords.
The `area.lst` file must remain in non-decreasing order by `V` minimum vnum (see `area_file_spec.md` §13 ordering constraint).

| File | Vnum Range | Level Range | Display Name | Keyword(s) |
|------|-----------|-------------|--------------|------------|
| `newadept.are` | 1–85 | 150 | The Wizard's Keep | `none` |
| `playerhousing.are` | 86–87 | 150 | Player Housing | `none` |
| `voidcitadel.are` | 88–290 | 150–5 | The Void Citadel of Kel'Shadra | `kelshadra` |
| `withered_depths.are` | 291–440 | 80–100 | The Withered Depths | `withereddepths` |
| `thornwood.are` | 441–590 | 50–80 | Thornwood | `thornwood` |
| `shadowmere.are` | 591–269 | 30–60 | Shadowmere, the Blighted Realm | `shadowmere blighted realm` |
| `akhenet.are` | 270–916 | 70–90 | The Sands of Akh'enet | `sands akhenet desert ruins` |
| `midgaard.are` | 917–407 | 0 (city) | The City of Midgaard | `midgaard` |
| `rocroad.are` | 408–606 | 10–120 | Roc Road and the Forest Path | `rocroad` |
| `midgaard_shops.are` | 607–100 | 0 (city) | Midgaard Class Shop Extensions | `midgaard_shops` |
| `rocroad_byways.are` | 507–606 | 10–120 | Roc Road Byways and Warrens | `rocroad_byways` |
| `wood.are` | 101–1831 | 18–92 | The Eccentric Woodland | `eccentricwoodland` |
| `eastern_desert.are` | 1832–1931 | 30–100 | The Eastern Desert | `eastern_desert` |
| `the_great_oasis.are` | 1932–823 | 40–60 | The Great Oasis | `the great oasis` |
| `the_northern_oasis.are` | 824–2231 | 45–65 | The Northern Oasis | `the northern oasis` |
| `kelshadra.are` | 2232–2265 | 150–5 | The Crypts of Kel'Shadra | `kelshadra crypts shadow dark` |
| `sunken_sanctum.are` | 2266–2293 | 150 | The Sunken Sanctum | `sunken sanctum` |
| `pubdun.are` | 2293–2338 | 5–35 | Public Dungeons | `publicdungeons` |
| `confusn.are` | 2339–2538 | 10–60 | Forest of Confusion | `forestconfusion` |
| `the_southern_oasis.are` | 2539–2738 | 50–70 | The Southern Oasis | `the southern oasis` |
| `verdantgreen.are` | 2739–2863 | 75–95 | The Verdant Depths | `verdantdepths` |
| `northern_pyramid.are` | 2864–979 | 100–120 | Northern Pyramid | `northernpyramid` |
| `umbra_heartspire.are` | 980–345 | 25–40 | The Umbra Heartspire | `umbra_heartspire` |
| `southern_pyramid.are` | 346–3338 | 120–140 | Southern Pyramid | `southernpyramid` |
| `kiess.are` | 3339–3438 | 0 (city) | Kiess | `kiess` |
| `kowloon.are` | 3439–1304 | 1–5 | Kowloon | `kowloon` |
| `mafdet.are` | 1305–3738 | 55–80 | Port Mafdet | `mafdet` |
| `great_northern_forest.are` | 3739–4238 | 1–5 | The Great Northern Forest | `greatnorthernforest` |
| `rakuen.are` | 4239–4438 | 45–95 | Rakuen, City of the Last Promise | `rakuen` |
| `arroyo.are` | 4439–4563 | 55–75 | The Arroyo | `arroyo` |
| `newschool.are` | 4564–4630 | 1–10 | Academy of Adventure | `soeacademy` |
| `forest_preserve.are` | 4631–4680 | 1–10 | Whispering Forest Preserve | `forestpreserve` |
| `nightfall_catacombs.are` | 4681–4880 | 10–25 | The Catacombs of Nightfall | `nightfall catacombs reliquary ossuary blackchoir` |
| `scorched_wastes.are` | 4881–4955 | 65–85 | The Scorched Wastes | `scorched wastes desert` |
| `scorchsands.are` | 4956–5030 | 65–85 | The Scorching Sands | `scorching_sands` |
| `gloamvault.are` | 5031–5105 | 5–20 | The Gloamvault of Kel'Shadra | `kelshadra gloamvault gothic dungeon` |
| `sepulcher_pasture.are` | 5106–5180 | 15–30 | Sepulcher Pasture | `sepulcher pasture` |
| `violet_eclipse_cathedral.are` | 5181–5255 | 20–35 | Cathedral of the Violet Eclipse | `violet eclipse cathedral gothic dark` |
| `sultans_palace.are` | 5256–5330 | 30–50 | The Sultan's Palace | `sultan palace gothic midnight` |
| `lost_city.are` | 5331–5410 | 35–55 | Lost City | `lost city desert ruins` |
| `pyramid.are` | 5411–5623 | 90–100 | The Great Pyramid | `pyramid ancient egypt pharaoh` |
| `khardaan.are` | 5624–5723 | 60–80 | Khar'Daan, the Sunken Necropolis | `khardaan necropolis desert dune ancient burial` |
| `saltglass_reach.are` | 5724–6123 | 55–80 | The Saltglass Reach | `saltglass reach desert littoral cairn` |
| `cinderteeth_mountains.are` | 6124–6723 | 140–150 | The Cinderteeth Mountains | `cinderteeth mountains volcanic caldera ventspeaker` |
| `ceiling.are` | 32501–32766 | 1 | The Topmost Area (meta/helps) | `helps` |

## Notes

- Vnum ranges may overlap between adjacent areas (e.g., `kelshadra` 2232–9549 and `sunken_sanctum` 2266–9599 share 2266–9549 as an envelope). Overlapping envelopes are legal as long as no two areas define the same individual room, mobile, or object vnum.
- `area.lst` ordering is validated by `test_area_format.c`: each area's `V` minimum must be ≥ the previous area's `V` minimum; ties are broken by `V` maximum. See `area_file_spec.md` §13.
- `wood.are` was expanded from `V 623 722` to `V 623 1831` as part of the Eccentric Woodland rebuild. The range 7700–8499 is unallocated.
- **Rakuen** (`rakuen.are`, vnums 4239–4438) is the Eccentric Woodland's southern neighbor. Room 4239 is the North Dispatch Gate entry, connected bidirectionally to `wood` 1830 (`1830 south → 4239`, `4239 north → 1830`).
