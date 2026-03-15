#!/usr/bin/env python3
"""
Generate a stylized fantasy world map for ACK!MUD TNG.
Based on lore in /docs/lore/
"""

import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.patheffects as pe
from matplotlib.patches import FancyArrowPatch, Arc, FancyBboxPatch, Wedge
from matplotlib.path import Path
import matplotlib.patches as patches
import matplotlib.colors as mcolors
from matplotlib.collections import LineCollection
import random
import math

# ─── Seeded RNG for reproducibility ───────────────────────────────────────────
rng = random.Random(42)
np_rng = np.random.RandomState(42)

# ─── Canvas setup ─────────────────────────────────────────────────────────────
fig = plt.figure(figsize=(24, 18), dpi=120)
ax = fig.add_axes([0, 0, 1, 1])
ax.set_xlim(0, 240)
ax.set_ylim(0, 180)
ax.set_aspect('equal')
ax.axis('off')

# ─── PARCHMENT BACKGROUND ─────────────────────────────────────────────────────
# Multi-layer parchment effect
parchment_base = '#F5DEB3'   # wheat
parchment_dark = '#C8A87A'
parchment_mid  = '#E8C88A'
ink_color      = '#3B2A1A'
ink_faded      = '#5C3D2E'
ink_light      = '#7A5230'
blood_red      = '#8B1A1A'
forest_dark    = '#2D4A1E'
forest_mid     = '#3D6B2A'
forest_light   = '#5A8A3A'
forest_pale    = '#7BA85A'
desert_dark    = '#C8964A'
desert_mid     = '#D4A85C'
desert_light   = '#E8C878'
desert_pale    = '#F0D8A0'
water_dark     = '#2A4A6B'
water_mid      = '#3A6B8B'
water_light    = '#5A8BAA'
mountain_dark  = '#4A3A2A'
mountain_mid   = '#6A5A4A'
mountain_light = '#8A7A6A'
shadow_color   = '#1A1008'
gold_color     = '#B8860B'
gold_light     = '#DAA520'
ash_color      = '#A09080'
void_color     = '#1A0A2A'
void_light     = '#3A2A5A'
neon_cyan      = '#008B8B'
salt_color     = '#D0CCC0'

# Parchment gradient background
for i in range(200):
    y_frac = i / 200
    color = tuple(
        a + (b - a) * y_frac
        for a, b in zip(
            mcolors.to_rgb('#D4B483'),
            mcolors.to_rgb('#F0D890')
        )
    )
    ax.axhspan(i * 0.9, (i + 1) * 0.9, color=color, zorder=0)

# Parchment noise/staining effect
for _ in range(300):
    x = np_rng.uniform(0, 240)
    y = np_rng.uniform(0, 180)
    r = np_rng.uniform(0.5, 8)
    alpha = np_rng.uniform(0.02, 0.12)
    c = rng.choice(['#8B6914', '#5C3D1E', '#C8A45A', '#3B2A0A'])
    circle = plt.Circle((x, y), r, color=c, alpha=alpha, zorder=1)
    ax.add_patch(circle)

# ─── HELPER FUNCTIONS ─────────────────────────────────────────────────────────

def draw_mountain_range(ax, peaks, scale=1.0, color=mountain_mid, shadow=mountain_dark, zorder=10):
    """Draw stylized triangle mountains."""
    for (px, py) in peaks:
        h = rng.uniform(3.5, 6.5) * scale
        w = rng.uniform(3.0, 5.0) * scale
        # Shadow side
        shadow_tri = plt.Polygon(
            [[px, py], [px + w * 0.6, py - h * 0.2], [px + w * 0.15, py + h]],
            color=shadow, alpha=0.85, zorder=zorder
        )
        ax.add_patch(shadow_tri)
        # Light side
        light_tri = plt.Polygon(
            [[px, py], [px - w * 0.6, py - h * 0.2], [px + w * 0.15, py + h]],
            color=color, alpha=0.9, zorder=zorder
        )
        ax.add_patch(light_tri)
        # Snow cap
        snow_h = h * 0.25
        ax.add_patch(plt.Polygon(
            [[px + w * 0.15, py + h],
             [px - w * 0.15, py + h - snow_h],
             [px + w * 0.35, py + h - snow_h]],
            color='#F0EEE8', alpha=0.7, zorder=zorder + 1
        ))

def draw_forest(ax, cx, cy, radius, density=30, colors=None, zorder=8):
    """Draw a cluster of stylized trees."""
    if colors is None:
        colors = [forest_dark, forest_mid, forest_light, forest_pale]
    for _ in range(density):
        angle = rng.uniform(0, 2 * math.pi)
        dist  = rng.uniform(0, radius)
        tx = cx + dist * math.cos(angle)
        ty = cy + dist * math.sin(angle)
        th = rng.uniform(1.2, 2.8)
        col = rng.choice(colors)
        # Tree trunk
        ax.plot([tx, tx], [ty, ty + th * 0.3],
                color='#5C3D2E', linewidth=0.6, zorder=zorder)
        # Tree canopy (stacked triangles)
        for k in range(3):
            frac = 1 - k * 0.28
            tri_w = th * 0.7 * frac
            tri_h = th * 0.5
            tri_y = ty + th * 0.2 + k * th * 0.3
            ax.add_patch(plt.Polygon(
                [[tx - tri_w, tri_y],
                 [tx + tri_w, tri_y],
                 [tx, tri_y + tri_h]],
                color=col, alpha=0.88, zorder=zorder + k
            ))

def draw_water_body(ax, path_verts, path_codes=None, color=water_mid, alpha=0.75, zorder=5):
    """Fill a water body polygon."""
    if path_codes is None:
        codes = [Path.MOVETO] + [Path.LINETO] * (len(path_verts) - 2) + [Path.CLOSEPOLY]
        path_codes = codes
    p = Path(path_verts, path_codes)
    patch = patches.PathPatch(p, facecolor=color, edgecolor=water_dark,
                               linewidth=1.0, alpha=alpha, zorder=zorder)
    ax.add_patch(patch)

def wavy_line(ax, x0, y0, x1, y1, amplitude=0.5, freq=8, color=water_mid,
              lw=1.0, alpha=0.8, zorder=6):
    """Draw a wavy line (river / coastline accent)."""
    t = np.linspace(0, 1, 300)
    xs = x0 + t * (x1 - x0)
    ys = y0 + t * (y1 - y0)
    angle = math.atan2(y1 - y0, x1 - x0) + math.pi / 2
    wave = amplitude * np.sin(t * freq * math.pi * 2)
    xs += wave * math.cos(angle)
    ys += wave * math.sin(angle)
    ax.plot(xs, ys, color=color, linewidth=lw, alpha=alpha, zorder=zorder)

def curvy_road(ax, points, color=ink_faded, lw=1.2, style='--', zorder=9, alpha=0.7):
    """Draw a road between several points with slight curvature."""
    xs = [p[0] for p in points]
    ys = [p[1] for p in points]
    ax.plot(xs, ys, color=color, linewidth=lw, linestyle=style,
            alpha=alpha, zorder=zorder, solid_capstyle='round')

def place_label(ax, x, y, text, fontsize=7, color=ink_color, style='normal',
                weight='normal', rotation=0, zorder=20, ha='center', va='center',
                outline=True):
    """Place a styled label with optional outline."""
    if outline:
        ax.text(x, y, text, fontsize=fontsize, color='#F0D890',
                style=style, fontweight=weight, rotation=rotation,
                zorder=zorder - 1, ha=ha, va=va,
                fontfamily='serif',
                path_effects=[pe.withStroke(linewidth=2.5, foreground='#F0D890')])
    ax.text(x, y, text, fontsize=fontsize, color=color,
            style=style, fontweight=weight, rotation=rotation,
            zorder=zorder, ha=ha, va=va, fontfamily='serif')

def city_symbol(ax, x, y, size=1.5, color=ink_color, crown=False, zorder=15):
    """Draw a stylized city symbol (square with towers)."""
    # Main keep
    rect = mpatches.Rectangle((x - size, y - size * 0.8),
                                size * 2, size * 1.6,
                                color=color, alpha=0.85, zorder=zorder)
    ax.add_patch(rect)
    # Towers
    for tx in [x - size, x + size - size * 0.6]:
        tower = mpatches.Rectangle((tx, y - size * 0.8),
                                    size * 0.6, size * 2.0,
                                    color=color, alpha=0.9, zorder=zorder + 1)
        ax.add_patch(tower)
        # Battlements
        for bx in np.linspace(tx, tx + size * 0.6, 4)[:-1]:
            battlement = mpatches.Rectangle((bx, y + size * 1.2),
                                             size * 0.12, size * 0.4,
                                             color=color, alpha=0.9, zorder=zorder + 2)
            ax.add_patch(battlement)
    if crown:
        # Crown star for capital
        for angle in np.linspace(0, 2 * math.pi, 6)[:-1]:
            ax.plot([x + math.cos(angle) * size * 0.5],
                    [y + size * 1.8 + math.sin(angle) * size * 0.3],
                    '*', color=gold_color, markersize=4, zorder=zorder + 3)

def pyramid_symbol(ax, x, y, size=3.0, color=desert_dark, zorder=12):
    """Draw a stylized pyramid."""
    tri = plt.Polygon(
        [[x - size, y], [x + size, y], [x, y + size * 1.6]],
        color=color, alpha=0.85, edgecolor=ink_color, linewidth=0.8, zorder=zorder
    )
    ax.add_patch(tri)
    # Shading
    shade = plt.Polygon(
        [[x, y + size * 1.6], [x + size, y], [x + size * 0.1, y]],
        color=mountain_dark, alpha=0.4, zorder=zorder + 1
    )
    ax.add_patch(shade)
    # Eye of the pyramid glow (Black Sun Shard)
    ax.plot(x, y + size * 0.8, 'o', color='#1A0A0A', markersize=4, zorder=zorder + 2)
    ax.plot(x, y + size * 0.8, 'o', color='#FF4400', markersize=2, alpha=0.8, zorder=zorder + 3)

def oasis_symbol(ax, x, y, radius=3.5, zorder=10):
    """Draw a stylized oasis."""
    # Water pool
    water = plt.Circle((x, y), radius * 0.6, color=water_mid, alpha=0.8, zorder=zorder)
    ax.add_patch(water)
    # Inner shimmer
    ax.add_patch(plt.Circle((x, y), radius * 0.35, color=water_light, alpha=0.5, zorder=zorder + 1))
    # Palm trees around edge
    for angle in np.linspace(0, 2 * math.pi, 6)[:-1]:
        px = x + radius * 0.8 * math.cos(angle)
        py = y + radius * 0.8 * math.sin(angle)
        ax.plot([px, px + math.cos(angle) * 0.3],
                [py, py + 2.0], color='#5C3D2E', linewidth=1.2, zorder=zorder + 1)
        ax.add_patch(plt.Circle((px + math.cos(angle) * 0.3, py + 2.0),
                                 1.0, color=forest_mid, alpha=0.85, zorder=zorder + 2))

def ruin_symbol(ax, x, y, size=1.2, color=mountain_light, zorder=11):
    """Draw a broken tower / ruin symbol."""
    # Broken column
    ax.add_patch(mpatches.Rectangle((x - size * 0.4, y),
                                     size * 0.8, size * 2,
                                     color=color, alpha=0.7, zorder=zorder))
    # Rubble
    for rx, ry in [(x - size, y - size * 0.5), (x + size * 0.5, y - size * 0.3),
                   (x - size * 0.2, y - size * 0.7)]:
        ax.plot(rx, ry, 's', color=color, markersize=3, alpha=0.6, zorder=zorder)

def skull_symbol(ax, x, y, size=1.0, color=ink_color, zorder=12):
    """Draw a simple skull for death-zones."""
    ax.plot(x, y, 'o', color=color, markersize=size * 5, alpha=0.6, zorder=zorder)
    ax.text(x, y, '\u2620', color=color, fontsize=size * 8, ha='center', va='center',
            alpha=0.85, zorder=zorder + 1)

def void_symbol(ax, x, y, size=2.0, zorder=13):
    """Draw a void/dark citadel symbol."""
    # Dark star
    for angle in np.linspace(0, 2 * math.pi, 9)[:-1]:
        x2 = x + size * math.cos(angle)
        y2 = y + size * math.sin(angle)
        ax.plot([x, x2], [y, y2], color=void_color, linewidth=1.5, alpha=0.9, zorder=zorder)
    ax.plot(x, y, 'o', color=void_light, markersize=size * 3, alpha=0.8, zorder=zorder + 1)
    ax.plot(x, y, 'o', color='#6A2A8A', markersize=size * 1.5, alpha=0.9, zorder=zorder + 2)

def draw_compass_rose(ax, cx, cy, size=8):
    """Draw an ornate compass rose."""
    # Outer ring
    ax.add_patch(plt.Circle((cx, cy), size * 1.3, color=parchment_mid,
                              edgecolor=ink_color, linewidth=1.5, alpha=0.9, zorder=30))
    ax.add_patch(plt.Circle((cx, cy), size * 1.1, color=None, fill=False,
                              edgecolor=gold_color, linewidth=0.8, alpha=0.7, zorder=31))
    # Cardinal points
    directions = [('N', 0), ('S', 180), ('E', 90), ('W', 270)]
    for label, deg in directions:
        rad = math.radians(deg)
        tip_x = cx + size * math.sin(rad)
        tip_y = cy + size * math.cos(rad)
        mid_x = cx + size * 0.55 * math.sin(rad)
        mid_y = cy + size * 0.55 * math.cos(rad)
        perp_x = size * 0.25 * math.cos(rad)
        perp_y = -size * 0.25 * math.sin(rad)
        # Arrow arm (gold for N/S, ink for E/W)
        col = gold_color if label in ('N', 'S') else ink_faded
        ax.add_patch(plt.Polygon(
            [[tip_x, tip_y], [mid_x + perp_x, mid_y + perp_y],
             [mid_x - perp_x, mid_y - perp_y]],
            color=col, alpha=0.95, zorder=32
        ))
        # Label
        label_x = cx + (size * 1.55) * math.sin(rad)
        label_y = cy + (size * 1.55) * math.cos(rad)
        ax.text(label_x, label_y, label, fontsize=8, fontweight='bold',
                color=ink_color, ha='center', va='center',
                fontfamily='serif', zorder=33)
    # Ordinal points (smaller)
    for deg in [45, 135, 225, 315]:
        rad = math.radians(deg)
        tip_x = cx + size * 0.65 * math.sin(rad)
        tip_y = cy + size * 0.65 * math.cos(rad)
        mid_x = cx + size * 0.35 * math.sin(rad)
        mid_y = cy + size * 0.35 * math.cos(rad)
        perp_x = size * 0.15 * math.cos(rad)
        perp_y = -size * 0.15 * math.sin(rad)
        ax.add_patch(plt.Polygon(
            [[tip_x, tip_y], [mid_x + perp_x, mid_y + perp_y],
             [mid_x - perp_x, mid_y - perp_y]],
            color=ink_faded, alpha=0.8, zorder=32
        ))
    # Center jewel
    ax.add_patch(plt.Circle((cx, cy), size * 0.12, color=gold_color, alpha=0.95, zorder=34))
    ax.add_patch(plt.Circle((cx, cy), size * 0.07, color='#FF4400', alpha=0.9, zorder=35))

def draw_border(ax):
    """Draw an ornate decorative border."""
    margin = 4
    # Outer double border
    for lw, offset, col in [(3.0, 0, ink_color), (1.0, 2, gold_color)]:
        rect = mpatches.FancyBboxPatch(
            (margin + offset, margin + offset),
            240 - 2 * (margin + offset), 180 - 2 * (margin + offset),
            boxstyle='square,pad=0', linewidth=lw,
            edgecolor=col, facecolor='none', zorder=50
        )
        ax.add_patch(rect)
    # Corner ornaments
    for cx_corner, cy_corner in [(8, 8), (232, 8), (8, 172), (232, 172)]:
        for r, alpha in [(5, 0.15), (3.5, 0.25), (2, 0.4)]:
            ax.add_patch(plt.Circle((cx_corner, cy_corner), r,
                                     color=gold_color, alpha=alpha, zorder=51))
        ax.plot(cx_corner, cy_corner, '*', color=gold_color, markersize=8,
                alpha=0.85, zorder=52)
    # Decorative dots along border
    for x in np.linspace(15, 225, 30):
        for y in [6.5, 173.5]:
            ax.plot(x, y, '.', color=gold_color, markersize=1.5, alpha=0.5, zorder=51)
    for y in np.linspace(15, 165, 22):
        for x in [6.5, 233.5]:
            ax.plot(x, y, '.', color=gold_color, markersize=1.5, alpha=0.5, zorder=51)

# ══════════════════════════════════════════════════════════════════════════════
#  WORLD GEOGRAPHY — PAINTING LAYERS (back to front)
# ══════════════════════════════════════════════════════════════════════════════

# ─── OCEAN / SEA (Eastern coast) ─────────────────────────────────────────────
sea_verts = [
    (195, 0), (240, 0), (240, 180), (195, 180),
    (192, 160), (188, 140), (191, 120), (185, 100),
    (190, 80),  (188, 60),  (192, 40),  (188, 20),
    (195, 0)
]
draw_water_body(ax, sea_verts, color=water_dark, alpha=0.55, zorder=3)
# Sea shimmer lines
for y_wave in range(10, 175, 6):
    wavy_line(ax, 197, y_wave, 236, y_wave + 1,
              amplitude=0.3, freq=12, color=water_light,
              lw=0.4, alpha=0.25, zorder=4)

# ─── NORTHERN SEA / DELTA ────────────────────────────────────────────────────
north_sea_verts = [
    (70, 165), (90, 168), (110, 172), (130, 170), (150, 165),
    (165, 160), (175, 163), (190, 162), (195, 165),
    (195, 180), (70, 180), (70, 165)
]
draw_water_body(ax, north_sea_verts, color=water_mid, alpha=0.55, zorder=3)

# Northern sea shimmer
for y_wave in range(167, 178, 3):
    wavy_line(ax, 72, y_wave, 190, y_wave,
              amplitude=0.4, freq=10, color=water_light,
              lw=0.5, alpha=0.3, zorder=4)

# ─── KOWLOON DELTA RIVER FINGERS ────────────────────────────────────────────
for dx, spread in [(-8, 1.0), (0, 1.2), (8, 0.8), (14, 0.7)]:
    wavy_line(ax, 115 + dx, 145, 115 + dx * spread, 168,
              amplitude=0.8, freq=5, color=water_mid,
              lw=2.5, alpha=0.5, zorder=6)

# ─── MAIN RIVER — from Cinderteeth to delta ───────────────────────────────────
river_points = [
    (148, 125), (140, 133), (132, 140), (123, 148), (118, 158), (115, 168)
]
xs = [p[0] for p in river_points]
ys = [p[1] for p in river_points]
wavy_line(ax, xs[0], ys[0], xs[-1], ys[-1],
          amplitude=1.5, freq=4, color=water_mid, lw=3.5, alpha=0.55, zorder=6)

# Secondary river — west branch
wavy_line(ax, 60, 115, 75, 130, amplitude=0.8, freq=5,
          color=water_mid, lw=2.0, alpha=0.45, zorder=6)
wavy_line(ax, 75, 130, 100, 148, amplitude=1.0, freq=4,
          color=water_mid, lw=2.5, alpha=0.45, zorder=6)

# ─── DEEPWELL AQUIFER HINT (underground lines in desert) ─────────────────────
for dy in [-2, 0, 2]:
    ax.plot([130, 175], [88 + dy, 88 + dy],
            color=water_dark, linewidth=0.5, alpha=0.2,
            linestyle=':', zorder=5)

# ─── SALT FLATS — Saltglass Reach ────────────────────────────────────────────
salt_verts = [
    (168, 60), (185, 58), (193, 65), (188, 78), (175, 82), (163, 76), (168, 60)
]
draw_water_body(ax, salt_verts, color=salt_color, alpha=0.65, zorder=5)
# Salt crystal sparkles
for _ in range(25):
    sx = rng.uniform(169, 187)
    sy = rng.uniform(62, 79)
    ax.plot(sx, sy, '+', color='white', markersize=2.5, alpha=0.8, zorder=6)
    ax.plot(sx, sy, '+', color='#90A0A8', markersize=2.5, alpha=0.5, zorder=6,
            transform=ax.transData.__class__(fig.transFigure, ax.transData))

# ─── SCORCHED WASTES ─────────────────────────────────────────────────────────
scorch_verts = [
    (105, 68), (118, 65), (128, 68), (130, 78), (122, 85),
    (110, 84), (100, 78), (105, 68)
]
draw_water_body(ax, scorch_verts, color='#A05030', alpha=0.45, zorder=5)
# Ash texture
for _ in range(40):
    sx = rng.uniform(103, 129)
    sy = rng.uniform(67, 83)
    ax.plot(sx, sy, '.', color='#404040', markersize=1.5, alpha=0.4, zorder=6)

# ─── GREAT DESERT EXPANSE ─────────────────────────────────────────────────────
desert_verts = [
    (120, 45), (135, 40), (155, 42), (175, 48), (188, 60),
    (186, 80), (175, 85), (158, 88), (140, 85), (128, 78),
    (118, 70), (115, 58), (120, 45)
]
draw_water_body(ax, desert_verts, color=desert_light, alpha=0.6, zorder=4)

# Desert dune ripples
for i in range(15):
    dy = rng.uniform(45, 83)
    dx_start = rng.uniform(122, 140)
    dx_end   = rng.uniform(155, 178)
    wavy_line(ax, dx_start, dy, dx_end, dy + rng.uniform(-1, 1),
              amplitude=0.3, freq=15, color=desert_dark,
              lw=0.4, alpha=0.3, zorder=5)

# ─── SCORCHING SANDS ─────────────────────────────────────────────────────────
scorch_sand_verts = [
    (148, 40), (165, 35), (180, 38), (186, 50), (175, 52),
    (158, 48), (148, 40)
]
draw_water_body(ax, scorch_sand_verts, color='#C07830', alpha=0.5, zorder=5)

# ─── GREAT NORTHERN FOREST ───────────────────────────────────────────────────
forest_verts = [
    (30, 120), (50, 118), (70, 120), (90, 122), (110, 120),
    (130, 118), (155, 122), (165, 128), (160, 148), (148, 155),
    (130, 158), (110, 155), (90, 158), (70, 155), (50, 150),
    (32, 145), (18, 138), (20, 125), (30, 120)
]
draw_water_body(ax, forest_verts, color=forest_dark, alpha=0.35, zorder=4)

# ─── THORNWOOD ───────────────────────────────────────────────────────────────
thorn_verts = [
    (68, 68), (80, 65), (88, 70), (85, 82), (75, 86), (65, 80), (68, 68)
]
draw_water_body(ax, thorn_verts, color='#2A3A18', alpha=0.5, zorder=5)

# ─── WITHERED DEPTHS blighted zone ──────────────────────────────────────────
wither_verts = [
    (105, 128), (118, 124), (128, 128), (126, 140), (115, 144),
    (104, 138), (105, 128)
]
draw_water_body(ax, wither_verts, color='#3A3020', alpha=0.5, zorder=5)

# ─── CATACOMBS / GRAVELANDS (dark underground zones) ─────────────────────────
grave_verts = [
    (62, 38), (80, 35), (95, 38), (98, 52), (88, 58),
    (72, 60), (58, 54), (62, 38)
]
draw_water_body(ax, grave_verts, color='#28201A', alpha=0.45, zorder=5)

# ─── CINDERTEETH MOUNTAINS ───────────────────────────────────────────────────
# Volcanic glow halo
ax.add_patch(plt.Circle((152, 128), 18, color='#602010', alpha=0.12, zorder=6))
ax.add_patch(plt.Circle((152, 128), 12, color='#802010', alpha=0.15, zorder=6))

# ══════════════════════════════════════════════════════════════════════════════
#  TERRAIN FEATURES
# ══════════════════════════════════════════════════════════════════════════════

# ─── CINDERTEETH MOUNTAINS ───────────────────────────────────────────────────
cinderteeth_peaks = [
    (140, 125), (145, 128), (150, 122), (155, 130), (160, 125),
    (148, 135), (143, 132), (158, 132), (153, 118), (162, 118),
    (136, 120), (168, 125)
]
draw_mountain_range(ax, cinderteeth_peaks, scale=1.4,
                    color='#5A3020', shadow='#3A1810', zorder=10)
# Lava/fire at peak
for peak in [(150, 122), (155, 130)]:
    ax.plot(peak[0], peak[1] + 9, '^', color='#FF6600', markersize=5,
            alpha=0.8, zorder=12)
    ax.plot(peak[0], peak[1] + 9, '^', color='#FF3300', markersize=3,
            alpha=0.9, zorder=13)
    # Smoke plume
    for puff in range(5):
        py = peak[1] + 10 + puff * 2.5
        px = peak[0] + rng.uniform(-1, 1)
        r = 0.5 + puff * 0.4
        ax.add_patch(plt.Circle((px, py), r, color='#808080', alpha=0.15 - puff * 0.02, zorder=11))

# ─── ISETH REACH BORDER MOUNTAINS (West) ─────────────────────────────────────
west_mts = [(12, 80), (14, 88), (16, 96), (13, 104), (15, 110), (12, 118)]
draw_mountain_range(ax, west_mts, scale=0.9, color=mountain_mid, shadow=mountain_dark, zorder=10)

# ─── SOUTHERN RIDGES ─────────────────────────────────────────────────────────
south_mts = [(55, 22), (60, 18), (68, 22), (74, 18), (82, 22), (90, 18), (98, 22)]
draw_mountain_range(ax, south_mts, scale=0.85, color=mountain_light, shadow=mountain_mid, zorder=10)

# ─── GREAT NORTHERN FOREST — tree symbols ─────────────────────────────────────
forest_clusters = [
    (40, 132, 12, 80),   (58, 138, 10, 60),   (75, 135, 11, 70),
    (92, 130, 10, 55),   (108, 133, 9, 50),   (125, 130, 10, 55),
    (142, 135, 9, 50),   (30, 140, 8, 40),    (50, 145, 9, 45),
    (68, 142, 10, 55),   (88, 145, 9, 50),    (108, 145, 8, 40),
    (128, 142, 9, 45),   (148, 140, 8, 40),
]
for (cx, cy, rad, dens) in forest_clusters:
    draw_forest(ax, cx, cy, rad, density=dens,
                colors=[forest_dark, forest_mid, forest_light], zorder=8)

# Withered Depths — dead tree zone
for _ in range(25):
    wx = rng.uniform(107, 126)
    wy = rng.uniform(126, 142)
    ax.plot([wx, wx], [wy, wy + rng.uniform(1.5, 3.5)],
            color='#4A3A28', linewidth=0.8, alpha=0.7, zorder=9)
    # Bare branches
    for _ in range(3):
        ang = rng.uniform(0.3, 2.8)
        blen = rng.uniform(0.5, 1.5)
        ax.plot([wx, wx + math.cos(ang) * blen],
                [wy + 3, wy + 3 + math.sin(ang) * blen],
                color='#4A3A28', linewidth=0.5, alpha=0.6, zorder=9)

# Thornwood — gnarled trees
for _ in range(30):
    wx = rng.uniform(69, 87)
    wy = rng.uniform(67, 84)
    ax.plot([wx, wx + rng.uniform(-0.5, 0.5)],
            [wy, wy + rng.uniform(1.8, 3.5)],
            color='#2A1A0A', linewidth=1.0, alpha=0.75, zorder=9)
    ax.add_patch(plt.Circle((wx, wy + 3.5), rng.uniform(0.8, 1.5),
                             color='#2A3A10', alpha=0.7, zorder=10))

# ─── PYRAMIDS ─────────────────────────────────────────────────────────────────
pyramid_symbol(ax, 173, 60, size=5.0, color='#C8A040', zorder=12)   # Great Pyramid
pyramid_symbol(ax, 165, 75, size=4.0, color='#B89030', zorder=12)   # Northern Pyramid
pyramid_symbol(ax, 170, 44, size=4.0, color='#A08028', zorder=12)   # Southern Pyramid

# Black Sun Shard corona at Great Pyramid
for r in [6.5, 7.5, 8.5]:
    ax.add_patch(plt.Circle((173, 68), r, fill=False,
                             edgecolor='#CC2200', linewidth=0.4, alpha=0.3, zorder=13))

# ─── OASES ────────────────────────────────────────────────────────────────────
oasis_symbol(ax, 138, 78, radius=5.0, zorder=10)   # Great Oasis
oasis_symbol(ax, 138, 90, radius=3.5, zorder=10)   # Northern Oasis
oasis_symbol(ax, 132, 66, radius=3.5, zorder=10)   # Southern Oasis

# ─── KHEPRA-LESH (Lost City) — ruined ─────────────────────────────────────────
ruin_symbol(ax, 158, 65, size=1.8, color='#8A7A5A', zorder=11)
ruin_symbol(ax, 161, 67, size=1.2, color='#7A6A4A', zorder=11)
ruin_symbol(ax, 156, 68, size=1.5, color='#8A7A5A', zorder=11)

# ─── VOID / CRYPT COMPLEX (south) ─────────────────────────────────────────────
void_symbol(ax, 80, 42, size=2.5, zorder=13)   # Void Citadel
skull_symbol(ax, 73, 50, size=1.0, color='#3A1A1A', zorder=13)   # Kel'Shadra Crypts
skull_symbol(ax, 68, 56, size=0.8, color='#4A2A1A', zorder=13)   # Gloamvault

# Catacombs crack lines
for angle in np.linspace(0, 2 * math.pi, 8)[:-1]:
    ex = 80 + 5 * math.cos(angle)
    ey = 42 + 5 * math.sin(angle)
    ax.plot([80, ex], [42, ey], color=void_color, linewidth=0.6, alpha=0.4, zorder=12)

# ══════════════════════════════════════════════════════════════════════════════
#  CITIES & SETTLEMENTS
# ══════════════════════════════════════════════════════════════════════════════

# ─── MIDGAARD (capital) ───────────────────────────────────────────────────────
city_symbol(ax, 65, 95, size=2.2, color='#3A2510', crown=True, zorder=15)

# ─── KIESS ────────────────────────────────────────────────────────────────────
city_symbol(ax, 25, 90, size=1.8, color='#2A1A0A', zorder=15)

# ─── KOWLOON (delta city) ─────────────────────────────────────────────────────
city_symbol(ax, 115, 155, size=1.9, color='#1A2A3A', zorder=15)
# Neon Covenant glow
ax.add_patch(plt.Circle((115, 155), 5, fill=False,
                         edgecolor=neon_cyan, linewidth=1.0, alpha=0.4, zorder=14))

# ─── RAKUEN ───────────────────────────────────────────────────────────────────
city_symbol(ax, 60, 78, size=1.5, color='#2A1A0A', zorder=15)

# ─── MAFDET (coastal port) ────────────────────────────────────────────────────
city_symbol(ax, 187, 75, size=1.6, color='#1A2030', zorder=15)
# Harbor docks
for i in range(4):
    ax.plot([188 + i * 1.5, 188 + i * 1.5], [73, 70],
            color=ink_color, linewidth=0.8, alpha=0.6, zorder=14)

# ─── SHRINE OF THE FIRST CLAW ─────────────────────────────────────────────────
ax.plot(185, 90, 'D', color=gold_color, markersize=5, alpha=0.85, zorder=15)
ax.plot(185, 90, 'D', color='none', markersize=7,
        markeredgecolor=ink_color, markeredgewidth=0.8, zorder=15)

# ══════════════════════════════════════════════════════════════════════════════
#  ROADS & TRADE ROUTES
# ══════════════════════════════════════════════════════════════════════════════

road_color   = '#6B4A28'
lantern_col  = '#8B6914'
desert_road  = '#A07830'

# Roc Road: Kiess → Midgaard
curvy_road(ax, [(26, 91), (35, 92), (46, 93), (55, 94), (63, 95)],
           color=road_color, lw=1.8, style='-', zorder=9, alpha=0.65)
# Roc Road waypost symbols
for wp in [(35, 92.5), (46, 93.5)]:
    ax.plot(wp[0], wp[1], '|', color=road_color, markersize=4, alpha=0.7, zorder=10)

# Lantern Road: Midgaard → Kowloon (through forest)
lantern_pts = [(66, 98), (70, 105), (72, 115), (78, 125), (88, 135),
               (98, 143), (107, 150), (114, 154)]
curvy_road(ax, lantern_pts, color=lantern_col, lw=1.6, style='-', zorder=9, alpha=0.6)
# Lantern posts along the road
for i in range(1, len(lantern_pts) - 1, 2):
    lx, ly = lantern_pts[i]
    ax.plot(lx, ly, 'o', color=gold_light, markersize=3, alpha=0.7, zorder=10)
    ax.add_patch(plt.Circle((lx, ly), 1.5, fill=False,
                              edgecolor=gold_light, linewidth=0.4, alpha=0.3, zorder=10))

# Midgaard → Rakuen road
curvy_road(ax, [(64, 93), (63, 88), (62, 83), (61, 79)],
           color=road_color, lw=1.4, style='-', zorder=9, alpha=0.6)

# Eastern Desert Caravan Road
caravan_pts = [(138, 80), (148, 78), (158, 72), (165, 67), (172, 62)]
curvy_road(ax, caravan_pts, color=desert_road, lw=1.5, style='--', zorder=9, alpha=0.65)
# Caravan camel markers
ax.text(153, 75, '~', color=desert_dark, fontsize=7, ha='center', va='center', alpha=0.7, zorder=10, style='italic')

# Caravan road to Mafdet (coast)
curvy_road(ax, [(172, 62), (178, 66), (183, 70), (186, 74)],
           color=desert_road, lw=1.3, style='--', zorder=9, alpha=0.6)

# Road: Midgaard → Catacombs
curvy_road(ax, [(64, 93), (68, 82), (72, 72), (76, 62), (78, 52), (79, 45)],
           color='#4A2A1A', lw=1.2, style=':', zorder=9, alpha=0.55)

# ══════════════════════════════════════════════════════════════════════════════
#  DECORATIVE MAP FEATURES
# ══════════════════════════════════════════════════════════════════════════════

# ─── SEA MONSTERS ─────────────────────────────────────────────────────────────
# Serpent coils in the eastern sea
for i, (sx, sy) in enumerate([(210, 50), (222, 90), (215, 130)]):
    scale = 1.0 + i * 0.2
    for coil_offset in range(4):
        angle = coil_offset * math.pi / 2
        cx2 = sx + scale * 3 * math.cos(angle)
        cy2 = sy + scale * 3 * math.sin(angle)
        ax.add_patch(plt.Circle((cx2, cy2), scale * 1.8,
                                 color='#0A3A5A', alpha=0.25, zorder=7))
    ax.plot(sx + scale * 2, sy + scale * 2, '^',
            color='#0A4A6A', markersize=int(6 * scale), alpha=0.5, zorder=8)
    ax.text(sx, sy - scale * 5, 'HERE BE\nDRAGONS',
            fontsize=4.5, color=water_dark, ha='center', style='italic',
            fontfamily='serif', alpha=0.55, zorder=8)

# ─── WIND HEADS ───────────────────────────────────────────────────────────────
for wx, wy, direction in [(18, 160, 0), (230, 155, 180)]:
    ax.add_patch(plt.Circle((wx, wy), 6, color=parchment_mid,
                             edgecolor=ink_faded, linewidth=0.8, alpha=0.7, zorder=20))
    # Cheeks
    ax.add_patch(plt.Circle((wx - 2, wy), 2.5, color='#E8C090', alpha=0.6, zorder=21))
    ax.add_patch(plt.Circle((wx + 2, wy), 2.5, color='#E8C090', alpha=0.6, zorder=21))
    # Wind lines
    for wl in range(5):
        wl_len = rng.uniform(6, 14)
        angle  = math.radians(-20 + wl * 10 + (90 if direction == 0 else -90))
        wx2 = wx + wl_len * math.cos(angle)
        wy2 = wy + wl_len * math.sin(angle) * 0.3
        wavy_line(ax, wx + 5 if direction == 180 else wx - 5, wy + wl - 2,
                  wx2, wy2, amplitude=0.4, freq=8,
                  color=water_light, lw=0.7, alpha=0.4, zorder=20)

# ─── SAILING SHIP in northern sea ──────────────────────────────────────────────
ship_x, ship_y = 145, 173
ax.plot([ship_x - 4, ship_x + 4], [ship_y, ship_y],
        color=ink_color, linewidth=1.5, alpha=0.55, zorder=15)   # hull
ax.plot([ship_x, ship_x], [ship_y, ship_y + 6],
        color=ink_color, linewidth=1.0, alpha=0.55, zorder=15)   # mast
ax.add_patch(plt.Polygon(
    [[ship_x, ship_y + 6], [ship_x - 4, ship_y + 2], [ship_x, ship_y + 2]],
    color='#D4B070', alpha=0.55, zorder=16))                     # sail

# ─── DECORATIVE MEDALLION around Kowloon (Neon Covenant) ─────────────────────
for r in [7, 9]:
    ax.add_patch(plt.Circle((115, 155), r, fill=False,
                             edgecolor=neon_cyan, linewidth=0.5, alpha=0.2,
                             linestyle='--', zorder=14))

# ─── BLACK SUN SHARD OMINOUS RAYS ─────────────────────────────────────────────
for angle in np.linspace(0, 2 * math.pi, 16)[:-1]:
    ray_len = rng.uniform(8, 18)
    rx = 173 + ray_len * math.cos(angle)
    ry = 68 + ray_len * math.sin(angle)
    ax.plot([173, rx], [68, ry],
            color='#CC0000', linewidth=0.5, alpha=0.2, zorder=11)

# ─── VOID CITADEL DARK RAYS ────────────────────────────────────────────────────
for angle in np.linspace(0, 2 * math.pi, 12)[:-1]:
    rx = 80 + 7 * math.cos(angle)
    ry = 42 + 7 * math.sin(angle)
    ax.plot([80, rx], [42, ry],
            color=void_light, linewidth=0.4, alpha=0.25, zorder=12)

# ─── DEEPWELL AQUIFER MARKER ──────────────────────────────────────────────────
ax.plot(138, 78, 'v', color=water_dark, markersize=8, alpha=0.7, zorder=13)
ax.add_patch(plt.Circle((138, 78), 6, fill=False,
                         edgecolor=water_dark, linewidth=0.8, alpha=0.4, zorder=12))

# ══════════════════════════════════════════════════════════════════════════════
#  LABELS
# ══════════════════════════════════════════════════════════════════════════════

# ─── Region / Area labels ─────────────────────────────────────────────────────
labels = [
    # (x, y, text, size, color, style, weight, rotation)
    (115, 165, 'KOWLOON', 10, ink_color, 'normal', 'bold', 0),
    (65, 99, 'MIDGAARD', 11, ink_color, 'normal', 'bold', 0),
    (25, 94, 'KIESS', 9, ink_color, 'normal', 'bold', 0),
    (60, 82, 'RAKUEN', 8.5, ink_color, 'italic', 'normal', 0),
    (187, 79, 'MAFDET', 8.5, '#1A2030', 'normal', 'bold', 0),
    (138, 83, 'GREAT\nOASIS', 7.5, '#1A3A5A', 'italic', 'bold', 0),
    (140, 95, 'NORTHERN\nOASIS', 6.5, '#1A3A5A', 'italic', 'normal', 0),
    (130, 70, 'SOUTHERN\nOASIS', 6.5, '#1A3A5A', 'italic', 'normal', 0),
    (173, 55, 'GREAT\nPYRAMID', 7.0, '#7A5010', 'normal', 'bold', 0),
    (163, 80, 'NORTHERN\nPYRAMID', 6.0, '#7A5010', 'italic', 'normal', 0),
    (168, 38, 'SOUTHERN\nPYRAMID', 6.0, '#7A5010', 'italic', 'normal', 0),
    (152, 138, 'CINDERTEETH\nMOUNTAINS', 7.5, '#5A2010', 'normal', 'bold', 0),
    (85, 140, 'THE GREAT\nNORTHERN FOREST', 8.5, forest_dark, 'italic', 'bold', 0),
    (148, 60, 'EASTERN\nDESERT', 7.5, desert_dark, 'italic', 'bold', 0),
    (113, 74, 'SCORCHED\nWASTES', 6.5, '#6A2808', 'italic', 'normal', 0),
    (165, 40, 'SCORCHING\nSANDS', 6.0, '#904010', 'italic', 'normal', 0),
    (178, 68, 'SALTGLASS\nREACH', 6.5, '#6A6050', 'italic', 'normal', -15),
    (157, 65, 'KHEPRA-LESH\n(Ruined)', 5.5, '#7A6A4A', 'italic', 'normal', 0),
    (80, 48, 'VOID CITADEL', 6.0, void_light, 'normal', 'bold', 0),
    (72, 56, "KEL'SHADRA\nCRYPTS", 6.0, '#5A2A3A', 'italic', 'bold', 0),
    (67, 64, 'GLOAMVAULT', 5.5, '#4A2A3A', 'italic', 'normal', 0),
    (76, 75, 'THORNWOOD', 6.0, '#2A3A18', 'italic', 'bold', 0),
    (75, 42, 'SEPULCHUR\nPASTURE', 6.0, '#3A2A1A', 'italic', 'normal', 0),
    (110, 130, 'WITHERED\nDEPTHS', 6.0, '#4A3A28', 'italic', 'normal', 0),
    (35, 130, 'VERDANT\nDEPTHS', 6.0, forest_dark, 'italic', 'normal', 0),
    (22, 120, 'FOREST OF\nCONFUSION', 5.5, forest_mid, 'italic', 'normal', 0),
    (185, 92, 'SHRINE OF\nTHE FIRST CLAW', 5.0, '#7A5010', 'italic', 'normal', 0),
    (45, 93, 'ROC ROAD', 5.5, road_color, 'italic', 'normal', 5),
    (90, 120, 'LANTERN ROAD', 5.5, lantern_col, 'italic', 'normal', 55),
    (155, 75, 'CARAVAN ROAD', 5.0, desert_road, 'italic', 'normal', -20),
    (220, 90, 'THE GLEAMING\nEASTERN SEA', 7.5, '#3A6A8A', 'italic', 'bold', -90),
    (135, 175, 'NORTHERN REACH', 7.0, water_dark, 'italic', 'bold', 0),
    (12, 60, 'ISETH\nWILDS', 6.0, mountain_mid, 'italic', 'normal', 0),
    (75, 18, 'SUNKEN REACHES', 6.0, mountain_light, 'italic', 'normal', 0),
]

for (x, y, text, size, color, style, weight, rotation) in labels:
    place_label(ax, x, y, text, fontsize=size, color=color,
                style=style, weight=weight, rotation=rotation, zorder=22)

# ─── FACTION CRESTS / SEALS ───────────────────────────────────────────────────
# Jackal Synod seal in Eastern Desert
ax.text(148, 58, '\u29D6', color=desert_dark, fontsize=10, ha='center', va='center', alpha=0.4, zorder=11)
ax.text(148, 53, 'JACKAL SYNOD', fontsize=4.5, color=desert_dark,
        ha='center', style='italic', fontfamily='serif', alpha=0.6, zorder=12)

# Spring Wardens at Great Oasis
ax.text(145, 82, '\u26C6', color=water_dark, fontsize=7, ha='center', va='center', alpha=0.5, zorder=11)
ax.text(145, 80, 'Spring Wardens', fontsize=4.0, color=water_dark,
        ha='center', style='italic', fontfamily='serif', alpha=0.55, zorder=12)

# Bell Synod mark near Catacombs
ax.text(70, 40, '\u237E', color=ink_faded, fontsize=7, ha='center', va='center', alpha=0.5, zorder=11)
ax.text(70, 37.5, 'BELL SYNOD', fontsize=4.0, color=ink_faded,
        ha='center', style='italic', fontfamily='serif', alpha=0.55, zorder=12)

# ══════════════════════════════════════════════════════════════════════════════
#  TITLE CARTOUCHE
# ══════════════════════════════════════════════════════════════════════════════
# Ornate scroll cartouche at the top-left
cartouche_x, cartouche_y = 12, 152
cartouche_w, cartouche_h = 68, 22

ax.add_patch(FancyBboxPatch(
    (cartouche_x, cartouche_y), cartouche_w, cartouche_h,
    boxstyle='round,pad=1.5',
    facecolor='#F0D878', edgecolor=gold_color,
    linewidth=2.5, alpha=0.88, zorder=40
))
ax.add_patch(FancyBboxPatch(
    (cartouche_x + 1.5, cartouche_y + 1.5), cartouche_w - 3, cartouche_h - 3,
    boxstyle='round,pad=1',
    facecolor='none', edgecolor=ink_color,
    linewidth=0.8, alpha=0.6, zorder=41
))
# Corner flourishes on cartouche
for cx2, cy2 in [(cartouche_x + 3, cartouche_y + 3),
                  (cartouche_x + cartouche_w - 3, cartouche_y + 3),
                  (cartouche_x + 3, cartouche_y + cartouche_h - 3),
                  (cartouche_x + cartouche_w - 3, cartouche_y + cartouche_h - 3)]:
    ax.plot(cx2, cy2, '*', color=gold_color, markersize=6, alpha=0.8, zorder=42)

ax.text(cartouche_x + cartouche_w / 2, cartouche_y + cartouche_h - 5,
        'THE KNOWN WORLD',
        fontsize=13, fontweight='bold', color=ink_color,
        ha='center', va='center', fontfamily='serif', zorder=43,
        path_effects=[pe.withStroke(linewidth=2, foreground='#F0D878')])
ax.text(cartouche_x + cartouche_w / 2, cartouche_y + cartouche_h - 11,
        'of the  ACK!MUD  Realm',
        fontsize=8.5, color=ink_faded,
        ha='center', va='center', fontfamily='serif', style='italic', zorder=43)
ax.text(cartouche_x + cartouche_w / 2, cartouche_y + 5,
        '\u2728  Where Memory Holds, There Law Endures  \u2728',
        fontsize=5.5, color=gold_color,
        ha='center', va='center', fontfamily='serif', style='italic', zorder=43)

# ─── SCALE BAR ────────────────────────────────────────────────────────────────
scale_x, scale_y = 14, 20
ax.plot([scale_x, scale_x + 25], [scale_y, scale_y],
        color=ink_color, linewidth=2.0, alpha=0.8, zorder=40)
for tick_x in [scale_x, scale_x + 12.5, scale_x + 25]:
    ax.plot([tick_x, tick_x], [scale_y - 0.8, scale_y + 0.8],
            color=ink_color, linewidth=1.5, alpha=0.8, zorder=40)
ax.text(scale_x, scale_y - 2.5, '0', fontsize=5.5, color=ink_color,
        ha='center', fontfamily='serif', zorder=41)
ax.text(scale_x + 12.5, scale_y - 2.5, '50 leagues', fontsize=5.5, color=ink_color,
        ha='center', fontfamily='serif', zorder=41)
ax.text(scale_x + 25, scale_y - 2.5, '100', fontsize=5.5, color=ink_color,
        ha='center', fontfamily='serif', zorder=41)
ax.text(scale_x + 12.5, scale_y + 2.5, 'SCALE', fontsize=5, color=ink_faded,
        ha='center', style='italic', fontfamily='serif', zorder=41)

# ─── COMPASS ROSE ─────────────────────────────────────────────────────────────
draw_compass_rose(ax, cx=215, cy=28, size=14)

# ─── LEGEND ───────────────────────────────────────────────────────────────────
leg_x, leg_y = 12, 30
ax.add_patch(FancyBboxPatch((leg_x, leg_y), 42, 58,
             boxstyle='round,pad=1', facecolor='#F5E8B0',
             edgecolor=gold_color, linewidth=1.5, alpha=0.85, zorder=40))
ax.text(leg_x + 21, leg_y + 54, 'LEGEND', fontsize=8, fontweight='bold',
        color=ink_color, ha='center', fontfamily='serif', zorder=41)

legend_items = [
    ('\u25A0', '#3A2510', 'City / Settlement'),
    ('\u25B2', mountain_mid, 'Mountain Range'),
    ('\u25CF', water_mid, 'Water / Oasis'),
    ('\u25B2', '#C8A040', 'Sacred Pyramid'),
    ('\u2620', void_color, 'Death / Void Zone'),
    ('\u25A0', forest_dark, 'Ancient Forest'),
    ('\u25A0', '#A05030', 'Scorched Wastes'),
    ('\u25A0', salt_color, 'Salt Flats'),
    ('\u25C6', gold_color, 'Shrine / Relic'),
    ('---', desert_road, 'Caravan Road'),
    ('---', road_color, 'Trade Road'),
    ('---', lantern_col, 'Lantern Road'),
]
for i, (sym, col, label) in enumerate(legend_items):
    iy = leg_y + 48 - i * 4.3
    if sym == '---':
        ax.plot([leg_x + 2, leg_x + 8], [iy + 0.5, iy + 0.5],
                color=col, linewidth=1.5, linestyle='--' if col == desert_road else '-',
                alpha=0.8, zorder=41)
    else:
        ax.text(leg_x + 5, iy, sym, fontsize=7, color=col,
                ha='center', va='center', zorder=41)
    ax.text(leg_x + 11, iy, label, fontsize=5.5, color=ink_faded,
            va='center', fontfamily='serif', zorder=41)

# ─── LORE ANNOTATIONS (flavor text on map) ────────────────────────────────────
lore_texts = [
    (152, 114, '"Here the mountain bleeds fire\nand the sky remembers ash."', 4.5, '#5A2010'),
    (100, 165, '"Where five covenants hold\none wall, one law."', 4.5, '#1A2A3A'),
    (173, 72, '"The Black Sun consumes\nnames. Write in stone."', 4.5, '#8B0000'),
    (80, 30, '"The dead keep better records\nthan the living."', 4.5, '#3A1A2A'),
    (25, 102, '"Old Evermeet sings\nbeneath these foundations."', 4.5, '#3A2A1A'),
    (155, 95, '"Measured thirst survives.\nUnmeasured thirst is war."', 4.5, '#6A4010'),
]
for (lx, ly, text, size, col) in lore_texts:
    ax.add_patch(FancyBboxPatch((lx - 16, ly - 5), 32, 10,
                 boxstyle='round,pad=0.5', facecolor='#F5E0A0',
                 edgecolor=col, linewidth=0.5, alpha=0.55, zorder=19))
    ax.text(lx, ly, text, fontsize=size, color=col, ha='center', va='center',
            style='italic', fontfamily='serif', zorder=20)

# ─── ORNATE BORDER ────────────────────────────────────────────────────────────
draw_border(ax)

# ─── PUBLISHER FOOTER ────────────────────────────────────────────────────────
ax.text(120, 7.5, 'Cartographed by the Granite Arcade of Midgaard  \u2014  Third Age, Containment Era',
        fontsize=5.5, color=ink_faded, ha='center', style='italic',
        fontfamily='serif', alpha=0.8, zorder=50)

# ══════════════════════════════════════════════════════════════════════════════
#  SAVE
# ══════════════════════════════════════════════════════════════════════════════
output_path = '/home/user/acktng/docs/world_map.png'
plt.savefig(output_path, dpi=120, bbox_inches='tight',
            facecolor=parchment_base, edgecolor='none')
plt.close()
print(f"Map saved to {output_path}")
