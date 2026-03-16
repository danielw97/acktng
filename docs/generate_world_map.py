#!/usr/bin/env python3
"""
ACK!MUD TNG — Fantasy World Map Generator  (v2)
Lore-accurate, hand-drawn antique style.
"""

import numpy as np
import math, random
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.patheffects as pe
import matplotlib.colors as mcolors
from matplotlib.path import Path
from matplotlib.patches import FancyBboxPatch, PathPatch
from scipy.interpolate import splprep, splev
from PIL import Image, ImageDraw, ImageFilter, ImageChops
import io

RNG  = random.Random(7)
NRNG = np.random.RandomState(7)

# ═══════════════════════════════════════════════════════════
#  PALETTE — antique map ink-on-vellum
# ═══════════════════════════════════════════════════════════
INK        = '#2C1A0E'
INK_FADED  = '#5C3D20'
INK_LIGHT  = '#8B6040'
GOLD       = '#9B7310'
GOLD_LT    = '#C8A030'
PARCH_DARK = '#C8A060'
PARCH_MID  = '#DDB870'
PARCH_LT   = '#EDD080'
PARCH_PALE = '#F5E0A0'

SEA_DEEP   = '#1A3A62'
SEA_MID    = '#2A5580'
SEA_LT     = '#4A7898'
SEA_PALE   = '#6AAAB8'
RIVER_COL  = '#3A6E98'

FOREST_INK = '#0E2A0A'
FOREST_DARK= '#1A4A14'
FOREST_MID = '#2A6820'
FOREST_LT  = '#4A8830'

DESERT_BRN = '#8A6018'
DESERT_TAN = '#C49030'
DESERT_PALE= '#DEBA70'

MTN_DARK   = '#2E2018'
MTN_MID    = '#524030'
MTN_LT     = '#806850'
MTN_SNOW   = '#F0EDE4'

SCORCH_COL = '#7A2A10'
ASH_COL    = '#605848'
SALT_COL   = '#D0C8B0'
VOID_COL   = '#120622'
VOID_LT    = '#341448'
CRYPT_COL  = '#201018'
BLIGHT_COL = '#241C10'

BLOOD_RED  = '#8B1010'
FIRE_ORG   = '#C84010'
TEAL_NEON  = '#008080'
BLIGHT_PURPLE = '#6A3878'

W, H = 240, 180   # data-space dimensions

# ═══════════════════════════════════════════════════════════
#  PARCHMENT TEXTURE  (PIL → numpy → matplotlib imshow)
# ═══════════════════════════════════════════════════════════
def make_parchment(w_px=2880, h_px=2160):
    """Generate a multi-layer aged-parchment texture."""
    img = Image.new('RGB', (w_px, h_px))
    pix = img.load()

    # Base gradient: rich warm parchment, darker at edges
    for y in range(h_px):
        for x in range(w_px):
            fx, fy = x / w_px, y / h_px
            # Radial vignette factor (stronger edge darkening)
            vx = (fx - 0.5) * 2
            vy = (fy - 0.5) * 2
            vign = 1.0 - 0.55 * (vx*vx + vy*vy) ** 0.55
            # Diagonal warm gradient for depth
            diag = (fx * 0.3 + fy * 0.7)
            r = int((228 - 20 * diag + 8 * math.sin(fx*8)) * vign)
            g = int((195 - 28 * diag + 5 * math.cos(fy*6)) * vign)
            b = int((128 - 22 * diag) * vign)
            pix[x, y] = (max(0,min(255,r)), max(0,min(255,g)), max(0,min(255,b)))

    # Noise passes
    base = np.array(img).astype(float)
    for scale, amp in [(8, 18), (20, 12), (55, 8), (120, 5)]:
        nw, nh = w_px // scale, h_px // scale
        noise = NRNG.uniform(-1, 1, (nh, nw, 3))
        # Upsample
        noise_up = np.array(Image.fromarray(
            ((noise + 1) * 127).astype(np.uint8)
        ).resize((w_px, h_px), Image.BILINEAR)).astype(float) - 127
        base += noise_up * amp / 127

    base = np.clip(base, 0, 255).astype(np.uint8)
    img = Image.fromarray(base)

    # Stain blobs
    draw = ImageDraw.Draw(img)
    for _ in range(180):
        sx = RNG.randint(0, w_px)
        sy = RNG.randint(0, h_px)
        sr = RNG.randint(10, 120)
        alpha_factor = RNG.uniform(0.03, 0.2)
        stain_col = RNG.choice([
            (100, 70, 30), (80, 55, 20), (140, 100, 50),
            (60, 40, 10), (120, 90, 45)
        ])
        # Draw stain as an ellipse
        draw.ellipse([sx-sr, sy-sr//2, sx+sr, sy+sr//2],
                     fill=tuple(int(c * alpha_factor + p * (1-alpha_factor))
                                for c, p in zip(stain_col,
                                                img.getpixel((min(sx,w_px-1),min(sy,h_px-1))))))

    # Light blur for softness
    img = img.filter(ImageFilter.GaussianBlur(radius=1.5))
    return np.array(img) / 255.0

# ═══════════════════════════════════════════════════════════
#  SPLINE HELPERS
# ═══════════════════════════════════════════════════════════
def smooth_polygon(pts, closed=True, tension=0.0, n=400):
    """Return smoothed x,y arrays via scipy B-spline."""
    pts = np.array(pts, dtype=float)
    if closed:
        pts = np.vstack([pts, pts[:3]])
    x, y = pts[:,0], pts[:,1]
    # Remove consecutive duplicates
    mask = np.concatenate(([True], np.any(np.diff(pts, axis=0) != 0, axis=1)))
    pts  = pts[mask]
    x, y = pts[:,0], pts[:,1]
    k = min(3, len(x)-1)
    try:
        tck, _ = splprep([x, y], s=tension, k=k, per=False)
    except Exception:
        # Fall back to linear interpolation
        t_raw = np.linspace(0, 1, len(x))
        t = np.linspace(0, 1, n)
        xs = np.interp(t, t_raw, x)
        ys = np.interp(t, t_raw, y)
        return xs, ys
    t = np.linspace(0, 1, n)
    xs, ys = splev(t, tck)
    return xs, ys

def smooth_path_patch(pts, closed=True, tension=0.0, n=600, **kwargs):
    """Create a matplotlib PathPatch from a smoothed polygon."""
    xs, ys = smooth_polygon(pts, closed=closed, tension=tension, n=n)
    verts = list(zip(xs, ys))
    if closed:
        verts.append(verts[0])
    codes = [Path.MOVETO] + [Path.LINETO]*(len(verts)-2) + [Path.CLOSEPOLY]
    return PathPatch(Path(verts, codes), **kwargs)

def smooth_line(pts, tension=0.5, n=300):
    """Return xs, ys for a smooth open curve."""
    pts = np.array(pts, dtype=float)
    x, y = pts[:,0], pts[:,1]
    k = min(3, len(x)-1)
    tck, _ = splprep([x, y], s=tension, k=k)
    xs, ys = splev(np.linspace(0, 1, n), tck)
    return xs, ys

# ═══════════════════════════════════════════════════════════
#  DRAWING PRIMITIVES
# ═══════════════════════════════════════════════════════════
def fill_region(ax, pts, color, alpha=0.72, edge_color=None, edge_lw=0.8,
                zorder=5, tension=8):
    ec = edge_color if edge_color else color
    patch = smooth_path_patch(pts, tension=tension,
                               facecolor=color, edgecolor=ec,
                               linewidth=edge_lw, alpha=alpha, zorder=zorder)
    ax.add_patch(patch)

def draw_smooth_line(ax, pts, color, lw=1.2, alpha=0.8, zorder=9,
                     tension=0.5, style='-'):
    xs, ys = smooth_line(pts, tension=tension)
    ax.plot(xs, ys, color=color, linewidth=lw, alpha=alpha,
            zorder=zorder, linestyle=style, solid_capstyle='round')

def hatch_region(ax, pts, color, dot_count=120, size=0.8, alpha=0.25, zorder=6):
    """Scatter tiny dots inside a polygon for texture."""
    xs, ys = smooth_polygon(pts)
    from matplotlib.path import Path as MPath
    p = MPath(list(zip(xs, ys)))
    xmin, xmax = xs.min(), xs.max()
    ymin, ymax = ys.min(), ys.max()
    dots_x, dots_y = [], []
    attempts = dot_count * 8
    while len(dots_x) < dot_count and attempts > 0:
        tx = NRNG.uniform(xmin, xmax)
        ty = NRNG.uniform(ymin, ymax)
        if p.contains_point((tx, ty)):
            dots_x.append(tx)
            dots_y.append(ty)
        attempts -= 1
    ax.scatter(dots_x, dots_y, s=size, color=color, alpha=alpha, zorder=zorder)

# ═══════════════════════════════════════════════════════════
#  TERRAIN SYMBOLS
# ═══════════════════════════════════════════════════════════
def mountain(ax, cx, cy, h=5.0, w=4.5, snow_frac=0.28, zorder=11,
             col=MTN_MID, shadow=MTN_DARK, snow=MTN_SNOW):
    """Single hand-drawn mountain — faceted faces, shadow hatching, jagged snow."""
    peak  = (cx,       cy + h)
    left  = (cx - w/2, cy)
    right = (cx + w/2, cy)
    ridge = (cx + w * 0.10, cy)   # visible ridge slightly right of centre

    # Ground shadow ellipse
    ax.add_patch(mpatches.Ellipse((cx + w*0.12, cy), w*1.15, h*0.15,
                                   color=MTN_DARK, alpha=0.13, zorder=zorder-1))

    # Light face (left of ridge)
    ax.add_patch(plt.Polygon([peak, left, ridge], color=col,
                              alpha=0.93, zorder=zorder))
    # Shadow face (right of ridge)
    ax.add_patch(plt.Polygon([peak, ridge, right], color=shadow,
                              alpha=0.93, zorder=zorder))

    # Shadow-face hatching — parallel lines across the right triangle
    n_hatch = 9
    for i in range(n_hatch):
        t = (i + 0.5) / n_hatch
        ex = right[0] + (peak[0] - right[0]) * t
        ey = right[1] + (peak[1] - right[1]) * t
        rx = ridge[0] + (peak[0] - ridge[0]) * t
        ry = ridge[1] + (peak[1] - ridge[1]) * t
        ax.plot([rx, ex], [ry, ey], color=MTN_DARK,
                linewidth=0.32, alpha=0.26, zorder=zorder+1)

    # Light-face contour lines (faint horizontal striations)
    for frac in [0.32, 0.52, 0.68, 0.80]:
        py  = cy + h * frac
        lx2 = left[0]  + (peak[0] - left[0])  * frac
        rx2 = ridge[0] + (peak[0] - ridge[0]) * frac
        ax.plot([lx2, rx2], [py, py], color=MTN_DARK,
                linewidth=0.28, alpha=0.17, zorder=zorder+1)

    # Snow cap — split into light and shadow portions
    snow_y  = cy + h * (1 - snow_frac)
    t_sn    = snow_frac * 0.88
    sn_mid  = (cx + w*0.03, snow_y + h*0.01)
    sl      = (cx - w/2 * t_sn,        snow_y)
    sr      = (cx + w/2 * t_sn * 0.62, snow_y)
    ax.add_patch(plt.Polygon([peak, sl, sn_mid],
                              color=snow, alpha=0.90, zorder=zorder+2))
    snow_shd = tuple(c * 0.84 for c in mcolors.to_rgb(snow))
    ax.add_patch(plt.Polygon([peak, sn_mid, sr],
                              color=snow_shd, alpha=0.84, zorder=zorder+2))
    ax.add_patch(plt.Polygon([peak, sl, sr],
                              fill=False, edgecolor=MTN_DARK,
                              linewidth=0.38, alpha=0.38, zorder=zorder+3))

    # Main ink outline
    ax.add_patch(plt.Polygon([peak, left, right],
                              fill=False, edgecolor=MTN_DARK,
                              linewidth=1.0, alpha=0.82, zorder=zorder+3))
    # Ridge line
    ax.plot([cx, ridge[0]], [cy + h, cy],
            color=MTN_DARK, linewidth=0.55, alpha=0.52, zorder=zorder+2)

def mountain_range(ax, peaks, base_h=5.0, base_w=4.5, scatter=True,
                   zorder=11, **kw):
    for (px, py) in peaks:
        dh = RNG.uniform(0.75, 1.25) * base_h if scatter else base_h
        dw = RNG.uniform(0.80, 1.20) * base_w if scatter else base_w
        mountain(ax, px, py, h=dh, w=dw, zorder=zorder, **kw)

def tree(ax, cx, cy, h=2.5, col=FOREST_MID, zorder=9):
    """Single conifer — antique-map style with ink outline and light/shadow tiers."""
    trunk_h = h * 0.20
    trunk_w = h * 0.055
    # Ground shadow
    ax.add_patch(mpatches.Ellipse((cx, cy + 0.04), h*0.58, h*0.09,
                                   color='#1A0E06', alpha=0.14, zorder=zorder-1))
    # Tapered trunk
    ax.add_patch(plt.Polygon([
        [cx - trunk_w,       cy],
        [cx + trunk_w,       cy],
        [cx + trunk_w * 0.5, cy + trunk_h],
        [cx - trunk_w * 0.5, cy + trunk_h]],
        color='#4A2A10', alpha=0.90, zorder=zorder))

    tiers  = 4
    base_y = cy + trunk_h
    rgb    = mcolors.to_rgb(col)
    for i in range(tiers):
        frac   = 1.0 - i * 0.20
        tw     = h * 0.56 * frac
        tier_y = base_y + i * h * 0.215
        tip_y  = tier_y + h * 0.355 * frac

        # Light left face
        lt = tuple(min(1.0, c * 1.18) for c in rgb)
        # Dark right face
        dk = tuple(max(0.0, c * 0.66) for c in rgb)

        ax.add_patch(plt.Polygon(
            [[cx - tw, tier_y], [cx, tier_y], [cx, tip_y]],
            color=lt, alpha=0.94, zorder=zorder + i))
        ax.add_patch(plt.Polygon(
            [[cx, tier_y], [cx + tw, tier_y], [cx, tip_y]],
            color=dk, alpha=0.94, zorder=zorder + i))
        # Ink outline
        ax.add_patch(plt.Polygon(
            [[cx - tw, tier_y], [cx + tw, tier_y], [cx, tip_y]],
            fill=False, edgecolor=FOREST_INK,
            linewidth=0.58, alpha=0.72, zorder=zorder + i + 1))
        # Underline shadow at tier base
        ax.plot([cx - tw*0.88, cx + tw*0.88], [tier_y, tier_y],
                color=FOREST_INK, linewidth=0.28, alpha=0.24, zorder=zorder + i)

def forest_cluster(ax, cx, cy, radius, count=45, h_range=(1.8,3.2), zorder=9):
    for _ in range(count):
        angle = RNG.uniform(0, 2*math.pi)
        dist  = RNG.uniform(0, radius) ** 0.7 * radius
        tx = cx + dist * math.cos(angle)
        ty = cy + dist * math.sin(angle)
        h  = RNG.uniform(*h_range)
        col = RNG.choice([FOREST_DARK, FOREST_MID, FOREST_LT,
                          FOREST_DARK, FOREST_MID])
        tree(ax, tx, ty, h=h, col=col, zorder=zorder)

def dead_tree(ax, cx, cy, h=3.0, zorder=9):
    """Bare/dead tree for Withered Depths — tapered trunk with secondary branching."""
    lean = RNG.uniform(-0.25, 0.25)
    # Tapered trunk in 4 segments
    for seg in range(4):
        sy = cy + seg * h / 4
        ey = cy + (seg + 1) * h / 4
        lw = max(0.3, 1.5 - seg * 0.32)
        ox = lean * (seg + 1) * 0.12
        ax.plot([cx + lean*seg*0.12, cx + ox], [sy, ey],
                color='#3C2C18', linewidth=lw,
                zorder=zorder, solid_capstyle='round')
    # Primary branches
    for _ in range(5):
        ang = RNG.uniform(0.2, math.pi - 0.2) * RNG.choice([-1, 1])
        bl  = RNG.uniform(0.8, 1.8)
        by  = cy + RNG.uniform(h*0.40, h*0.90)
        bx  = cx + lean * (by - cy) / h * 0.3
        bex = bx + math.cos(ang) * bl
        bey = by + math.sin(ang) * bl * 0.50
        ax.plot([bx, bex], [by, bey],
                color='#3C2C18', linewidth=0.75, alpha=0.82, zorder=zorder)
        # Secondary branches
        if bl > 1.0:
            for _ in range(2):
                sang = ang + RNG.uniform(-0.8, 0.8)
                sbl  = bl * RNG.uniform(0.28, 0.52)
                t2   = RNG.uniform(0.45, 0.75)
                sbx  = bx + math.cos(ang)*bl*t2
                sby  = by + math.sin(ang)*bl*t2*0.50
                ax.plot([sbx, sbx + math.cos(sang)*sbl],
                        [sby, sby + math.sin(sang)*sbl*0.45],
                        color='#3C2C18', linewidth=0.38, alpha=0.65, zorder=zorder)

def gnarled_tree(ax, cx, cy, h=3.0, zorder=9):
    """Thornwood gnarled tree — segmented twisted trunk with lobed dark crown."""
    lean = RNG.uniform(-0.6, 0.6)
    # Segmented trunk with accumulated lean and micro-jitter
    pts = [(cx, cy)]
    for seg in range(4):
        t = (seg + 1) / 4
        pts.append((
            cx + lean*t*0.45 + RNG.uniform(-0.1, 0.1)*h*0.15,
            cy + h * t))
    for i in range(len(pts) - 1):
        lw = max(0.4, 1.6 - i * 0.28)
        ax.plot([pts[i][0], pts[i+1][0]], [pts[i][1], pts[i+1][1]],
                color='#1A0E08', linewidth=lw, zorder=zorder,
                solid_capstyle='round')
    crown_cx = pts[-1][0]
    crown_cy = pts[-1][1] - h * 0.08
    # Crown — multiple overlapping dark lobes for organic silhouette
    lobe_specs = [
        (0.0,       0.0,    h*0.42, '#1E2A0A', 0.84),
        (-h*0.22,   h*0.08, h*0.28, '#0E1806', 0.76),
        ( h*0.20,   h*0.05, h*0.26, '#182208', 0.74),
        ( h*0.05,  -h*0.12, h*0.22, '#1A2A0A', 0.66),
        (-h*0.12,  -h*0.10, h*0.20, '#131A06', 0.62),
    ]
    for ox, oy, r, c, al in lobe_specs:
        ax.add_patch(plt.Circle((crown_cx + ox, crown_cy + oy), r,
                                 color=c, alpha=al, zorder=zorder+1))
    # Thorn tips projecting from crown edge
    for _ in range(6):
        ang = RNG.uniform(0, 2*math.pi)
        br  = h * RNG.uniform(0.38, 0.50)
        bx  = crown_cx + br * math.cos(ang)
        by  = crown_cy + br * 0.70 * math.sin(ang)
        tip = h * RNG.uniform(0.11, 0.19)
        ax.plot([bx, bx + tip*math.cos(ang)],
                [by, by + tip*math.sin(ang)*0.55],
                color='#1A0E08', linewidth=0.55, alpha=0.62, zorder=zorder+2)
    # Faint crown outline to crisp the silhouette
    ax.add_patch(plt.Circle((crown_cx, crown_cy), h*0.45,
                             fill=False, edgecolor=FOREST_INK,
                             linewidth=0.35, alpha=0.30, zorder=zorder+2))

def oasis(ax, cx, cy, r=4.0, zorder=11):
    # Water
    ax.add_patch(plt.Circle((cx, cy), r*0.65, color=SEA_MID,
                             alpha=0.85, zorder=zorder))
    ax.add_patch(plt.Circle((cx, cy), r*0.40, color=SEA_LT,
                             alpha=0.55, zorder=zorder+1))
    # Ripple rings
    for rr in [r*0.50, r*0.60]:
        ax.add_patch(plt.Circle((cx, cy), rr, fill=False,
                                 edgecolor=SEA_PALE, linewidth=0.5,
                                 alpha=0.45, zorder=zorder+1))
    # Palms
    for ang in np.linspace(0, 2*math.pi, 7)[:-1]:
        px = cx + r*0.82*math.cos(ang)
        py = cy + r*0.82*math.sin(ang)
        # trunk lean
        lean_x = math.cos(ang)*0.4
        ax.plot([px, px+lean_x], [py, py+2.2],
                color='#6B4420', linewidth=1.0, zorder=zorder+2)
        ax.add_patch(plt.Circle((px+lean_x, py+2.2), 1.0,
                                 color=FOREST_MID, alpha=0.85, zorder=zorder+3))

def pyramid_sym(ax, cx, cy, size=4.0, col='#B89828', zorder=13):
    h    = size * 1.58
    apex = (cx, cy + h)
    bl   = (cx - size, cy)
    br   = (cx + size, cy)

    # Ground shadow
    ax.add_patch(mpatches.Ellipse((cx + size*0.35, cy), size*3.0, size*0.36,
                                   color=MTN_DARK, alpha=0.18, zorder=zorder-1))
    # Stone platform base
    rgb = mcolors.to_rgb(col)
    base_col = tuple(c * 0.75 for c in rgb)
    ax.add_patch(mpatches.Rectangle(
        (cx - size*1.08, cy - size*0.15), size*2.16, size*0.15,
        color=base_col, alpha=0.88, zorder=zorder))

    # Light face (left half)
    light_col = tuple(min(1.0, c * 1.22) for c in rgb)
    dark_col  = tuple(c * 0.46 for c in rgb)
    ax.add_patch(plt.Polygon([apex, bl, (cx, cy)],
                              color=light_col, alpha=0.93, zorder=zorder))
    # Shadow face (right half)
    ax.add_patch(plt.Polygon([apex, (cx, cy), br],
                              color=dark_col, alpha=0.93, zorder=zorder))

    # Stone course lines (converging toward apex)
    n_courses = 8
    for i in range(1, n_courses):
        frac     = i / n_courses
        course_y = cy + h * frac
        course_w = size * (1 - frac)
        al_lt    = 0.20 + frac * 0.10
        al_dk    = 0.32 + frac * 0.18
        ax.plot([cx - course_w, cx], [course_y, course_y],
                color=MTN_DARK, linewidth=0.44, alpha=al_lt, zorder=zorder+1)
        ax.plot([cx, cx + course_w], [course_y, course_y],
                color=MTN_DARK, linewidth=0.44, alpha=al_dk, zorder=zorder+1)

    # Outline
    ax.add_patch(plt.Polygon([apex, bl, br],
                              fill=False, edgecolor=MTN_DARK,
                              linewidth=1.1, alpha=0.88, zorder=zorder+2))
    # Ridge line
    ax.plot([cx, cx], [cy, cy + h],
            color=MTN_DARK, linewidth=0.44, alpha=0.38, zorder=zorder+2)

    # Gilded capstone
    cap = size * 0.14
    ax.add_patch(plt.Polygon(
        [[cx - cap*0.9, cy + h - cap*2.2],
         [cx + cap*0.9, cy + h - cap*2.2],
         [cx, cy + h]],
        color=GOLD_LT, alpha=0.82, zorder=zorder+3))

    # Black Sun glyph
    gy = cy + h * 0.46
    ax.add_patch(plt.Circle((cx, gy), size*0.115,
                             color='#CC1100', alpha=0.62, zorder=zorder+3))
    ax.add_patch(plt.Circle((cx, gy), size*0.065,
                             color='#FF3300', alpha=0.88, zorder=zorder+4))
    for ray_i in range(8):
        ang = ray_i * math.pi / 4
        ax.plot([cx + size*0.13*math.cos(ang), cx + size*0.24*math.cos(ang)],
                [gy + size*0.13*math.sin(ang), gy + size*0.24*math.sin(ang)],
                color='#CC1100', linewidth=0.52, alpha=0.42, zorder=zorder+3)

def city_symbol(ax, cx, cy, size=1.6, col=INK, crown=False, zorder=16):
    """Medieval town symbol: walled keep with gate-towers."""
    bw = size * 2.0
    bh = size * 1.4
    # Main wall
    ax.add_patch(mpatches.Rectangle(
        (cx - bw/2, cy - bh*0.3), bw, bh,
        color=col, alpha=0.88, zorder=zorder))
    # Two towers
    for tx in [cx - bw/2, cx + bw/2 - size*0.6]:
        ax.add_patch(mpatches.Rectangle(
            (tx, cy - bh*0.3), size*0.6, bh*1.5,
            color=col, alpha=0.92, zorder=zorder+1))
        # Battlements
        for bx in np.linspace(tx, tx+size*0.6, 4)[:-1]:
            ax.add_patch(mpatches.Rectangle(
                (bx, cy - bh*0.3 + bh*1.5),
                size*0.12, size*0.35,
                color=col, alpha=0.92, zorder=zorder+2))
    # Gate arch
    arch_x = cx - size*0.18
    ax.add_patch(mpatches.Arc((cx, cy - bh*0.3 + size*0.2),
                               size*0.36, size*0.5,
                               angle=0, theta1=0, theta2=180,
                               color=PARCH_PALE, linewidth=0, zorder=zorder+3))
    ax.add_patch(mpatches.Rectangle(
        (arch_x, cy - bh*0.3), size*0.36, size*0.2,
        color=PARCH_PALE, alpha=0.85, zorder=zorder+3))
    if crown:
        for ang in np.linspace(0, 2*math.pi, 6)[:-1]:
            ax.plot(cx + size*0.5*math.cos(ang),
                    cy + bh*1.3 + size*0.3*math.sin(ang),
                    '*', color=GOLD_LT, markersize=5,
                    zorder=zorder+4, alpha=0.9)

def port_symbol(ax, cx, cy, size=1.4, col=INK, zorder=16):
    """Port city — tower + anchor."""
    city_symbol(ax, cx, cy, size=size, col=col, zorder=zorder)
    # Anchor
    ax.text(cx + size*1.6, cy, '\u2693', color=SEA_MID,
            fontsize=7, ha='center', va='center', zorder=zorder+1, alpha=0.6)

def ruin_sym(ax, cx, cy, size=1.5, col=MTN_LT, zorder=12):
    """Broken column / ruin."""
    ax.add_patch(mpatches.Rectangle(
        (cx - size*0.35, cy), size*0.7, size*2.0,
        color=col, alpha=0.65, zorder=zorder))
    # Crack
    ax.plot([cx - size*0.1, cx + size*0.3],
            [cy + size*0.8, cy + size*1.4],
            color=MTN_DARK, linewidth=0.5, alpha=0.5, zorder=zorder+1)
    # Rubble scatter
    for rx, ry in [(cx-size, cy-size*0.3), (cx+size*0.5, cy-size*0.2),
                   (cx-size*0.3, cy-size*0.5), (cx+size*0.8, cy-size*0.5)]:
        ax.plot(rx, ry, 's', color=col, markersize=2.5,
                alpha=0.55, zorder=zorder)

def sunken_sanctum_sym(ax, cx, cy, size=2.2, zorder=14):
    """Sunken Sanctum: partially buried, arcane circle with crystal shards."""
    # Underground halo
    for r_halo, alpha in [(size*1.4, 0.12), (size*1.0, 0.18), (size*0.65, 0.25)]:
        ax.add_patch(plt.Circle((cx, cy), r_halo,
                                 color='#5030A0', alpha=alpha, zorder=zorder-1))
    # Broken outer ring
    for seg_start, seg_end in [(10,80),(100,170),(190,350)]:
        ax.add_patch(mpatches.Arc(
            (cx, cy), size*2.0, size*2.0,
            angle=0, theta1=seg_start, theta2=seg_end,
            color='#8050C0', linewidth=1.2, alpha=0.7, zorder=zorder))
    # Inner collapsed structure
    ax.add_patch(mpatches.Rectangle(
        (cx - size*0.7, cy - size*0.5), size*1.4, size*1.0,
        color='#3A2A5A', alpha=0.78, edgecolor='#6040A0',
        linewidth=0.8, zorder=zorder+1))
    # Crystal shards sticking up
    shard_positions = [(-size*0.4, size*0.3), (0, size*0.55),
                       (size*0.35, size*0.2), (-size*0.15, size*0.45)]
    for spx, spy in shard_positions:
        sh = RNG.uniform(0.6, 1.1) * size * 0.7
        sw = sh * 0.22
        ax.add_patch(plt.Polygon(
            [[cx+spx-sw, cy+spy], [cx+spx+sw, cy+spy], [cx+spx, cy+spy+sh]],
            color='#C0A0FF', alpha=0.75, zorder=zorder+2))
        # Crystal glow
        ax.add_patch(plt.Circle((cx+spx, cy+spy+sh*0.7),
                                 sw*1.5, color='#E8D8FF',
                                 alpha=0.25, zorder=zorder+2))
    # Entrance crack / stairs going down
    for i in range(5):
        step_w = size*0.4 - i*size*0.05
        step_y = cy - size*0.45 - i*size*0.12
        ax.plot([cx-step_w, cx+step_w], [step_y, step_y],
                color='#4030A0', linewidth=0.8, alpha=0.6, zorder=zorder+1)
    # Arcane rune glow centre
    ax.plot(cx, cy + size*0.2, 'o', color='#C080FF',
            markersize=4, alpha=0.8, zorder=zorder+3)
    ax.plot(cx, cy + size*0.2, 'o', color='white',
            markersize=2, alpha=0.5, zorder=zorder+4)

def void_citadel_sym(ax, cx, cy, size=2.0, zorder=14):
    """Void Citadel — dark spiked tower."""
    # Dark ground stain
    ax.add_patch(plt.Circle((cx, cy), size*1.2,
                             color=VOID_COL, alpha=0.28, zorder=zorder-1))
    # Rays
    for ang in np.linspace(0, 2*math.pi, 10)[:-1]:
        rx = cx + size*2.2*math.cos(ang)
        ry = cy + size*2.2*math.sin(ang)
        ax.plot([cx, rx], [cy, ry],
                color=VOID_LT, linewidth=0.5, alpha=0.30, zorder=zorder)
    # Tower body
    ax.add_patch(mpatches.Rectangle(
        (cx - size*0.55, cy - size*0.4), size*1.1, size*2.5,
        color=VOID_COL, alpha=0.90, zorder=zorder+1))
    # Spires
    for sx, sh in [(cx-size*0.3, size*1.2), (cx, size*1.6),
                   (cx+size*0.3, size*1.1)]:
        ax.add_patch(plt.Polygon(
            [[sx-size*0.18, cy+size*2.1], [sx+size*0.18, cy+size*2.1],
             [sx, cy+size*2.1+sh]],
            color=VOID_COL, alpha=0.92, zorder=zorder+2))
    # Glowing window
    ax.plot(cx, cy+size*1.5, 'o', color=VOID_LT,
            markersize=3.5, alpha=0.7, zorder=zorder+3)

def cathedral_sym(ax, cx, cy, size=1.8, zorder=14):
    """Cathedral of the Violet Eclipse — gothic spire with eclipse window."""
    # Nave body
    ax.add_patch(mpatches.Rectangle(
        (cx - size*0.9, cy - size*0.3), size*1.8, size*1.8,
        color='#2A1040', alpha=0.85, zorder=zorder))
    # Central spire
    ax.add_patch(plt.Polygon(
        [[cx - size*0.28, cy + size*1.5], [cx + size*0.28, cy + size*1.5],
         [cx, cy + size*3.2]],
        color='#3C1858', alpha=0.92, zorder=zorder+1))
    # Two flanking mini-spires
    for sx in [cx - size*0.7, cx + size*0.7]:
        ax.add_patch(plt.Polygon(
            [[sx - size*0.15, cy + size*1.2], [sx + size*0.15, cy + size*1.2],
             [sx, cy + size*2.2]],
            color='#321450', alpha=0.85, zorder=zorder+1))
    # Eclipse rose window
    ax.add_patch(plt.Circle((cx, cy + size*0.7), size*0.38,
                             color='#8040C0', alpha=0.55, zorder=zorder+2))
    ax.add_patch(plt.Circle((cx, cy + size*0.7), size*0.38,
                             fill=False, edgecolor='#C080F0',
                             linewidth=0.9, alpha=0.80, zorder=zorder+2))
    # Half-shadow on rose window
    ax.add_patch(mpatches.Wedge((cx, cy + size*0.7), size*0.35,
                                 90, 270, color='#120622',
                                 alpha=0.50, zorder=zorder+3))
    # Violet glow halo from spire tip
    ax.add_patch(plt.Circle((cx, cy + size*3.2), size*0.25,
                             color='#A060E0', alpha=0.45, zorder=zorder+2))

def compass_rose(ax, cx, cy, size=9):
    """Ornate compass rose."""
    # Outer decorative ring
    for r, lw, col, alpha in [(size*1.45, 1.8, INK, 0.65),
                               (size*1.30, 0.7, GOLD, 0.55)]:
        ax.add_patch(plt.Circle((cx,cy), r, fill=False,
                                 edgecolor=col, linewidth=lw,
                                 alpha=alpha, zorder=31))
    ax.add_patch(plt.Circle((cx,cy), size*1.45,
                             color=PARCH_PALE, alpha=0.80, zorder=30))
    # Eight-point star
    for i, ang in enumerate(np.linspace(0, 2*math.pi, 9)[:-1]):
        cardinal = (i % 2 == 0)
        tip_len  = size if cardinal else size*0.60
        tip_x = cx + tip_len * math.sin(ang)
        tip_y = cy + tip_len * math.cos(ang)
        base_w = size * (0.28 if cardinal else 0.18)
        perp_ang = ang + math.pi/2
        bx1 = cx + base_w * math.sin(perp_ang)
        by1 = cy + base_w * math.cos(perp_ang)
        bx2 = cx - base_w * math.sin(perp_ang)
        by2 = cy - base_w * math.cos(perp_ang)
        col = GOLD if cardinal else INK_FADED
        ax.add_patch(plt.Polygon([[tip_x,tip_y],[bx1,by1],[bx2,by2]],
                                  color=col, alpha=0.92, zorder=32))
    # Cardinal labels
    for label, ang in [('N',0),('S',180),('E',90),('W',270)]:
        lx = cx + (size*1.65) * math.sin(math.radians(ang))
        ly = cy + (size*1.65) * math.cos(math.radians(ang))
        ax.text(lx, ly, label, fontsize=7.5, fontweight='bold',
                color=INK, ha='center', va='center',
                fontfamily='DejaVu Serif', zorder=33)
    # Center jewel
    ax.add_patch(plt.Circle((cx,cy), size*0.14,
                             color=GOLD_LT, alpha=0.95, zorder=34))
    ax.add_patch(plt.Circle((cx,cy), size*0.07,
                             color=BLOOD_RED, alpha=0.90, zorder=35))

def decorative_border(ax):
    """Layered ornate border — drawn as edge strips only, never covering map content."""
    m = 3.5
    bw = 5.0   # border band width

    # Four edge bands (top/bottom/left/right strips)
    for rx, ry, rw, rh in [
        (0, 0, W, bw),           # bottom
        (0, H-bw, W, bw),        # top
        (0, 0, bw, H),           # left
        (W-bw, 0, bw, H),        # right
    ]:
        ax.add_patch(mpatches.Rectangle((rx, ry), rw, rh,
                      color=PARCH_DARK, alpha=0.92, zorder=55))

    # Three nested frame lines
    for offset, lw, col, alpha in [
        (m,       2.2, INK,       0.88),
        (m+1.8,   0.8, GOLD,      0.70),
        (m+3.0,   0.5, INK_FADED, 0.50),
    ]:
        ax.add_patch(mpatches.FancyBboxPatch(
            (offset, offset), W-2*offset, H-2*offset,
            boxstyle='square,pad=0', fill=False,
            edgecolor=col, linewidth=lw, alpha=alpha, zorder=56))

    # Corner ornaments
    for cx2, cy2 in [(m+1.5,m+1.5),(W-m-1.5,m+1.5),(m+1.5,H-m-1.5),(W-m-1.5,H-m-1.5)]:
        for r, al in [(4.5,0.15),(3.0,0.28),(1.8,0.50)]:
            ax.add_patch(plt.Circle((cx2,cy2), r,
                                     color=GOLD, alpha=al, zorder=57))
        ax.plot(cx2, cy2, '*', color=GOLD_LT, markersize=8,
                alpha=0.90, zorder=58)

    # Diamond tick marks along inner border edge
    for xi in np.linspace(14, W-14, 26):
        for yi in [m+1.0, H-m-1.0]:
            ax.plot(xi, yi, 'D', color=GOLD, markersize=1.4,
                    alpha=0.50, zorder=57)
    for yi in np.linspace(14, H-14, 20):
        for xi in [m+1.0, W-m-1.0]:
            ax.plot(xi, yi, 'D', color=GOLD, markersize=1.4,
                    alpha=0.50, zorder=57)

def label(ax, x, y, text, size=7, col=INK, style='normal', weight='normal',
          rot=0, zorder=22, ha='center', va='center', outline_col=None):
    oc = outline_col if outline_col else PARCH_PALE
    ax.text(x, y, text, fontsize=size, color=col, style=style,
            fontweight=weight, rotation=rot, zorder=zorder-1,
            ha=ha, va=va, fontfamily='DejaVu Serif',
            path_effects=[pe.withStroke(linewidth=2.8, foreground=oc)])
    ax.text(x, y, text, fontsize=size, color=col, style=style,
            fontweight=weight, rotation=rot, zorder=zorder,
            ha=ha, va=va, fontfamily='DejaVu Serif')

def lore_scroll(ax, cx, cy, w, h, text, text_size=4.5, col=INK_FADED,
                bg=PARCH_LT, edge=INK_LIGHT, zorder=22):
    """Small lore-quote scroll box."""
    ax.add_patch(FancyBboxPatch((cx-w/2, cy-h/2), w, h,
                  boxstyle='round,pad=0.6',
                  facecolor=bg, edgecolor=edge,
                  linewidth=0.7, alpha=0.65, zorder=zorder-1))
    ax.text(cx, cy, text, fontsize=text_size, color=col,
            ha='center', va='center', style='italic',
            fontfamily='DejaVu Serif', zorder=zorder,
            wrap=True, multialignment='center')

# ═══════════════════════════════════════════════════════════
#  FIGURE SETUP
# ═══════════════════════════════════════════════════════════
fig = plt.figure(figsize=(26, 19.5), dpi=180)
ax  = fig.add_axes([0, 0, 1, 1])
ax.set_xlim(0, W)
ax.set_ylim(0, H)
ax.set_aspect('equal')
ax.axis('off')

# Parchment background
parch_img = make_parchment(w_px=1440, h_px=1080)
ax.imshow(parch_img, extent=[0, W, 0, H], origin='upper',
          aspect='auto', zorder=0, alpha=1.0)

# Soft vignette overlay — stronger edges for depth
vx = np.linspace(-1, 1, 400)
vy = np.linspace(-1, 1, 300)
VX, VY = np.meshgrid(vx, vy)
vign = np.clip((VX**2 + VY**2)**0.55 * 0.65, 0, 1)
ax.imshow(vign, extent=[0, W, 0, H], origin='upper', aspect='auto',
          cmap='Greys', alpha=0.32, zorder=1)

# ═══════════════════════════════════════════════════════════
#  WATER BODIES
# ═══════════════════════════════════════════════════════════

# Eastern sea
sea_pts = [
    (W, 0), (W, H), (196,H), (193,162),(188,142),(192,122),
    (186,102),(191,82),(189,62),(193,42),(188,22),(195,0)
]
fill_region(ax, sea_pts, SEA_DEEP, alpha=0.82, edge_color=SEA_MID,
            edge_lw=1.2, zorder=3, tension=15)

# Sea shimmer lines — layered for depth
for yi in np.arange(4, 175, 4.2):
    xs_w = np.linspace(198, W-2, 100)
    ys_w = yi + 0.55 * np.sin(xs_w * 0.55 + yi * 0.8)
    ax.plot(xs_w, ys_w, color=SEA_LT, linewidth=0.40, alpha=0.28, zorder=4)
for yi in np.arange(6, 175, 9):
    xs_w = np.linspace(200, W-1, 60)
    ys_w = yi + 0.30 * np.cos(xs_w * 0.9 + yi)
    ax.plot(xs_w, ys_w, color=SEA_PALE, linewidth=0.25, alpha=0.18, zorder=4)

# Northern sea (Kowloon reach)
north_sea_pts = [
    (68,H),(68,165),(80,167),(95,170),(112,172),(130,170),
    (150,166),(168,162),(182,164),(193,162),(196,H)
]
fill_region(ax, north_sea_pts, SEA_MID, alpha=0.78, edge_color=SEA_LT,
            edge_lw=1.0, zorder=3, tension=12)

# ═══════════════════════════════════════════════════════════
#  RIVERS
# ═══════════════════════════════════════════════════════════

# Main river from Cinderteeth → Kowloon delta
river_main = [(148,124),(142,131),(136,138),(128,145),(122,152),(118,160),(115,167)]
draw_smooth_line(ax, river_main, RIVER_COL, lw=3.8, alpha=0.50, zorder=6)
draw_smooth_line(ax, river_main, SEA_LT, lw=1.4, alpha=0.30, zorder=7)

# Delta fingers
for dx, dy_end in [(-9,168),(-3,170),(4,169),(11,168)]:
    draw_smooth_line(ax,
        [(118+dx, 162),(116+dx*0.8, 165),(113+dx*0.5, 168+dy_end-168)],
        RIVER_COL, lw=2.0, alpha=0.45, zorder=6)

# Western forest river
west_river = [(58,118),(66,126),(78,134),(92,144),(104,152),(112,160)]
draw_smooth_line(ax, west_river, RIVER_COL, lw=2.6, alpha=0.42, zorder=6)

# Small tributary (oasis → desert edge)
draw_smooth_line(ax,
    [(138,78),(140,73),(145,68),(152,64)],
    RIVER_COL, lw=1.0, alpha=0.30, zorder=6, tension=0.3)

# ═══════════════════════════════════════════════════════════
#  TERRAIN FILLS — from bottom up
# ═══════════════════════════════════════════════════════════

# Southern barren wastes
fill_region(ax, [(15,0),(95,0),(100,22),(88,28),(72,24),(60,28),(45,22),(28,26),(15,18)],
            PARCH_DARK, alpha=0.38, edge_color=DESERT_BRN, edge_lw=0.5, zorder=4)

# Gravelands / Necropolis corridor (south)
grave_pts = [
    (55,30),(72,26),(92,30),(98,44),(94,58),(82,66),(68,68),(54,60),(50,46)
]
fill_region(ax, grave_pts, CRYPT_COL, alpha=0.52, edge_color='#3A2830',
            edge_lw=0.8, zorder=4)
hatch_region(ax, grave_pts, '#604040', dot_count=160, size=0.6, alpha=0.18, zorder=5)

# Great Desert Expanse
desert_pts = [
    (118,42),(136,38),(158,40),(178,46),(190,58),(188,80),(178,88),
    (160,90),(140,88),(125,80),(115,68),(112,54)
]
fill_region(ax, desert_pts, DESERT_TAN, alpha=0.68, edge_color=DESERT_BRN,
            edge_lw=0.9, zorder=4)
hatch_region(ax, desert_pts, DESERT_BRN, dot_count=260, size=0.6, alpha=0.14, zorder=5)
# Inner warm gold highlight
desert_inner = [
    (126,50),(144,46),(162,48),(178,56),(180,72),(170,82),
    (154,84),(140,82),(128,74),(120,62),(118,52)
]
fill_region(ax, desert_inner, '#D4A030', alpha=0.20, edge_color=None,
            edge_lw=0, zorder=5)

# Scorched Wastes
scorch_pts = [(100,62),(116,58),(126,62),(128,72),(120,80),(108,82),(98,76),(96,68)]
fill_region(ax, scorch_pts, SCORCH_COL, alpha=0.50, edge_color='#5A2010',
            edge_lw=0.7, zorder=5)
hatch_region(ax, scorch_pts, '#303030', dot_count=100, size=0.6, alpha=0.22, zorder=6)

# Scorching Sands
scorch_sand_pts = [(148,38),(168,33),(184,36),(188,50),(174,54),(156,50),(148,38)]
fill_region(ax, scorch_sand_pts, '#B07030', alpha=0.48, edge_color='#804010',
            edge_lw=0.6, zorder=5)

# Saltglass Reach
salt_pts = [(168,58),(186,54),(194,62),(190,78),(176,82),(162,78),(162,64)]
fill_region(ax, salt_pts, SALT_COL, alpha=0.62, edge_color='#A8A090',
            edge_lw=0.8, zorder=5)
# Salt crystal sparkles
for _ in range(35):
    sx = RNG.uniform(166,188)
    sy = RNG.uniform(60,78)
    ax.plot(sx, sy, '+', color='white', markersize=2.8, alpha=0.75, zorder=6)

# Great Northern Forest — base fill (two-layer for depth)
forest_pts = [
    (28,118),(50,115),(72,118),(94,120),(114,118),(136,116),(158,120),
    (168,128),(165,148),(150,155),(132,158),(112,156),(92,158),(72,154),
    (52,150),(34,144),(20,136),(18,124)
]
fill_region(ax, forest_pts, FOREST_DARK, alpha=0.52, edge_color=FOREST_INK,
            edge_lw=1.0, zorder=4)
# Inner lighter layer for depth
forest_inner_pts = [
    (36,122),(60,120),(82,122),(100,122),(118,120),(140,120),(160,125),
    (163,142),(150,150),(132,154),(112,152),(90,153),(70,150),
    (54,147),(36,140),(24,132),(22,126)
]
fill_region(ax, forest_inner_pts, FOREST_MID, alpha=0.28, edge_color=None,
            edge_lw=0, zorder=4)

# Withered Depths blight
wither_pts = [(103,124),(120,120),(130,124),(128,140),(116,145),(102,138)]
fill_region(ax, wither_pts, BLIGHT_COL, alpha=0.55, edge_color='#282018',
            edge_lw=0.7, zorder=5)

# Thornwood — necrotic estate-forest at southern fringe of GNF, north of Midgaard
thorn_pts = [(56,112),(70,110),(84,113),(83,122),(72,126),(58,124),(53,118)]
fill_region(ax, thorn_pts, '#202E10', alpha=0.52, edge_color=FOREST_INK,
            edge_lw=0.7, zorder=5)

# Shadowmere — the Blighted Crownlands (failed frontier march within the GNF)
shadowmere_pts = [
    (67,128),(80,125),(94,127),(98,134),(94,142),(80,145),(66,141),(62,134)
]
fill_region(ax, shadowmere_pts, '#34202C', alpha=0.52, edge_color='#281820',
            edge_lw=0.8, zorder=5)
hatch_region(ax, shadowmere_pts, '#605048', dot_count=120, size=0.6, alpha=0.20, zorder=6)

# Eccentric Woodland — between Midgaard (south gate) and Rakuen
eccentric_pts = [
    (53,78),(64,76),(68,82),(67,90),(60,92),(53,90),(51,84)
]
fill_region(ax, eccentric_pts, '#1E3C12', alpha=0.44, edge_color=FOREST_INK,
            edge_lw=0.7, zorder=5)

# ═══════════════════════════════════════════════════════════
#  MOUNTAINS
# ═══════════════════════════════════════════════════════════

# Cinderteeth Mountains — main volcanic range (expanded, more dramatic)
cinderteeth = [
    (132,118),(136,121),(140,126),(144,120),(148,128),(152,122),
    (156,130),(160,124),(164,118),(168,122),(170,116),
    (148,134),(142,131),(155,136),(158,128),(134,124),
    (138,130),(162,120),(166,126),
]
mountain_range(ax, cinderteeth, base_h=6.2, base_w=5.0,
               col='#5A2E18', shadow='#2E1208', snow='#F0EBE2', zorder=11)
# Second tier — smaller outlier peaks
cinderteeth_foothills = [
    (130,115),(134,118),(162,115),(170,118),(172,112),
    (126,122),(128,128),(174,122)
]
mountain_range(ax, cinderteeth_foothills, base_h=3.8, base_w=3.2,
               col=MTN_MID, shadow=MTN_DARK, snow=MTN_SNOW, zorder=10)
# Volcanic fire glow — two active vents
for vp in [(148,129),(156,131)]:
    for r, al in [(9,0.10),(6,0.18),(3.5,0.28),(2,0.38)]:
        ax.add_patch(plt.Circle(vp, r, color='#DD2800', alpha=al, zorder=10))
    ax.plot(vp[0], vp[1]+6.5, '^', color=FIRE_ORG, markersize=7, alpha=0.85, zorder=12)
    ax.plot(vp[0], vp[1]+6.5, '^', color='#FF7700', markersize=4, alpha=0.95, zorder=13)
    # Lava glow on surrounding terrain
    ax.add_patch(plt.Circle(vp, 14, color='#882200', alpha=0.06, zorder=9))
    # Smoke plumes — billowing upward
    for pi in range(8):
        sx = vp[0] + RNG.uniform(-1.5, 1.5)
        ax.add_patch(plt.Circle((sx, vp[1]+8+pi*2.8),
                                 0.8+pi*0.5, color='#504844',
                                 alpha=max(0.01, 0.16-pi*0.018), zorder=11))

# West border ridges (Iseth Wilds)
west_mts = [(11,82),(13,90),(11,98),(13,106),(11,114),(13,121)]
mountain_range(ax, west_mts, base_h=4.2, base_w=3.5, col=MTN_MID,
               shadow=MTN_DARK, snow=MTN_SNOW, zorder=10)

# Southern ridges
south_mts = [(54,20),(60,16),(68,20),(76,16),(84,20),(92,16),(100,20)]
mountain_range(ax, south_mts, base_h=4.0, base_w=3.2, col=MTN_LT,
               shadow=MTN_MID, snow=MTN_SNOW, zorder=10)

# ═══════════════════════════════════════════════════════════
#  FOREST TREES
# ═══════════════════════════════════════════════════════════

forest_cluster_specs = [
    (38,130,11,72), (56,135,10,68), (74,133,11,72),
    (92,130,10,66), (108,132,10,64), (128,130,9,60),
    (148,135,9,58), (30,138,8,50),  (50,143,9,60),
    (70,140,10,66), (90,143,9,62),  (110,142,8,52),
    (130,140,9,58), (148,138,8,52),
    (24,128,6,38),  (160,138,7,44),
    # Extra density clusters
    (42,138,7,44), (62,142,8,50), (80,136,7,42),
    (96,138,8,48), (114,136,7,40), (136,134,8,46),
    (156,132,7,38), (38,144,6,34), (158,142,6,32),
]
for cx_, cy_, r_, c_ in forest_cluster_specs:
    forest_cluster(ax, cx_, cy_, r_, count=c_, h_range=(1.6,3.2), zorder=9)

# Withered Depths — dead trees
for _ in range(38):
    wx = RNG.uniform(105,128)
    wy = RNG.uniform(122,142)
    dead_tree(ax, wx, wy, h=RNG.uniform(2.2,4.0), zorder=9)

# Thornwood — gnarled trees at GNF southern fringe, north of Midgaard
for _ in range(55):
    wx = RNG.uniform(55,83)
    wy = RNG.uniform(111,126)
    gnarled_tree(ax, wx, wy, h=RNG.uniform(2.0,3.8), zorder=9)

# Eccentric Woodland trees (mixed, slightly whimsical)
for _ in range(40):
    wx = RNG.uniform(53,68)
    wy = RNG.uniform(78,91)
    col_ = RNG.choice([FOREST_MID, FOREST_LT, '#3A5A22', FOREST_DARK, '#4A6830'])
    tree(ax, wx, wy, h=RNG.uniform(1.8,3.0), col=col_, zorder=9)

# ═══════════════════════════════════════════════════════════
#  SPECIAL FEATURES
# ═══════════════════════════════════════════════════════════

# ── Three Oases ────────────────────────────────────────────
oasis(ax, 138, 76, r=5.0, zorder=11)    # Great Oasis
oasis(ax, 140, 90, r=3.5, zorder=11)    # Northern Oasis
oasis(ax, 131, 63, r=3.5, zorder=11)    # Southern Oasis

# Deepwell confluence marker
ax.add_patch(plt.Circle((138,76), 4.5, fill=False,
                         edgecolor=RIVER_COL, linewidth=0.8,
                         alpha=0.30, linestyle='--', zorder=10))
ax.add_patch(plt.Circle((138,76), 3.2, fill=False,
                         edgecolor=RIVER_COL, linewidth=0.5,
                         alpha=0.20, linestyle=':', zorder=10))

# ── Three Pyramids ─────────────────────────────────────────
pyramid_sym(ax, 175, 60, size=5.5, col='#C0960A', zorder=13)  # Great Pyramid
pyramid_sym(ax, 167, 76, size=4.2, col='#B08808', zorder=13)  # Northern Pyramid
pyramid_sym(ax, 172, 42, size=4.2, col='#A07808', zorder=13)  # Southern Pyramid

# Black Sun Shard corona (Great Pyramid)
for r_, al_, ls_ in [(5.5,0.30,'-'),(7.5,0.18,'--'),(9.5,0.10,':')]:
    ax.add_patch(plt.Circle((175,68.5), r_, fill=False,
                             edgecolor=BLOOD_RED, linewidth=0.7,
                             alpha=al_, linestyle=ls_, zorder=12))
for ang in np.linspace(0,2*math.pi,14)[:-1]:
    ray_l = RNG.uniform(6,11)
    ax.plot([175, 175+ray_l*math.cos(ang)],
            [68.5, 68.5+ray_l*math.sin(ang)],
            color=BLOOD_RED, linewidth=0.4, alpha=0.15, zorder=11)

# Khepra-Lesh ruined city
ruin_sym(ax, 160, 64, size=1.8, col='#8A7850', zorder=11)
ruin_sym(ax, 163, 66.5, size=1.3, col='#7A6840', zorder=11)

# ── Void Citadel of Kel'Shadra (underground at Roc Road Crossroads) ────────
# Accessed by going DOWN from the Roc Road Crossroads hub (rocroad.are #1148),
# northwest of Midgaard between Kiess and the city.
void_citadel_sym(ax, 44, 95, size=2.0, zorder=14)

# Kel'Shadra crypts skull mark — within/just north of Midgaard
# Per midgaard.are southern gate room: "To the north, the Crypts of Kel'Shadra
# lie along the route past the market districts."
ax.add_patch(plt.Circle((62,97), 2.5, color=CRYPT_COL, alpha=0.55, zorder=12))
ax.text(62, 97, '☠', color='#6A3A4A', fontsize=11, ha='center', va='center',
        alpha=0.80, zorder=13)

# Gloamvault is a subterranean vault inside Midgaard — no separate overworld symbol

# Void Citadel connecting crack-lines
for ang in np.linspace(0,2*math.pi,9)[:-1]:
    ex = 44 + 4.5*math.cos(ang)
    ey = 95 + 4.5*math.sin(ang)
    ax.plot([44, ex], [95, ey], color=VOID_LT,
            linewidth=0.45, alpha=0.25, zorder=12)

# ── SUNKEN SANCTUM (western spur of Cinderteeth) ───────────
sunken_sanctum_sym(ax, 128, 121, size=2.4, zorder=14)

# Lava-tube tunnel hinting at Sanctum connection to mountains
draw_smooth_line(ax,
    [(128,123),(132,124),(137,124),(141,125)],
    VOID_LT, lw=0.8, alpha=0.15, zorder=10, style=':')

# ── Shrine of the First Claw ────────────────────────────────
ax.plot(188, 92, 'D', color=GOLD_LT, markersize=6.5, alpha=0.88, zorder=15)
ax.plot(188, 92, 'D', color='none', markersize=9,
        markeredgecolor=INK, markeredgewidth=0.8, zorder=15)
ax.add_patch(mpatches.Arc((188,92), 7, 7, angle=0, theta1=30, theta2=150,
                            color=GOLD, linewidth=0.6, alpha=0.45, zorder=14))

# Sultan's Palace — opulent desert capital near Great Pyramid
city_symbol(ax, 183, 66, size=1.6, col='#7A5000', crown=True, zorder=16)
for r_, al_ in [(4.5,0.15),(3.0,0.22)]:
    ax.add_patch(plt.Circle((183,66), r_, fill=False,
                             edgecolor=GOLD_LT, linewidth=0.7,
                             alpha=al_, zorder=15))

# Akh'enet — City of the Eclipsed Ledger (desert trade hub)
city_symbol(ax, 160, 80, size=1.4, col='#5A3808', crown=False, zorder=16)
# Eclipsed ledger symbol — overlapping circles
for r_, al_ in [(3.5,0.16),(2.2,0.22)]:
    ax.add_patch(plt.Circle((160,80), r_, fill=False,
                             edgecolor='#8A6020', linewidth=0.6,
                             alpha=al_, zorder=15))

# Spirebound Conclave ruins — in Cinderteeth foothills
ruin_sym(ax, 138, 116, size=1.6, col='#7050B0', zorder=12)
ruin_sym(ax, 142, 114, size=1.2, col='#604090', zorder=12)
ax.add_patch(plt.Circle((140,115), 5.5, fill=False,
                         edgecolor='#7050B0', linewidth=0.6,
                         alpha=0.22, linestyle=':', zorder=11))

# ── Cathedral of the Violet Eclipse (western Roc Road approach) ──────────
cathedral_sym(ax, 46, 92, size=1.8, zorder=14)
for r_, al_ in [(4.2, 0.16), (2.8, 0.26)]:
    ax.add_patch(plt.Circle((46, 92), r_, fill=False,
                             edgecolor='#9040D0', linewidth=0.7,
                             alpha=al_, zorder=13))

# ── Catacombs of Nightfall (beneath the southern chapel quarter of Midgaard) ──
# Accessed from Midgaard room 941 (Ossuary Transit Court); lore places it
# directly below the city's southern civic district.
ax.add_patch(plt.Circle((64, 90), 2.2, color='#1C1018', alpha=0.55, zorder=12))
ax.text(64, 90, '✝', color='#5A2840', fontsize=11, ha='center', va='center',
        alpha=0.82, zorder=13)
for r_, al_ in [(3.6, 0.14), (5.2, 0.07)]:
    ax.add_patch(plt.Circle((64, 90), r_, fill=False,
                             edgecolor='#3C1828', linewidth=0.5,
                             alpha=al_, linestyle=':', zorder=11))

# ── Umbra Heartspire (shadow archive between Midgaard and Gloamvault) ────
void_citadel_sym(ax, 70, 79, size=1.3, zorder=13)
ax.add_patch(plt.Circle((70, 79), 3.5, fill=False,
                         edgecolor='#5A3060', linewidth=0.5,
                         alpha=0.22, linestyle=':', zorder=12))

# ── Arroyo — the Jurisdictional Canyon (western Eastern Desert) ──────────
# Canyon walls
arroyo_n = [(119,52),(122,54),(126,53),(130,55),(134,54)]
arroyo_s = [(119,48),(122,50),(126,49),(130,51),(134,50)]
draw_smooth_line(ax, arroyo_n, '#5A3010', lw=1.6, alpha=0.65, zorder=10, tension=0.2)
draw_smooth_line(ax, arroyo_s, '#5A3010', lw=1.6, alpha=0.65, zorder=10, tension=0.2)
# Canyon floor fill
arroyo_fill = [(119,48),(122,50),(126,49),(130,51),(134,50),
               (134,54),(130,55),(126,53),(122,54),(119,52)]
fill_region(ax, arroyo_fill, '#4A2C0A', alpha=0.42, edge_color=None,
            edge_lw=0, zorder=9, tension=2)
ruin_sym(ax, 126, 51, size=1.2, col='#6A4828', zorder=12)

# ── Lost City (buried desert ruins, Eastern Desert interior) ────────────
ruin_sym(ax, 142, 55, size=1.9, col='#8A7040', zorder=12)
ruin_sym(ax, 146, 57, size=1.4, col='#7A6030', zorder=12)
ax.add_patch(plt.Circle((144, 56), 5.2, color=DESERT_TAN, alpha=0.10, zorder=9))

# ── Khar'Daan — the Sunken Necropolis (Saltglass Reach border) ──────────
ax.add_patch(plt.Circle((183, 83), 3.0, color='#20100C', alpha=0.52, zorder=12))
ax.text(183, 83, '☠', color='#5A2830', fontsize=12, ha='center', va='center',
        alpha=0.80, zorder=13)
ruin_sym(ax, 179, 82, size=1.2, col='#4A3020', zorder=11)
ruin_sym(ax, 185, 81, size=1.0, col='#4A3020', zorder=11)
ax.add_patch(plt.Circle((183, 83), 5.5, fill=False,
                         edgecolor=SALT_COL, linewidth=0.6,
                         alpha=0.28, linestyle=':', zorder=11))

# Shadowmere — blight symbol (ruined fortress within GNF)
ruin_sym(ax, 84, 135, size=1.8, col='#5A3A48', zorder=12)
ruin_sym(ax, 88, 137, size=1.3, col='#4A2A38', zorder=12)
ax.add_patch(plt.Circle((85, 136), 5.0, fill=False,
                         edgecolor='#4A2838', linewidth=0.6,
                         alpha=0.22, linestyle=':', zorder=11))

# ═══════════════════════════════════════════════════════════
#  CITIES
# ═══════════════════════════════════════════════════════════
city_symbol(ax, 63, 93,  size=2.0, col=INK,       crown=True,  zorder=16)  # Midgaard
city_symbol(ax, 24, 88,  size=1.7, col='#2A1A0A', crown=False, zorder=16)  # Kiess
city_symbol(ax, 60, 76,  size=1.4, col='#2A1A0A', crown=False, zorder=16)  # Rakuen
port_symbol(ax, 188, 76, size=1.5, col='#1A2030',              zorder=16)  # Mafdet
# Kowloon — delta city with Neon Covenant ring
city_symbol(ax, 115, 154, size=1.8, col='#1A2A3A', crown=False, zorder=16)
for r_, al_, col_ in [(4.0,0.35,TEAL_NEON),(5.8,0.20,TEAL_NEON)]:
    ax.add_patch(plt.Circle((115,154), r_, fill=False,
                             edgecolor=col_, linewidth=0.9,
                             alpha=al_, zorder=15))

# ═══════════════════════════════════════════════════════════
#  ROADS & TRADE ROUTES
# ═══════════════════════════════════════════════════════════

road_kw   = dict(lw=1.8, alpha=0.62, zorder=9, tension=0.5)
lantern_kw= dict(lw=1.5, alpha=0.58, zorder=9, tension=0.5)
caravan_kw= dict(lw=1.4, alpha=0.55, zorder=9, style='--', tension=0.5)

# Roc Road: Kiess → Midgaard
draw_smooth_line(ax, [(25,89),(35,90),(46,91),(56,92),(62,93)],
                 '#7A5028', **road_kw)
# Road mile-post markers
for mp in [(35,90.5),(46,91.5)]:
    ax.plot(*mp, '|', color='#7A5028', markersize=5, alpha=0.65, zorder=10)

# Lantern Road: Midgaard → Kowloon
lantern_pts = [(64,96),(68,103),(71,112),(76,122),(84,131),(94,140),(104,148),(112,153)]
draw_smooth_line(ax, lantern_pts, '#8A6818', **lantern_kw)
# Lantern glows
for i in range(1, len(lantern_pts)-1, 2):
    lx, ly = lantern_pts[i]
    ax.plot(lx, ly, 'o', color=GOLD_LT, markersize=3.5, alpha=0.7, zorder=10)
    ax.add_patch(plt.Circle((lx,ly), 2.2, fill=False,
                             edgecolor=GOLD, linewidth=0.35, alpha=0.28, zorder=10))

# Midgaard → Rakuen road
draw_smooth_line(ax, [(62,91),(61,86),(60,80),(60,77)],
                 '#7A5028', lw=1.4, alpha=0.55, zorder=9)

# Desert Caravan Road
draw_smooth_line(ax, [(139,78),(150,76),(160,71),(168,65),(175,62)],
                 DESERT_BRN, **caravan_kw)
# Caravan symbol
ax.text(152,74, '☽', color=DESERT_TAN, fontsize=7, ha='center',
        va='center', alpha=0.55, zorder=10, rotation=90)

# Caravan → Mafdet coast
draw_smooth_line(ax, [(175,62),(180,66),(184,70),(187,74)],
                 DESERT_BRN, lw=1.2, alpha=0.50, style='--', zorder=9)

# Short descent path: Midgaard southern gate → Catacombs of Nightfall entrance
# (the Ossuary Transport Court is within the city; only a brief approach is shown)
draw_smooth_line(ax, [(63,92),(64,91),(64,90)],
                 '#4A2818', lw=1.2, alpha=0.45, style=':', zorder=9)

# Branch road: Roc Road → Cathedral of the Violet Eclipse
draw_smooth_line(ax, [(43,90),(44,91),(46,92)],
                 '#6A3870', lw=1.0, alpha=0.48, style=':', zorder=9)

# ═══════════════════════════════════════════════════════════
#  DECORATIVE SEA ELEMENTS
# ═══════════════════════════════════════════════════════════

# Sea serpents
for sx, sy, scale in [(210,45,1.0),(220,95,1.2),(214,135,0.95)]:
    for ci in range(4):
        ang_ = ci * math.pi / 2
        ax.add_patch(plt.Circle(
            (sx+scale*3.5*math.cos(ang_), sy+scale*3.5*math.sin(ang_)),
            scale*2.0, color='#0A3050', alpha=0.22, zorder=7))
    ax.plot(sx+scale*2, sy+scale*2, '^', color='#0A405A',
            markersize=int(6.5*scale), alpha=0.50, zorder=8)
    ax.text(sx, sy-scale*5.5, 'HERE BE\nDRAGONS',
            fontsize=4.8, color=SEA_MID, ha='center', style='italic',
            fontfamily='DejaVu Serif', alpha=0.50, zorder=8)

# Sailing ship in northern sea
for shx, shy in [(148,172),(168,170)]:
    ax.plot([shx-4, shx+4], [shy, shy],  color=INK, lw=1.8, alpha=0.50, zorder=14)
    ax.plot([shx, shx],     [shy, shy+6], color=INK, lw=1.0, alpha=0.50, zorder=14)
    ax.add_patch(plt.Polygon(
        [[shx, shy+6],[shx-4,shy+2],[shx,shy+2]],
        color='#C8A840', alpha=0.52, zorder=15))

# Wind cherub heads (top-left and mid-right border areas)
for wchx, wchy, puff_ang in [(17,162,0),(232,150,180)]:
    ax.add_patch(plt.Circle((wchx,wchy), 5.5, color=PARCH_MID,
                             edgecolor=INK_LIGHT, linewidth=0.8,
                             alpha=0.72, zorder=20))
    ax.add_patch(plt.Circle((wchx-1.5,wchy), 2.0, color='#DDAA80',
                             alpha=0.60, zorder=21))
    ax.add_patch(plt.Circle((wchx+1.5,wchy), 2.0, color='#DDAA80',
                             alpha=0.60, zorder=21))
    for wi in range(5):
        pang = math.radians(puff_ang - 30 + wi*15)
        wl   = RNG.uniform(7,16)
        wavy_xs = [wchx + (5.5+t*wl)*math.cos(pang) for t in np.linspace(0,1,20)]
        wavy_ys = [wchy + (5.5+t*wl)*math.sin(pang)
                   + 0.6*math.sin(t*math.pi*4) for t in np.linspace(0,1,20)]
        ax.plot(wavy_xs, wavy_ys, color=SEA_LT,
                linewidth=0.65, alpha=0.35, zorder=20)

# ═══════════════════════════════════════════════════════════
#  LABELS
# ═══════════════════════════════════════════════════════════

# ── Major Cities ────────────────────────────────────────────
label(ax, 63, 99.5,'MIDGAARD',  size=12.5, col=INK, weight='bold')
label(ax, 24, 93,  'KIESS',     size=10.5, col=INK, weight='bold')
label(ax, 60, 80.5,'RAKUEN',    size=9.0,  col=INK_FADED, style='italic', weight='bold')
label(ax, 115,160, 'KOWLOON',   size=11.5, col='#1A2A3A', weight='bold')
label(ax, 188,80,  'MAFDET',    size=10.0, col='#1A2030', weight='bold')

# ── Regions ─────────────────────────────────────────────────
label(ax, 85,140,  'THE GREAT NORTHERN FOREST', size=9.5,
      col=FOREST_INK, style='italic', weight='bold')
label(ax, 152,138, 'CINDERTEETH\nMOUNTAINS', size=8.5,
      col='#4A1808', weight='bold')
label(ax, 148,57,  'EASTERN DESERT', size=8.5,
      col=DESERT_BRN, style='italic', weight='bold')
label(ax, 112,71,  'SCORCHED\nWASTES', size=7.0, col=SCORCH_COL, style='italic', weight='bold')
label(ax, 164,38,  'SCORCHING\nSANDS', size=6.5, col='#7A3010', style='italic', weight='bold')
label(ax, 178,68,  'SALTGLASS\nREACH', size=6.5, col='#585040', style='italic', weight='bold', rot=-12)
label(ax, 222,90,  'THE GLEAMING\nEASTERN SEA', size=8.5,
      col=SEA_MID, style='italic', weight='bold', rot=-90)
label(ax, 178,170, 'NORTHERN REACH', size=7.5, col=SEA_DEEP, style='italic', weight='bold')
label(ax, 34,128,  'VERDANT DEPTHS', size=6.5, col=FOREST_DARK, style='italic', weight='bold')
label(ax, 116,130, 'WITHERED\nDEPTHS', size=6.5, col='#504030', style='italic', weight='bold',
      outline_col=PARCH_LT)
label(ax, 22,122,  'FOREST OF\nCONFUSION', size=6.0, col=FOREST_MID, style='italic', weight='bold')
label(ax, 68,119,  'THORNWOOD', size=6.5, col='#0E2408', style='italic', weight='bold')
label(ax, 10,60,   'ISETH\nWILDS', size=6.5, col=MTN_MID, style='italic', weight='bold')
label(ax, 75,18,   'SUNKEN REACHES', size=6.0, col=MTN_LT, style='italic')

# ── Oases & Water ───────────────────────────────────────────
label(ax, 138,82,  'GREAT\nOASIS', size=7.0, col='#1A3A5A', style='italic', weight='bold')
label(ax, 142,95,  'NORTHERN\nOASIS', size=5.8, col='#1A3A5A', style='italic')
label(ax, 128,67,  'SOUTHERN\nOASIS', size=5.8, col='#1A3A5A', style='italic')

# ── Pyramids ────────────────────────────────────────────────
label(ax, 175,55,  'GREAT\nPYRAMID', size=7.0, col='#785008', weight='bold')
label(ax, 165,82,  'NORTHERN\nPYRAMID', size=5.8, col='#785008', style='italic')
label(ax, 170,36,  'SOUTHERN\nPYRAMID', size=5.8, col='#785008', style='italic')

# ── Necropolis / South ──────────────────────────────────────
label(ax, 76, 46,  'VOID CITADEL', size=6.0, col=VOID_LT, weight='bold',
      outline_col='#0A0410')
label(ax, 68, 56,  "KEL'SHADRA\nCRYPTS", size=5.8, col='#5A283A',
      style='italic', weight='bold', outline_col='#0A0410')
label(ax, 74, 26,  'SEPULCHUR\nPASTURE', size=5.8, col='#3A2818', style='italic')

# ── Special Sites ───────────────────────────────────────────
label(ax, 160, 62, 'KHEPRA-LESH\n(Ruined)', size=5.5, col='#7A6040', style='italic')
label(ax, 188, 96, 'SHRINE OF\nTHE FIRST CLAW', size=5.0, col='#7A5010', style='italic')
label(ax, 183, 62, "SULTAN'S\nPALACE", size=5.5, col='#7A5000', style='italic', weight='bold')
label(ax, 160, 76, "AKH'ENET", size=5.8, col='#6A4810', style='italic', weight='bold')
label(ax, 140, 111, 'SPIREBOUND\nCONCLAVE (RUINS)', size=4.8, col='#7050B0', style='italic',
      outline_col='#100820')
label(ax, 128, 115,'SUNKEN\nSANCTUM', size=6.5, col='#7050B0',
      weight='bold', outline_col='#0A0418')

# ── Shadowmere ──────────────────────────────────────────────────────────
label(ax, 80,  147, 'SHADOWMERE', size=6.0, col='#5A2A3A', style='italic', weight='bold',
      outline_col='#080408')
label(ax, 80,  144, 'Blighted Crownlands', size=4.5, col='#482030', style='italic',
      outline_col='#080408')

# ── Eccentric Woodland ───────────────────────────────────────────────────
label(ax, 59,  85,  'ECCENTRIC\nWOODLAND', size=5.5, col=FOREST_INK,
      style='italic', weight='bold')

# ── Violet Eclipse Cathedral ─────────────────────────────────────────────
label(ax, 44,  96,  'CATHEDRAL OF\nTHE VIOLET ECLIPSE', size=4.8, col='#7030A0',
      style='italic', weight='bold', outline_col='#0A0418')

# ── Catacombs of Nightfall ───────────────────────────────────────────────
label(ax, 67,  88,  'CATACOMBS\nOF NIGHTFALL', size=5.2, col='#5A2030',
      style='italic', weight='bold', outline_col='#080408')

# ── Crypts of Kel'Shadra ─────────────────────────────────────────────────
label(ax, 56,  98,  "CRYPTS OF\nKEL'SHADRA", size=5.0, col='#5A2840',
      style='italic', weight='bold', outline_col='#080408')

# ── Void Citadel of Kel'Shadra ───────────────────────────────────────────
label(ax, 44,  99,  "VOID CITADEL\nOF KEL'SHADRA", size=5.0, col=VOID_LT,
      style='italic', weight='bold', outline_col='#0A0418')

# ── Umbra Heartspire ─────────────────────────────────────────────────────
label(ax, 64,  76,  'UMBRA\nHEARTSPIRE', size=5.0, col=VOID_LT,
      style='italic', outline_col='#0A0418')

# ── Arroyo ───────────────────────────────────────────────────────────────
label(ax, 126, 57,  'THE ARROYO', size=5.5, col='#6A4010', style='italic', weight='bold')

# ── Lost City ────────────────────────────────────────────────────────────
label(ax, 144, 61,  'LOST CITY', size=5.8, col='#7A5820', style='italic', weight='bold')

# ── Khar'Daan ────────────────────────────────────────────────────────────
label(ax, 183, 88,  "KHAR'DAAN", size=5.5, col='#5A2830', style='italic', weight='bold',
      outline_col='#080408')
label(ax, 183, 85,  'Sunken Necropolis', size=4.2, col='#4A2020', style='italic',
      outline_col='#080408')

# ── Roads ───────────────────────────────────────────────────
label(ax, 44, 90,  'Roc Road', size=5.2, col='#7A5028', style='italic', rot=5)
label(ax, 88,118,  'Lantern Road', size=5.2, col='#8A6818', style='italic', rot=52)
label(ax, 155,72,  'Caravan Road', size=5.0, col=DESERT_BRN, style='italic', rot=-22)

# ── Faction seals ───────────────────────────────────────────
ax.add_patch(plt.Circle((150,60), 3.5, fill=False, edgecolor=DESERT_BRN,
                          linewidth=0.6, alpha=0.35, linestyle='--', zorder=10))
ax.text(150, 57, 'JACKAL\nSYNOD', fontsize=4.2, color=DESERT_BRN,
        ha='center', style='italic', fontfamily='DejaVu Serif', alpha=0.60, zorder=11)

ax.text(145, 83, 'Spring\nWardens', fontsize=4.0, color=SEA_DEEP,
        ha='center', style='italic', fontfamily='DejaVu Serif', alpha=0.55, zorder=11)

ax.text(70, 38, 'Bell\nSynod', fontsize=4.0, color=INK_FADED,
        ha='center', style='italic', fontfamily='DejaVu Serif', alpha=0.50, zorder=11)

ax.text(128, 110, 'Spirebound\nConclave\n(Ruins)', fontsize=4.0,
        ha='center', style='italic', fontfamily='DejaVu Serif', alpha=0.55,
        zorder=11, color='#7050B0')

# ═══════════════════════════════════════════════════════════
#  TITLE CARTOUCHE
# ═══════════════════════════════════════════════════════════
cx_cart, cy_cart = 68, 163
cw_cart, ch_cart = 100, 12

# Drop shadow
ax.add_patch(FancyBboxPatch(
    (cx_cart+1.0, cy_cart-1.0), cw_cart, ch_cart,
    boxstyle='round,pad=1.5',
    facecolor=PARCH_DARK, edgecolor='none', alpha=0.55, zorder=39))
# Main cartouche fill
ax.add_patch(FancyBboxPatch(
    (cx_cart, cy_cart), cw_cart, ch_cart,
    boxstyle='round,pad=1.5',
    facecolor=PARCH_LT, edgecolor=GOLD, linewidth=2.8, alpha=0.92, zorder=40))
# Inner line
ax.add_patch(FancyBboxPatch(
    (cx_cart+1.5, cy_cart+1.5), cw_cart-3, ch_cart-3,
    boxstyle='round,pad=0.8',
    facecolor='none', edgecolor=INK, linewidth=0.8, alpha=0.55, zorder=41))
# Corner flourishes
for fx, fy in [(cx_cart+3.5, cy_cart+3.5),
               (cx_cart+cw_cart-3.5, cy_cart+3.5),
               (cx_cart+3.5, cy_cart+ch_cart-3.5),
               (cx_cart+cw_cart-3.5, cy_cart+ch_cart-3.5)]:
    ax.plot(fx, fy, '*', color=GOLD_LT, markersize=7, alpha=0.88, zorder=42)

# Title text
mid_cart = cx_cart + cw_cart/2
ax.text(mid_cart, cy_cart+ch_cart-3.5, 'THE KNOWN WORLD',
        fontsize=15.0, fontweight='bold', color=INK, ha='center',
        va='center', fontfamily='DejaVu Serif', zorder=43,
        path_effects=[pe.withStroke(linewidth=3.0, foreground=PARCH_LT)])
ax.text(mid_cart, cy_cart+ch_cart-8.5, 'of the  ACK!MUD  Realm',
        fontsize=9.5, color=INK_FADED, ha='center', va='center',
        fontfamily='DejaVu Serif', style='italic', zorder=43)
ax.text(mid_cart, cy_cart+2.5,
        'Where Memory Holds, There Law Endures',
        fontsize=6.0, color=GOLD, ha='center', va='center',
        fontfamily='DejaVu Serif', style='italic', zorder=43)

# ═══════════════════════════════════════════════════════════
#  LEGEND
# ═══════════════════════════════════════════════════════════
lx_leg, ly_leg = 198, 48
lw_leg, lh_leg = 38, 68

ax.add_patch(FancyBboxPatch(
    (lx_leg, ly_leg), lw_leg, lh_leg,
    boxstyle='round,pad=1.2',
    facecolor=PARCH_LT, edgecolor=GOLD, linewidth=1.8, alpha=0.88, zorder=40))
ax.add_patch(FancyBboxPatch(
    (lx_leg+1.2, ly_leg+1.2), lw_leg-2.4, lh_leg-2.4,
    boxstyle='round,pad=0.6',
    facecolor='none', edgecolor=INK_LIGHT, linewidth=0.5, alpha=0.45, zorder=41))

ax.text(lx_leg+lw_leg/2, ly_leg+lh_leg-5,
        'LEGEND', fontsize=8.5, fontweight='bold', color=INK,
        ha='center', fontfamily='DejaVu Serif', zorder=42,
        path_effects=[pe.withStroke(linewidth=2, foreground=PARCH_LT)])

legend_items = [
    ('■', INK,        'City / Settlement'),
    ('■', INK,        '★ = Capital City'),
    ('▲', MTN_MID,    'Mountain Range'),
    ('●', SEA_MID,    'Water / Oasis'),
    ('▲', '#B09020',  'Sacred Pyramid'),
    ('◆', '#7050B0',  'Sunken Sanctum'),
    ('☠', VOID_COL,   'Void / Death Zone'),
    ('■', FOREST_DARK,'Ancient Forest'),
    ('■', SCORCH_COL, 'Scorched Wastes'),
    ('■', SALT_COL,   'Salt Flats'),
    ('◆', GOLD_LT,    'Shrine / Relic'),
    ('—', '#8A6818',  'Lantern Road'),
    ('—', '#7A5028',  'Trade Road'),
    ('- -', DESERT_BRN,'Caravan Road'),
]
for i, (sym, col, lbl) in enumerate(legend_items):
    iy = ly_leg + lh_leg - 13 - i * 3.5
    if sym in ('—', '- -'):
        ls = '--' if '- -' in sym else '-'
        ax.plot([lx_leg+3, lx_leg+10], [iy+0.4, iy+0.4],
                color=col, linewidth=1.6, linestyle=ls, alpha=0.80, zorder=42)
    else:
        ax.text(lx_leg+6, iy+0.4, sym, fontsize=7, color=col,
                ha='center', va='center', zorder=42)
    ax.text(lx_leg+12, iy+0.4, lbl, fontsize=5.2, color=INK_FADED,
            va='center', fontfamily='DejaVu Serif', zorder=42)

# ═══════════════════════════════════════════════════════════
#  SCALE BAR
# ═══════════════════════════════════════════════════════════
sb_x, sb_y = 198, 122
for xi, col_ in [(0, INK),(12.5, PARCH_LT),(12.5, INK)]:
    ax.add_patch(mpatches.Rectangle(
        (sb_x+xi, sb_y-1), 12.5, 2.5,
        color=col_, alpha=0.85 if col_ == INK else 0.70, zorder=41))
ax.add_patch(mpatches.Rectangle(
    (sb_x, sb_y-1.2), 25, 2.9,
    fill=False, edgecolor=INK, linewidth=0.8, alpha=0.75, zorder=42))
for tick_x, tick_lbl in [(sb_x,'0'),(sb_x+12.5,'50'),(sb_x+25,'100')]:
    ax.plot(tick_x, sb_y+1.45, '|', color=INK, markersize=4, alpha=0.75, zorder=42)
    ax.text(tick_x, sb_y-2.5, tick_lbl, fontsize=5.5, color=INK,
            ha='center', fontfamily='DejaVu Serif', zorder=43)
ax.text(sb_x+12.5, sb_y+3.0, 'LEAGUES', fontsize=5.0, color=INK_FADED,
        ha='center', style='italic', fontfamily='DejaVu Serif', zorder=43)

# ═══════════════════════════════════════════════════════════
#  COMPASS ROSE
# ═══════════════════════════════════════════════════════════
compass_rose(ax, cx=218, cy=26, size=14)

# ═══════════════════════════════════════════════════════════
#  BORDER & FOOTER
# ═══════════════════════════════════════════════════════════
decorative_border(ax)

ax.text(W/2, 7.8,
        'Cartographed by the Granite Arcade of Midgaard'
        '  \u2014  Third Age, Containment Era',
        fontsize=5.8, color=INK_FADED, ha='center', style='italic',
        fontfamily='DejaVu Serif', alpha=0.80, zorder=65)

# ═══════════════════════════════════════════════════════════
#  SAVE
# ═══════════════════════════════════════════════════════════
out = '/home/user/acktng/docs/world_map.png'
plt.savefig(out, dpi=180, bbox_inches='tight',
            facecolor='#2A1808', edgecolor='none')
plt.close()
print(f'Saved → {out}')
