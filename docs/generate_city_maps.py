#!/usr/bin/env python3
"""
ACK!MUD TNG — Fantasy City Map Generator
Antique parchment style, one map per city.
"""

import numpy as np
import math, random, shutil, os
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.patheffects as pe
import matplotlib.colors as mcolors
from matplotlib.path import Path
from matplotlib.patches import FancyBboxPatch, PathPatch
from scipy.interpolate import splprep, splev
from PIL import Image, ImageDraw, ImageFilter
import io

RNG  = random.Random(42)
NRNG = np.random.RandomState(42)

# ═══════════════════════════════════════════════════════════
#  PALETTE
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
FOREST_MID = '#2A6820'
FOREST_LT  = '#4A8830'
MTN_DARK   = '#2E2018'
MTN_MID    = '#524030'
BLOOD_RED  = '#8B1010'
VOID_COL   = '#120622'
VOID_LT    = '#341448'

CW, CH = 100, 80

DOCS_DIR = '/home/user/acktng/docs'
WEB_DIR  = '/home/user/acktng/web/img'

# ═══════════════════════════════════════════════════════════
#  PARCHMENT TEXTURE
# ═══════════════════════════════════════════════════════════
def make_parchment(w_px=1200, h_px=960, seed=42):
    lrng  = random.Random(seed)
    lnrng = np.random.RandomState(seed)
    img = Image.new('RGB', (w_px, h_px))
    pix = img.load()
    for y in range(h_px):
        for x in range(w_px):
            fx, fy = x / w_px, y / h_px
            vx = (fx - 0.5) * 2
            vy = (fy - 0.5) * 2
            vign = 1.0 - 0.55 * (vx*vx + vy*vy) ** 0.55
            diag = fx * 0.3 + fy * 0.7
            r = int((228 - 20*diag + 8*math.sin(fx*8)) * vign)
            g = int((195 - 28*diag + 5*math.cos(fy*6)) * vign)
            b = int((128 - 22*diag) * vign)
            pix[x, y] = (max(0,min(255,r)), max(0,min(255,g)), max(0,min(255,b)))
    base = np.array(img).astype(float)
    for scale, amp in [(8,18),(20,12),(55,8),(120,5)]:
        nw, nh = w_px//scale, h_px//scale
        noise = lnrng.uniform(-1, 1, (nh, nw, 3))
        noise_up = np.array(Image.fromarray(
            ((noise+1)*127).astype(np.uint8)
        ).resize((w_px, h_px), Image.BILINEAR)).astype(float) - 127
        base += noise_up * amp / 127
    base = np.clip(base, 0, 255).astype(np.uint8)
    img = Image.fromarray(base)
    draw = ImageDraw.Draw(img)
    for _ in range(120):
        sx = lrng.randint(0, w_px)
        sy = lrng.randint(0, h_px)
        sr = lrng.randint(8, 90)
        af = lrng.uniform(0.03, 0.18)
        sc = lrng.choice([(100,70,30),(80,55,20),(140,100,50),(60,40,10),(120,90,45)])
        px_col = img.getpixel((min(sx,w_px-1), min(sy,h_px-1)))
        draw.ellipse([sx-sr, sy-sr//2, sx+sr, sy+sr//2],
                     fill=tuple(int(c*af + p*(1-af)) for c,p in zip(sc, px_col)))
    img = img.filter(ImageFilter.GaussianBlur(radius=1.2))
    return np.array(img) / 255.0

# ═══════════════════════════════════════════════════════════
#  SPLINE HELPERS
# ═══════════════════════════════════════════════════════════
def smooth_polygon(pts, closed=True, tension=0.0, n=400):
    pts = np.array(pts, dtype=float)
    if closed:
        pts = np.vstack([pts, pts[:3]])
    x, y = pts[:,0], pts[:,1]
    mask = np.concatenate(([True], np.any(np.diff(pts, axis=0) != 0, axis=1)))
    pts  = pts[mask]
    x, y = pts[:,0], pts[:,1]
    k = min(3, len(x)-1)
    try:
        tck, _ = splprep([x, y], s=tension, k=k, per=False)
    except Exception:
        t_raw = np.linspace(0, 1, len(x))
        t = np.linspace(0, 1, n)
        return np.interp(t, t_raw, x), np.interp(t, t_raw, y)
    t = np.linspace(0, 1, n)
    return splev(t, tck)

def smooth_path_patch(pts, closed=True, tension=0.0, n=600, **kwargs):
    xs, ys = smooth_polygon(pts, closed=closed, tension=tension, n=n)
    verts = list(zip(xs, ys))
    if closed:
        verts.append(verts[0])
    codes = [Path.MOVETO] + [Path.LINETO]*(len(verts)-2) + [Path.CLOSEPOLY]
    return PathPatch(Path(verts, codes), **kwargs)

def smooth_line(pts, tension=0.5, n=300):
    pts = np.array(pts, dtype=float)
    x, y = pts[:,0], pts[:,1]
    k = min(3, len(x)-1)
    tck, _ = splprep([x, y], s=tension, k=k)
    return splev(np.linspace(0, 1, n), tck)

# ═══════════════════════════════════════════════════════════
#  DRAWING HELPERS
# ═══════════════════════════════════════════════════════════
def draw_city_wall(ax, pts, col=MTN_MID, lw=2.5, alpha=0.88, zorder=15):
    """Thick smoothed polygon wall with shadow and tower dots."""
    xs, ys = smooth_polygon(pts, closed=True, tension=2.0, n=500)
    verts = list(zip(xs, ys)) + [(xs[0], ys[0])]
    codes = [Path.MOVETO] + [Path.LINETO]*(len(verts)-2) + [Path.CLOSEPOLY]
    # Shadow
    shadow_verts = [(x+0.35, y-0.35) for x,y in verts]
    ax.add_patch(PathPatch(Path(shadow_verts, codes),
                           fill=False, edgecolor=MTN_DARK,
                           linewidth=lw+1.5, alpha=0.18, zorder=zorder-1))
    # Main wall
    ax.add_patch(PathPatch(Path(verts, codes),
                           fill=False, edgecolor=col,
                           linewidth=lw, alpha=alpha, zorder=zorder))
    # Tower squares at vertices
    for px, py in pts:
        s = 0.9
        ax.add_patch(mpatches.Rectangle((px-s/2, py-s/2), s, s,
                      color=col, alpha=alpha*0.9, zorder=zorder+1))

def draw_road(ax, pts, lw=1.8, road_col='#8B6A30', pale_col='#F0D880',
              zorder=7, tension=1.0):
    """Double-line road: thick dark under, thinner pale on top."""
    if len(pts) < 2:
        return
    try:
        xs, ys = smooth_line(pts, tension=tension, n=200)
    except Exception:
        arr = np.array(pts)
        xs, ys = arr[:,0], arr[:,1]
    ax.plot(xs, ys, color=road_col, linewidth=lw, alpha=0.72, zorder=zorder,
            solid_capstyle='round')
    ax.plot(xs, ys, color=pale_col, linewidth=lw*0.45, alpha=0.55, zorder=zorder+1,
            solid_capstyle='round')

def fill_district(ax, pts, color, alpha=0.35, edge_col=None, zorder=4):
    """Filled district region."""
    ec = edge_col if edge_col else color
    patch = smooth_path_patch(pts, tension=1.0,
                               facecolor=color, edgecolor=ec,
                               linewidth=0.7, alpha=alpha, zorder=zorder)
    ax.add_patch(patch)

def sea_fill(ax, pts, zorder=3):
    """Fill a sea/harbor region with shimmer lines."""
    patch = smooth_path_patch(pts, tension=2.0,
                               facecolor=SEA_DEEP, edgecolor=SEA_MID,
                               linewidth=1.0, alpha=0.82, zorder=zorder)
    ax.add_patch(patch)
    xs, ys = smooth_polygon(pts, closed=True, tension=2.0, n=500)
    xmin, xmax = min(xs), max(xs)
    ymin, ymax = min(ys), max(ys)
    p = Path(list(zip(xs,ys)) + [(xs[0],ys[0])],
             [Path.MOVETO]+[Path.LINETO]*(len(xs)-1)+[Path.CLOSEPOLY])
    for yi in np.arange(ymin+1, ymax, 2.2):
        lxs = np.linspace(xmin, xmax, 80)
        lys = yi + 0.4*np.sin(lxs*0.9 + yi*0.7)
        pts2 = list(zip(lxs, lys))
        visible = [pt for pt in pts2 if p.contains_point(pt)]
        if len(visible) > 2:
            vx2 = [v[0] for v in visible]
            vy2 = [v[1] for v in visible]
            ax.plot(vx2, vy2, color=SEA_LT, linewidth=0.35, alpha=0.25, zorder=zorder+1)

# ═══════════════════════════════════════════════════════════
#  SYMBOL FUNCTIONS
# ═══════════════════════════════════════════════════════════
def temple_sym(ax, cx, cy, size=1.4, col='#6A3878', zorder=12):
    """Rectangle body + triangle spire + cross."""
    bw, bh = size*1.4, size*1.0
    ax.add_patch(mpatches.Rectangle((cx-bw/2, cy), bw, bh,
                  color=col, alpha=0.85, zorder=zorder))
    ax.add_patch(plt.Polygon(
        [[cx-bw/2*1.2, cy+bh], [cx+bw/2*1.2, cy+bh], [cx, cy+bh+size*1.1]],
        color=col, alpha=0.90, zorder=zorder+1))
    ax.plot([cx, cx], [cy+bh+size*1.1, cy+bh+size*1.8],
            color=col, linewidth=1.2, alpha=0.85, zorder=zorder+2)
    ax.plot([cx-size*0.4, cx+size*0.4], [cy+bh+size*1.4, cy+bh+size*1.4],
            color=col, linewidth=1.0, alpha=0.80, zorder=zorder+2)
    ax.add_patch(mpatches.Rectangle((cx-bw/2, cy-size*0.18), bw, size*0.18,
                  color=col, alpha=0.70, zorder=zorder))

def keep_sym(ax, cx, cy, size=1.5, col=INK, zorder=12):
    """Main block + spire + corner turrets."""
    bw, bh = size*1.6, size*1.2
    ax.add_patch(mpatches.Rectangle((cx-bw/2, cy), bw, bh,
                  color=col, alpha=0.88, zorder=zorder))
    ax.add_patch(plt.Polygon(
        [[cx-size*0.35, cy+bh], [cx+size*0.35, cy+bh], [cx, cy+bh+size*0.9]],
        color=col, alpha=0.92, zorder=zorder+1))
    for tx in [cx-bw/2, cx+bw/2-size*0.4]:
        ax.add_patch(mpatches.Rectangle((tx, cy-size*0.1), size*0.4, bh+size*0.2,
                      color=col, alpha=0.78, zorder=zorder+1))
        for btx in [tx, tx+size*0.15]:
            ax.add_patch(mpatches.Rectangle((btx, cy+bh+size*0.1), size*0.12, size*0.22,
                          color=col, alpha=0.88, zorder=zorder+2))
    ax.add_patch(mpatches.Rectangle((cx-size*0.15, cy), size*0.30, size*0.45,
                  color=PARCH_PALE, alpha=0.75, zorder=zorder+2))

def archive_sym(ax, cx, cy, size=1.2, col='#4A3020', zorder=12):
    """Columned building / archive."""
    bw, bh = size*1.8, size*0.9
    ax.add_patch(mpatches.Rectangle((cx-bw/2, cy), bw, bh,
                  color=col, alpha=0.80, zorder=zorder))
    ax.add_patch(mpatches.Rectangle((cx-bw/2*1.1, cy+bh), bw*1.1, size*0.18,
                  color=col, alpha=0.85, zorder=zorder+1))
    ax.add_patch(mpatches.Rectangle((cx-bw/2*1.15, cy-size*0.14), bw*1.15, size*0.14,
                  color=col, alpha=0.80, zorder=zorder))
    for ci, cx2 in enumerate(np.linspace(cx-bw/2+size*0.2, cx+bw/2-size*0.2, 4)):
        ax.add_patch(mpatches.Rectangle((cx2-size*0.07, cy), size*0.14, bh,
                      color=PARCH_PALE, alpha=0.35, zorder=zorder+1))

def prism_sym(ax, cx, cy, size=1.8, zorder=12):
    """Crystal prism with glow for Kiess Central Prism."""
    for gr, ga in [(size*1.8, 0.08), (size*1.2, 0.14), (size*0.7, 0.22)]:
        ax.add_patch(plt.Circle((cx, cy+size*0.8), gr,
                                 color='#A0D0FF', alpha=ga, zorder=zorder-1))
    hw = size*0.55
    ax.add_patch(plt.Polygon(
        [[cx-hw, cy], [cx+hw, cy], [cx+hw*0.5, cy+size*2.0], [cx-hw*0.5, cy+size*2.0]],
        color='#C8E8FF', alpha=0.60, zorder=zorder))
    ax.add_patch(plt.Polygon(
        [[cx-hw, cy], [cx-hw*0.5, cy+size*2.0], [cx, cy+size*2.2]],
        color='#88C4F8', alpha=0.72, zorder=zorder+1))
    ax.add_patch(plt.Polygon(
        [[cx+hw, cy], [cx+hw*0.5, cy+size*2.0], [cx, cy+size*2.2]],
        color='#4090D0', alpha=0.72, zorder=zorder+1))
    ax.add_patch(plt.Polygon(
        [[cx-hw, cy], [cx+hw, cy], [cx+hw*0.5, cy+size*2.0], [cx-hw*0.5, cy+size*2.0]],
        fill=False, edgecolor='#2060A8', linewidth=0.8, alpha=0.85, zorder=zorder+2))
    for ray_ang in np.linspace(0, 2*math.pi, 7)[:-1]:
        rx = cx + size*2.0*math.cos(ray_ang)
        ry = (cy+size*0.8) + size*2.0*math.sin(ray_ang)
        ax.plot([cx, rx], [cy+size*0.8, ry],
                color='#80C0FF', linewidth=0.4, alpha=0.28, zorder=zorder-1)
    ax.plot(cx, cy+size*2.2, '*', color='#FFFFFF', markersize=6,
            alpha=0.90, zorder=zorder+3)

def pagoda_sym(ax, cx, cy, size=1.8, zorder=12):
    """Multi-tier pagoda for Rakuen Bloom Pavilion."""
    tier_data = [
        (size*1.4, size*0.32, '#7A4A20'),
        (size*1.0, size*0.30, '#8A5428'),
        (size*0.65, size*0.28, '#9A5E30'),
        (size*0.35, size*0.26, '#AA6838'),
    ]
    base_y = cy
    for tw, th, tc in tier_data:
        ax.add_patch(mpatches.Rectangle((cx-tw/2, base_y), tw, th,
                      color=tc, alpha=0.88, zorder=zorder))
        eave_col = '#C89040'
        ax.plot([cx-tw/2-size*0.18, cx+tw/2+size*0.18],
                [base_y+th, base_y+th],
                color=eave_col, linewidth=1.5, alpha=0.88, zorder=zorder+1)
        ax.plot([cx-tw/2-size*0.10, cx-tw/2-size*0.18],
                [base_y+th-size*0.08, base_y+th],
                color=eave_col, linewidth=1.0, alpha=0.75, zorder=zorder+1)
        ax.plot([cx+tw/2+size*0.10, cx+tw/2+size*0.18],
                [base_y+th-size*0.08, base_y+th],
                color=eave_col, linewidth=1.0, alpha=0.75, zorder=zorder+1)
        base_y += th
    ax.add_patch(plt.Polygon(
        [[cx-size*0.20, base_y], [cx+size*0.20, base_y], [cx, base_y+size*0.65]],
        color='#C89040', alpha=0.92, zorder=zorder+2))
    ax.plot(cx, base_y+size*0.65, 'o', color=GOLD_LT, markersize=4,
            alpha=0.95, zorder=zorder+3)
    for bl_ang in np.linspace(0, 2*math.pi, 7)[:-1]:
        bx = cx + size*1.8*math.cos(bl_ang)
        by_b = cy - size*0.3 + size*0.8*math.sin(bl_ang)
        ax.plot(bx, by_b, '.', color='#E8A850', markersize=3.5,
                alpha=0.55, zorder=zorder-1)

def claw_shrine_sym(ax, cx, cy, size=1.8, zorder=12):
    """Elevated spire with claw marks for Mafdet Shrine of First Claw."""
    bw, bh = size*1.2, size*0.6
    ax.add_patch(mpatches.Rectangle((cx-bw/2, cy), bw, bh,
                  color='#8B5530', alpha=0.88, zorder=zorder))
    ax.add_patch(plt.Polygon(
        [[cx-bw/2*0.9, cy+bh], [cx+bw/2*0.9, cy+bh], [cx, cy+bh+size*1.8]],
        color='#B06030', alpha=0.90, zorder=zorder+1))
    ax.add_patch(plt.Polygon(
        [[cx-bw/2*0.9, cy+bh], [cx+bw/2*0.9, cy+bh], [cx, cy+bh+size*1.8]],
        fill=False, edgecolor=BLOOD_RED, linewidth=0.9, alpha=0.75, zorder=zorder+2))
    ax.plot(cx, cy+bh+size*1.8, 'o', color=BLOOD_RED, markersize=4.5,
            alpha=0.90, zorder=zorder+3)
    for cl_i, (clx, cly, cang) in enumerate([
        (cx-size*0.3, cy+bh+size*0.7,  0.3),
        (cx,          cy+bh+size*1.0, -0.1),
        (cx+size*0.3, cy+bh+size*0.8,  -0.3),
    ]):
        for clen in [size*0.5, size*0.38, size*0.28]:
            ex = clx + clen*math.cos(math.pi/2 + cang + (cl_i-1)*0.15)
            ey = cly + clen*math.sin(math.pi/2 + cang + (cl_i-1)*0.15)*0.6
            ax.plot([clx, ex], [cly, ey],
                    color=BLOOD_RED, linewidth=0.8, alpha=0.65, zorder=zorder+2)
    ax.add_patch(mpatches.Rectangle((cx-bw/2*1.3, cy-size*0.25), bw*1.3, size*0.25,
                  color='#7A4828', alpha=0.85, zorder=zorder))

def anchor_sym(ax, cx, cy, size=1.5, col='#2A5580', zorder=12):
    """Anchor symbol for harbors."""
    ax.plot([cx, cx], [cy-size*0.9, cy+size*0.9],
            color=col, linewidth=1.8, alpha=0.85, zorder=zorder)
    ax.add_patch(mpatches.Arc((cx, cy+size*0.5), size*0.7, size*0.7,
                               angle=0, theta1=0, theta2=180,
                               color=col, linewidth=1.5, alpha=0.82, zorder=zorder))
    ax.plot([cx-size*0.55, cx+size*0.55], [cy-size*0.9, cy-size*0.9],
            color=col, linewidth=1.5, alpha=0.82, zorder=zorder)
    ax.plot([cx-size*0.55, cx-size*0.35], [cy-size*0.9, cy-size*0.65],
            color=col, linewidth=1.0, alpha=0.70, zorder=zorder)
    ax.plot([cx+size*0.55, cx+size*0.35], [cy-size*0.9, cy-size*0.65],
            color=col, linewidth=1.0, alpha=0.70, zorder=zorder)
    ax.plot([cx-size*0.38, cx+size*0.38], [cy+size*0.0, cy+size*0.0],
            color=col, linewidth=1.2, alpha=0.80, zorder=zorder)

def lantern_sym(ax, cx, cy, size=1.2, col='#C89020', zorder=12):
    """Glowing lantern."""
    for gr, ga in [(size*1.0, 0.12), (size*0.6, 0.20)]:
        ax.add_patch(plt.Circle((cx, cy+size*0.4), gr,
                                 color=GOLD_LT, alpha=ga, zorder=zorder-1))
    ax.add_patch(mpatches.FancyBboxPatch((cx-size*0.3, cy), size*0.6, size*0.8,
                  boxstyle='round,pad=0.05',
                  color=col, alpha=0.85, zorder=zorder))
    ax.plot([cx, cx], [cy+size*0.8, cy+size*1.1],
            color=col, linewidth=1.2, alpha=0.85, zorder=zorder+1)
    ax.add_patch(mpatches.Rectangle((cx-size*0.38, cy+size*0.75), size*0.76, size*0.15,
                  color=col, alpha=0.80, zorder=zorder+1))
    ax.plot(cx, cy+size*0.4, 'o', color=GOLD_LT, markersize=3.5,
            alpha=0.90, zorder=zorder+2)

def dispatch_sym(ax, cx, cy, size=1.0, zorder=12):
    """Stilted platform / dispatch house for Rakuen."""
    pw = size*1.6
    ax.add_patch(mpatches.Rectangle((cx-pw/2, cy+size*0.5), pw, size*0.7,
                  color='#7A5030', alpha=0.85, zorder=zorder))
    ax.add_patch(mpatches.Rectangle((cx-pw/2*1.1, cy+size*0.5+size*0.7), pw*1.1, size*0.12,
                  color='#9A6840', alpha=0.82, zorder=zorder+1))
    for stx in [cx-pw/2+size*0.15, cx, cx+pw/2-size*0.15]:
        ax.plot([stx, stx], [cy, cy+size*0.5],
                color='#5A3818', linewidth=1.2, alpha=0.82, zorder=zorder)
    ax.add_patch(plt.Polygon(
        [[cx-pw/2*0.9, cy+size*1.2], [cx+pw/2*0.9, cy+size*1.2],
         [cx, cy+size*1.75]],
        color='#6A4020', alpha=0.80, zorder=zorder+1))

def gate_arch_sym(ax, cx, cy, size=1.2, col=MTN_MID, zorder=14):
    """Gate arch for city gates."""
    gw, gh = size*1.4, size*1.6
    ax.add_patch(mpatches.Rectangle((cx-gw/2, cy), gw/3, gh,
                  color=col, alpha=0.85, zorder=zorder))
    ax.add_patch(mpatches.Rectangle((cx+gw/6, cy), gw/3, gh,
                  color=col, alpha=0.85, zorder=zorder))
    ax.add_patch(mpatches.Arc((cx, cy+gh*0.6), gw*0.67, gh*0.7,
                               angle=0, theta1=0, theta2=180,
                               color=col, linewidth=2.0, alpha=0.88, zorder=zorder+1))
    ax.add_patch(mpatches.Rectangle((cx-gw/2*1.15, cy+gh-size*0.05), gw*1.15, size*0.22,
                  color=col, alpha=0.78, zorder=zorder+1))

def hearth_sym(ax, cx, cy, size=1.2, zorder=12):
    """Flame/hearth symbol."""
    ax.add_patch(mpatches.Rectangle((cx-size*0.6, cy), size*1.2, size*0.5,
                  color='#604028', alpha=0.85, zorder=zorder))
    for fx, fh, fc in [(cx-size*0.25, size*1.1, '#D04010'),
                        (cx, size*1.4, '#E06020'),
                        (cx+size*0.22, size*0.9, '#C83010')]:
        ax.add_patch(plt.Polygon(
            [[fx-size*0.18, cy+size*0.5],
             [fx+size*0.18, cy+size*0.5],
             [fx, cy+size*0.5+fh]],
            color=fc, alpha=0.80, zorder=zorder+1))
    ax.plot(cx, cy+size*0.5+size*0.35, 'o', color=GOLD_LT,
            markersize=2.5, alpha=0.85, zorder=zorder+2)

def market_square_sym(ax, cx, cy, size=2.0, col='#9B7040', zorder=8):
    """Paved market square with cobble pattern."""
    ax.add_patch(mpatches.Rectangle((cx-size, cy-size*0.6), size*2, size*1.2,
                  color=col, alpha=0.55, zorder=zorder))
    ax.add_patch(mpatches.Rectangle((cx-size, cy-size*0.6), size*2, size*1.2,
                  fill=False, edgecolor=INK_FADED, linewidth=0.6, alpha=0.45, zorder=zorder+1))
    for row in np.arange(cy-size*0.6+0.4, cy+size*0.6, 0.8):
        for col2 in np.arange(cx-size+0.4, cx+size, 0.9):
            ax.add_patch(mpatches.Rectangle((col2-0.3, row-0.25), 0.6, 0.5,
                          color=INK_FADED, alpha=0.12, zorder=zorder+1))

# ═══════════════════════════════════════════════════════════
#  SHARED COMPONENTS
# ═══════════════════════════════════════════════════════════
def compass_rose(ax, cx, cy, size=5, zorder=32):
    """Ornate compass rose, same style as world map."""
    for r, lw, col, alpha in [(size*1.45, 1.8, INK, 0.65),
                               (size*1.30, 0.7, GOLD, 0.55)]:
        ax.add_patch(plt.Circle((cx,cy), r, fill=False,
                                 edgecolor=col, linewidth=lw,
                                 alpha=alpha, zorder=zorder))
    ax.add_patch(plt.Circle((cx,cy), size*1.45,
                             color=PARCH_PALE, alpha=0.80, zorder=zorder-1))
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
        pcol = GOLD if cardinal else INK_FADED
        ax.add_patch(plt.Polygon([[tip_x,tip_y],[bx1,by1],[bx2,by2]],
                                  color=pcol, alpha=0.92, zorder=zorder+1))
    for lbl, ang in [('N',0),('S',180),('E',90),('W',270)]:
        lx = cx + (size*1.65)*math.sin(math.radians(ang))
        ly = cy + (size*1.65)*math.cos(math.radians(ang))
        ax.text(lx, ly, lbl, fontsize=6.5, fontweight='bold',
                color=INK, ha='center', va='center',
                fontfamily='DejaVu Serif', zorder=zorder+2)
    ax.add_patch(plt.Circle((cx,cy), size*0.14,
                             color=GOLD_LT, alpha=0.95, zorder=zorder+3))
    ax.add_patch(plt.Circle((cx,cy), size*0.07,
                             color=BLOOD_RED, alpha=0.90, zorder=zorder+4))

def decorative_border(ax, w=CW, h=CH):
    """Layered ornate border."""
    m = 2.5
    bw = 3.8
    for rx, ry, rw, rh in [
        (0, 0, w, bw),
        (0, h-bw, w, bw),
        (0, 0, bw, h),
        (w-bw, 0, bw, h),
    ]:
        ax.add_patch(mpatches.Rectangle((rx, ry), rw, rh,
                      color=PARCH_DARK, alpha=0.92, zorder=55))
    for offset, lw, col, alpha in [
        (m,       1.8, INK,       0.88),
        (m+1.4,   0.65, GOLD,      0.70),
        (m+2.3,   0.40, INK_FADED, 0.50),
    ]:
        ax.add_patch(mpatches.FancyBboxPatch(
            (offset, offset), w-2*offset, h-2*offset,
            boxstyle='square,pad=0', fill=False,
            edgecolor=col, linewidth=lw, alpha=alpha, zorder=56))
    for cx2, cy2 in [(m+1.2,m+1.2),(w-m-1.2,m+1.2),(m+1.2,h-m-1.2),(w-m-1.2,h-m-1.2)]:
        for r, al in [(3.5,0.15),(2.2,0.28),(1.4,0.50)]:
            ax.add_patch(plt.Circle((cx2,cy2), r, color=GOLD, alpha=al, zorder=57))
        ax.plot(cx2, cy2, '*', color=GOLD_LT, markersize=7, alpha=0.90, zorder=58)
    for xi in np.linspace(10, w-10, 22):
        for yi in [m+0.8, h-m-0.8]:
            ax.plot(xi, yi, 'D', color=GOLD, markersize=1.2, alpha=0.50, zorder=57)
    for yi in np.linspace(10, h-10, 18):
        for xi in [m+0.8, w-m-0.8]:
            ax.plot(xi, yi, 'D', color=GOLD, markersize=1.2, alpha=0.50, zorder=57)

def map_label(ax, x, y, text, size=6, col=INK, style='normal', weight='normal',
              ha='center', va='center', zorder=22):
    """Double-draw text with parchment outline stroke."""
    ax.text(x, y, text, fontsize=size, color=col, style=style,
            fontweight=weight, ha=ha, va=va, fontfamily='DejaVu Serif',
            zorder=zorder-1,
            path_effects=[pe.withStroke(linewidth=2.5, foreground=PARCH_PALE)])
    ax.text(x, y, text, fontsize=size, color=col, style=style,
            fontweight=weight, ha=ha, va=va, fontfamily='DejaVu Serif',
            zorder=zorder)

def district_label(ax, x, y, text, size=5.5, col=INK_FADED):
    """Italic district name."""
    map_label(ax, x, y, text, size=size, col=col, style='italic', zorder=21)

def landmark_label(ax, x, y, text, size=4.8, col=INK):
    """Small label below a symbol."""
    map_label(ax, x, y, text, size=size, col=col, zorder=22)

def gate_label(ax, x, y, text, direction='N', size=5.5, col=INK):
    """Gate name with direction offset."""
    offsets = {'N':(0, 1.5), 'S':(0,-1.5), 'E':(2.0,0), 'W':(-2.0,0)}
    ox, oy = offsets.get(direction, (0,0))
    map_label(ax, x+ox, y+oy, text, size=size, col=col, weight='bold', zorder=23)

def title_label(ax, text, w=CW, h=CH):
    """City title at top center."""
    map_label(ax, w/2, h-2.0, text, size=9, col=INK, weight='bold', zorder=25)

# ═══════════════════════════════════════════════════════════
#  FIGURE FACTORY
# ═══════════════════════════════════════════════════════════
def make_city_fig(seed=42):
    lnrng = np.random.RandomState(seed)
    fig = plt.figure(figsize=(13, 10.4), dpi=180)
    ax  = fig.add_axes([0, 0, 1, 1])
    ax.set_xlim(0, CW)
    ax.set_ylim(0, CH)
    ax.set_aspect('equal')
    ax.axis('off')
    parch = make_parchment(w_px=1200, h_px=960, seed=seed)
    ax.imshow(parch, extent=[0, CW, 0, CH], origin='upper',
              aspect='auto', zorder=0, alpha=1.0)
    vx = np.linspace(-1, 1, 300)
    vy = np.linspace(-1, 1, 240)
    VX, VY = np.meshgrid(vx, vy)
    vign = np.clip((VX**2 + VY**2)**0.55 * 0.55, 0, 1)
    ax.imshow(vign, extent=[0, CW, 0, CH], origin='upper', aspect='auto',
              cmap='Greys', alpha=0.28, zorder=1)
    return fig, ax

def save_map(fig, city_name):
    docs_path = os.path.join(DOCS_DIR, f'{city_name}_map.png')
    web_path  = os.path.join(WEB_DIR,  f'{city_name}_map.png')
    fig.savefig(docs_path, dpi=180, bbox_inches='tight', pad_inches=0,
                facecolor='none')
    shutil.copy2(docs_path, web_path)
    plt.close(fig)
    print(f'  Saved {city_name}_map.png')

# ═══════════════════════════════════════════════════════════
#  MIDGAARD
# ═══════════════════════════════════════════════════════════
def draw_midgaard():
    print('Drawing Midgaard...')
    fig, ax = make_city_fig(seed=10)

    # Outer ground / surroundings
    ax.add_patch(mpatches.Rectangle((0, 0), CW, CH,
                  color='#C4A870', alpha=0.30, zorder=2))

    # Octagonal wall polygon
    wall_pts = [
        (28, 14), (38,  8), (62,  8), (72, 14),
        (78, 28), (78, 52), (72, 58), (62, 64),
        (38, 64), (28, 58), (22, 52), (22, 28),
    ]

    # District fills (inside wall)
    fill_district(ax, [(38,48),(50,48),(54,64),(38,64),(28,58),(28,52)],
                  '#B09060', alpha=0.45, zorder=3)  # Granite Arcade N
    fill_district(ax, [(54,48),(72,52),(72,44),(60,36),(54,36)],
                  '#C8A840', alpha=0.40, zorder=3)  # Temple Quarter NE
    fill_district(ax, [(60,36),(72,44),(78,36),(72,28),(64,28)],
                  '#6878A0', alpha=0.35, zorder=3)  # Archive Quarter E
    fill_district(ax, [(36,28),(60,28),(58,20),(40,20),(34,22)],
                  '#C89028', alpha=0.42, zorder=3)  # Market District center
    fill_district(ax, [(22,28),(36,28),(34,22),(28,14),(22,28)],
                  '#4A7838', alpha=0.38, zorder=3)  # Guild Quarter W
    fill_district(ax, [(38,8),(54,8),(56,22),(40,20),(38,14)],
                  '#604840', alpha=0.40, zorder=3)  # Intake District S
    fill_district(ax, [(22,28),(36,28),(38,14),(28,14)],
                  '#585858', alpha=0.38, zorder=3)  # Guard Quarter SW

    # Roads: N-S Main Avenue and E-W Arcana Strip
    draw_road(ax, [(50, 8),(50,64)], lw=2.0, zorder=7)
    draw_road(ax, [(22,36),(78,36)], lw=2.0, zorder=7)
    draw_road(ax, [(36,8),(28,14),(22,28)], lw=1.3, zorder=6)
    draw_road(ax, [(64,8),(72,14),(78,28)], lw=1.3, zorder=6)
    draw_road(ax, [(22,52),(28,58),(38,64)], lw=1.3, zorder=6)
    draw_road(ax, [(78,52),(72,58),(62,64)], lw=1.3, zorder=6)

    # City wall
    draw_city_wall(ax, wall_pts, col='#907050', lw=2.8, alpha=0.90, zorder=15)

    # Gates
    for gx, gy, gd, glabel in [
        (50, 64, 'N', 'North Gate'),
        (50,  8, 'S', 'South Wild Gate'),
        (78, 36, 'E', 'East Gate'),
        (22, 36, 'W', 'West Gate'),
    ]:
        ax.plot(gx, gy, 's', color='#706050', markersize=8, zorder=16, alpha=0.85)
        ax.plot(gx, gy, 's', color=PARCH_PALE, markersize=4, zorder=17, alpha=0.75)
        gate_label(ax, gx, gy, glabel, direction=gd, size=4.5)

    # Landmarks
    temple_sym(ax, 62, 48, size=1.6, col='#8848A0', zorder=12)
    landmark_label(ax, 62, 44.5, 'Temple of\nResounding Heart', size=4.2)

    keep_sym(ax, 30, 22, size=1.4, col='#404040', zorder=12)
    landmark_label(ax, 30, 19.5, 'Guard Command HQ', size=4.2)

    archive_sym(ax, 50, 52, size=1.2, col='#504070', zorder=12)
    landmark_label(ax, 50, 49.5, 'Magistrate\nLedgerhouse', size=4.2)

    archive_sym(ax, 66, 52, size=1.0, col='#4A5060', zorder=12)
    landmark_label(ax, 66, 50.2, 'Lantern Registry', size=4.2)

    keep_sym(ax, 30, 38, size=1.3, col='#3A5030', zorder=12)
    landmark_label(ax, 30, 36, 'Guild Concordat\nHalls', size=4.2)

    market_square_sym(ax, 50, 36, size=2.8, zorder=8)
    district_label(ax, 50, 36, 'Market District', size=5.0)

    # Violet Reliquary Arch — center, violet glow
    for gr, ga in [(3.5, 0.08), (2.0, 0.15), (1.0, 0.25)]:
        ax.add_patch(plt.Circle((50, 36), gr,
                                 color='#8840C0', alpha=ga, zorder=10))
    ax.add_patch(mpatches.Arc((50, 36), 4.0, 4.0,
                               angle=0, theta1=0, theta2=180,
                               color='#9050D0', linewidth=1.8, alpha=0.80, zorder=11))
    ax.plot(50, 36, 'o', color='#C080FF', markersize=5, alpha=0.90, zorder=12)
    landmark_label(ax, 50, 33, 'Violet Reliquary Arch', size=4.2)

    # District labels
    district_label(ax, 44, 56, 'Granite Arcade')
    district_label(ax, 62, 55, 'Temple Quarter')
    district_label(ax, 68, 42, 'Archive Quarter')
    district_label(ax, 36, 22, 'Intake District')
    district_label(ax, 26, 44, 'Guild Quarter')
    district_label(ax, 32, 30, 'Guard Quarter')

    # Compass + border
    compass_rose(ax, cx=8, cy=8, size=4.5, zorder=32)
    decorative_border(ax)
    title_label(ax, 'MIDGAARD')
    map_label(ax, CW/2, 4.5, 'Great Administrative City of the Heartlands',
              size=5.2, col=INK_FADED, style='italic')

    save_map(fig, 'midgaard')

# ═══════════════════════════════════════════════════════════
#  KIESS
# ═══════════════════════════════════════════════════════════
def draw_kiess():
    print('Drawing Kiess...')
    fig, ax = make_city_fig(seed=20)

    # Surroundings
    ax.add_patch(mpatches.Rectangle((0, 0), CW, CH,
                  color='#B8C4D0', alpha=0.22, zorder=2))

    # Rectangular wall
    wall_pts = [
        (18, 12), (82, 12), (82, 68), (18, 68)
    ]

    # Districts
    fill_district(ax, [(18,50),(50,50),(50,68),(18,68)],
                  '#B0C8E0', alpha=0.40, zorder=3)  # Council Ring N
    fill_district(ax, [(50,50),(82,50),(82,68),(62,68),(50,60)],
                  '#D4C870', alpha=0.40, zorder=3)  # Temple of Concord NE
    fill_district(ax, [(62,28),(82,28),(82,50),(50,50),(50,38),(62,38)],
                  '#C8A860', alpha=0.38, zorder=3)  # Trade Syndics SE
    fill_district(ax, [(18,12),(50,12),(50,28),(18,28)],
                  '#B89858', alpha=0.38, zorder=3)  # Warehouse District S
    fill_district(ax, [(18,28),(36,28),(36,50),(18,50)],
                  '#909898', alpha=0.38, zorder=3)  # Wall Command W

    # Grid streets
    draw_road(ax, [(50,12),(50,68)], lw=2.0, zorder=7)
    draw_road(ax, [(18,40),(82,40)], lw=2.0, zorder=7)
    draw_road(ax, [(18,28),(82,28)], lw=1.4, zorder=6)
    draw_road(ax, [(18,50),(82,50)], lw=1.4, zorder=6)
    draw_road(ax, [(36,12),(36,68)], lw=1.4, zorder=6)
    draw_road(ax, [(64,12),(64,68)], lw=1.4, zorder=6)

    # Wall (rectangular, more rigid)
    draw_city_wall(ax, wall_pts, col='#8898A8', lw=3.0, alpha=0.90, zorder=15)

    # Memorial waystones along interior walls
    for wx, wy in [(22,20),(22,36),(22,52),(22,62),(78,20),(78,36),(78,52),(78,62),
                   (32,14),(50,14),(66,14),(32,66),(50,66),(66,66)]:
        ax.plot(wx, wy, 'o', color='#8898C8', markersize=3.5, alpha=0.65, zorder=13)
        ax.plot(wx, wy, 'o', color=PARCH_PALE, markersize=1.8, alpha=0.55, zorder=14)

    # Gates
    for gx, gy, gd, glabel in [
        (50, 68, 'N', 'North Gate'),
        (50, 12, 'S', 'South Gate'),
        (82, 40, 'E', 'East Gate\n(Roc Road)'),
        (18, 40, 'W', 'West Gate'),
    ]:
        ax.plot(gx, gy, 's', color='#6878A0', markersize=9, zorder=16, alpha=0.85)
        ax.plot(gx, gy, 's', color=PARCH_PALE, markersize=4.5, zorder=17, alpha=0.75)
        gate_label(ax, gx, gy, glabel, direction=gd, size=4.5)

    # Central Prism
    prism_sym(ax, 50, 34, size=1.8, zorder=12)
    landmark_label(ax, 50, 31, 'Central Prism', size=4.5)

    # Temple of Concord
    temple_sym(ax, 68, 58, size=1.5, col='#9898C0', zorder=12)
    landmark_label(ax, 68, 55, 'Temple of Concord', size=4.2)

    # Trade Syndics Annex
    archive_sym(ax, 70, 40, size=1.1, col='#9A7840', zorder=12)
    landmark_label(ax, 70, 37.8, 'Trade Syndics Annex', size=4.2)

    # Wall Command HQ
    keep_sym(ax, 26, 40, size=1.3, col='#606870', zorder=12)
    landmark_label(ax, 26, 37.8, 'Wall Command HQ', size=4.2)

    # District labels
    district_label(ax, 34, 59, 'Council Ring')
    district_label(ax, 63, 59, 'Temple of Concord')
    district_label(ax, 68, 44, 'Trade Syndics Quarter')
    district_label(ax, 36, 20, 'Warehouse District')
    district_label(ax, 26, 44, 'Wall Command')

    compass_rose(ax, cx=8, cy=8, size=4.5, zorder=32)
    decorative_border(ax)
    title_label(ax, 'KIESS')
    map_label(ax, CW/2, 4.5, 'Fortified Trade City — Built on Evermeet Ruins',
              size=5.2, col=INK_FADED, style='italic')

    save_map(fig, 'kiess')

# ═══════════════════════════════════════════════════════════
#  KOWLOON
# ═══════════════════════════════════════════════════════════
def draw_kowloon():
    print('Drawing Kowloon...')
    fig, ax = make_city_fig(seed=30)

    # Coppersalt Bay — east side harbor
    harbor_pts = [
        (70, 8), (85, 8), (92, 14), (95, 25), (96, 40),
        (95, 54), (90, 62), (82, 68), (72, 70), (70, 70),
        (70, 8)
    ]
    sea_fill(ax, harbor_pts, zorder=3)

    # Breakwaters
    ax.plot([80, 92, 95], [65, 60, 50], color=MTN_MID, linewidth=2.2, alpha=0.75, zorder=9)
    ax.plot([80, 88, 90], [14, 18, 28], color=MTN_MID, linewidth=2.2, alpha=0.75, zorder=9)

    # Pier/quay line
    ax.plot([70, 70], [10, 68], color='#604030', linewidth=2.5, alpha=0.80, zorder=9)
    for py in range(15, 65, 6):
        ax.plot([70, 78], [py, py], color='#604030', linewidth=0.8, alpha=0.55, zorder=9)

    # Surroundings
    ax.add_patch(mpatches.Rectangle((0, 0), 70, CH,
                  color='#3A3028', alpha=0.22, zorder=2))

    # Irregular city wall (coastal left side open to harbor)
    wall_pts = [
        (14, 12), (50, 10), (70, 10),
        (70, 70), (50, 70), (14, 70),
        (10, 60), (8, 40), (10, 20),
    ]
    draw_city_wall(ax, wall_pts, col='#504038', lw=2.8, alpha=0.88, zorder=15)

    # Districts
    fill_district(ax, [(14,50),(50,50),(50,70),(14,70),(10,60)],
                  '#2A6840', alpha=0.38, zorder=3)  # Jade Quarter N
    fill_district(ax, [(8,25),(36,25),(36,50),(14,50),(10,42)],
                  '#C89028', alpha=0.38, zorder=3)  # Lantern Quarter W
    fill_district(ax, [(14,10),(50,10),(50,25),(14,25),(10,20)],
                  '#383840', alpha=0.42, zorder=3)  # Iron Quarter S
    fill_district(ax, [(50,10),(70,10),(70,70),(50,70),(50,10)],
                  '#2A4868', alpha=0.40, zorder=3)  # Harbor Quarter E

    # Streets
    draw_road(ax, [(36,10),(36,70)], lw=1.8, zorder=7)
    draw_road(ax, [(8,40),(70,40)], lw=1.8, zorder=7)
    draw_road(ax, [(14,25),(70,25)], lw=1.3, zorder=6)
    draw_road(ax, [(14,55),(70,55)], lw=1.3, zorder=6)
    draw_road(ax, [(22,10),(22,70)], lw=1.3, zorder=6)
    draw_road(ax, [(52,10),(52,70)], lw=1.3, zorder=6)

    # Gates
    for gx, gy, gd, glabel, gcol in [
        (36, 70, 'N', 'Jade Gate',   '#2A8040'),
        (36, 10, 'S', 'Iron Gate',   '#404040'),
        (8,  40, 'W', 'Lantern Gate','#C89028'),
        (70, 40, 'E', 'Tide Gate',   '#2A5580'),
    ]:
        ax.plot(gx, gy, 's', color=gcol, markersize=9, zorder=16, alpha=0.85)
        ax.plot(gx, gy, 's', color=PARCH_PALE, markersize=4.5, zorder=17, alpha=0.75)
        gate_label(ax, gx, gy, glabel, direction=gd, size=4.5)

    # Crimson-Jade Execution Square
    ax.add_patch(mpatches.Rectangle((32, 36), 8, 8,
                  color='#603828', alpha=0.60, zorder=8))
    ax.add_patch(mpatches.Rectangle((32, 36), 8, 8,
                  fill=False, edgecolor=BLOOD_RED, linewidth=1.0,
                  alpha=0.65, zorder=9))
    ax.plot(36, 40, 'o', color=BLOOD_RED, markersize=5, alpha=0.85, zorder=10)
    ax.plot(36, 40, 'o', color='#40A840', markersize=2.5, alpha=0.75, zorder=11)
    landmark_label(ax, 36, 34.2, 'Crimson-Jade\nExecution Square', size=4.2)

    # Jade Magistracy
    archive_sym(ax, 30, 62, size=1.2, col='#286838', zorder=12)
    landmark_label(ax, 30, 59.8, 'Jade Magistracy', size=4.2)

    # Courier Lantern Office
    lantern_sym(ax, 16, 42, size=1.2, col='#C89028', zorder=12)
    landmark_label(ax, 16, 39.5, 'Courier Lantern\nOffice', size=4.2)

    # Temple Circle
    for tang in np.linspace(0, 2*math.pi, 5)[:-1]:
        tx = 42 + 3.5*math.cos(tang)
        ty = 60 + 3.5*math.sin(tang)
        ax.add_patch(plt.Circle((tx, ty), 1.0, color='#9870C0',
                                 alpha=0.65, zorder=12))
    ax.add_patch(plt.Circle((42, 60), 1.5, fill=False,
                             edgecolor='#7050A0', linewidth=0.8,
                             alpha=0.70, zorder=12))
    landmark_label(ax, 42, 56, 'Temple Circle', size=4.2)

    # Harbor Syndics
    anchor_sym(ax, 62, 40, size=1.5, col='#2A5580', zorder=12)
    landmark_label(ax, 62, 37, 'Harbor Syndics', size=4.2)

    # Wardens of Iron barracks
    keep_sym(ax, 26, 18, size=1.3, col='#383838', zorder=12)
    landmark_label(ax, 26, 15.8, 'Wardens of Iron', size=4.2)

    # District labels
    district_label(ax, 32, 61, 'Jade Quarter')
    district_label(ax, 20, 38, 'Lantern Quarter')
    district_label(ax, 30, 17, 'Iron Quarter')
    district_label(ax, 60, 58, 'Harbor Quarter')

    compass_rose(ax, cx=8, cy=8, size=4.5, zorder=32)
    decorative_border(ax)
    title_label(ax, 'KOWLOON')
    map_label(ax, CW/2, 4.5, 'Harbor City of the Five Houses — Coppersalt Bay',
              size=5.2, col=INK_FADED, style='italic')

    save_map(fig, 'kowloon')

# ═══════════════════════════════════════════════════════════
#  RAKUEN
# ═══════════════════════════════════════════════════════════
def draw_rakuen():
    print('Drawing Rakuen...')
    fig, ax = make_city_fig(seed=40)

    # Surroundings
    ax.add_patch(mpatches.Rectangle((0, 0), CW, CH,
                  color='#3A5028', alpha=0.18, zorder=2))

    # Three elevation zones
    fill_district(ax, [(10,8),(90,8),(90,28),(10,28)],
                  '#6A5030', alpha=0.50, zorder=3)   # Lower Terraces
    fill_district(ax, [(10,28),(90,28),(90,52),(10,52)],
                  '#A08040', alpha=0.40, zorder=3)   # Causeway Quarter
    fill_district(ax, [(10,52),(90,52),(90,72),(10,72)],
                  '#C8A860', alpha=0.35, zorder=3)   # Upper Civic Ring

    # Canopy Workshops (E side)
    fill_district(ax, [(72,28),(90,28),(90,52),(72,52)],
                  '#2A5820', alpha=0.45, zorder=4)   # Forest green cluster

    # Redwater River — flows E-W between lower and causeway
    river_pts = [(10,28),(22,30),(36,27),(50,29),(64,27),(80,30),(90,28)]
    try:
        rxs, rys = smooth_line(river_pts, tension=2.0, n=300)
        ax.plot(rxs, rys, color=RIVER_COL, linewidth=4.5, alpha=0.55, zorder=6)
        ax.plot(rxs, rys, color=SEA_LT,    linewidth=1.8, alpha=0.35, zorder=7)
    except Exception:
        arr = np.array(river_pts)
        ax.plot(arr[:,0], arr[:,1], color=RIVER_COL, linewidth=4.5, alpha=0.55, zorder=6)

    # Root-texture dots in lower terraces
    lrng = np.random.RandomState(40)
    for _ in range(80):
        rx = lrng.uniform(12, 88)
        ry = lrng.uniform(10, 26)
        ax.plot(rx, ry, '.', color=FOREST_INK, markersize=1.2, alpha=0.22, zorder=4)

    # Walkway raised lines in causeway
    for wly in [32, 36, 40, 44, 48]:
        ax.plot([10, 90], [wly, wly], color='#A89050',
                linewidth=0.5, alpha=0.20, zorder=5)

    # Low outer wall
    low_wall = [(10,8),(90,8),(90,72),(10,72)]
    draw_city_wall(ax, low_wall, col='#7A6040', lw=2.0, alpha=0.75, zorder=14)

    # Southmark Wall — heavier S edge
    ax.plot([10, 90], [8, 8], color='#5A4020', linewidth=3.5, alpha=0.88, zorder=15)
    map_label(ax, 50, 6.5, 'Southmark Wall', size=4.8, col=INK_FADED, weight='bold')

    # Elevation band dividers
    ax.plot([10, 90], [28, 28], color='#7A5020', linewidth=1.8, alpha=0.70, zorder=8)
    ax.plot([10, 90], [52, 52], color='#8A6030', linewidth=1.8, alpha=0.70, zorder=8)

    # Roads (ramps between terraces)
    draw_road(ax, [(50,8),(50,72)], lw=1.8, zorder=7)
    draw_road(ax, [(30,8),(30,72)], lw=1.3, zorder=6)
    draw_road(ax, [(70,8),(70,52)], lw=1.3, zorder=6)
    draw_road(ax, [(10,40),(90,40)], lw=1.3, zorder=6)
    draw_road(ax, [(10,62),(90,62)], lw=1.3, zorder=6)

    # Three Dispatch Houses in Causeway Quarter
    for dhx in [24, 46, 68]:
        dispatch_sym(ax, dhx, 34, size=1.1, zorder=12)
    landmark_label(ax, 46, 32, 'Three Dispatch Houses', size=4.2)

    # Bloom Pavilion — center-N in Upper Civic Ring
    pagoda_sym(ax, 50, 58, size=1.8, zorder=12)
    landmark_label(ax, 50, 55.5, 'Bloom Pavilion', size=4.5)

    # Promise Stones around Bloom Pavilion
    for pang in np.linspace(0, 2*math.pi, 7)[:-1]:
        psx = 50 + 5.5*math.cos(pang)
        psy = 62 + 5.5*math.sin(pang)
        ax.plot(psx, psy, 'o', color='#8878A0', markersize=3.8,
                alpha=0.65, zorder=11)
        ax.plot(psx, psy, 'o', color=PARCH_PALE, markersize=1.8,
                alpha=0.55, zorder=12)

    # Garden Ledger Archive
    archive_sym(ax, 20, 62, size=1.2, col='#5A4828', zorder=12)
    landmark_label(ax, 20, 59.8, 'Garden Ledger\nArchive', size=4.2)

    # Founder's Hearth
    hearth_sym(ax, 78, 62, size=1.2, zorder=12)
    landmark_label(ax, 78, 59.5, "Founder's Hearth", size=4.2)

    # Central Market in Causeway
    market_square_sym(ax, 46, 42, size=2.0, zorder=8)
    district_label(ax, 46, 42, 'Central Market', size=4.5)

    # Ration Tower symbols: 2 upper, 5 causeway
    for rtx, rty in [(28,66),(66,66)]:
        ax.plot(rtx, rty, '^', color=INK_FADED, markersize=5, alpha=0.65, zorder=12)
    for rtx, rty in [(18,44),(36,44),(60,44),(74,44),(82,44)]:
        ax.plot(rtx, rty, '^', color=INK_FADED, markersize=4.5, alpha=0.55, zorder=12)

    # Canopy Workshops label
    district_label(ax, 80, 40, 'Canopy\nWorkshops')

    # District labels
    district_label(ax, 35, 18, 'Lower Terraces')
    district_label(ax, 32, 40, 'Causeway Quarter')
    district_label(ax, 32, 63, 'Upper Civic Ring')

    compass_rose(ax, cx=8, cy=72, size=4.5, zorder=32)
    decorative_border(ax)
    title_label(ax, 'RAKUEN')
    map_label(ax, CW/2, 4.5, 'Terraced City of Root-Woven Towers — Redwater River',
              size=5.2, col=INK_FADED, style='italic')

    save_map(fig, 'rakuen')

# ═══════════════════════════════════════════════════════════
#  MAFDET
# ═══════════════════════════════════════════════════════════
def draw_mafdet():
    print('Drawing Mafdet...')
    fig, ax = make_city_fig(seed=50)

    # Coppersalt Bay — northern coast
    harbor_pts = [
        (0, 60), (0, 80), (100, 80), (100, 60),
        (90, 58), (75, 56), (60, 55), (45, 55),
        (30, 56), (15, 58), (5, 60),
    ]
    sea_fill(ax, harbor_pts, zorder=3)

    # Tidewall strip
    ax.plot([5, 95], [55, 55], color='#3A5070', linewidth=3.0, alpha=0.85, zorder=9)
    map_label(ax, 50, 57.5, 'Tidewall', size=4.5, col='#2A4060', weight='bold')

    # Harbor chain
    chain_pts = [(8, 66),(20,65),(35,64),(50,64),(65,64),(80,65),(92,66)]
    try:
        cxs, cys = smooth_line(chain_pts, tension=1.0, n=200)
        ax.plot(cxs, cys, color='#8A6020', linewidth=1.8, alpha=0.75, zorder=10,
                linestyle='--', dashes=(3, 2))
    except Exception:
        arr = np.array(chain_pts)
        ax.plot(arr[:,0], arr[:,1], color='#8A6020', linewidth=1.8, alpha=0.75,
                zorder=10, linestyle='--')
    map_label(ax, 50, 68, 'Harbor Chain', size=4.2, col='#7A5820', style='italic')

    # Bronze breakwaters
    ax.plot([10, 18, 22], [55, 62, 72], color='#8A7040', linewidth=2.5, alpha=0.72, zorder=9)
    ax.plot([90, 82, 78], [55, 62, 72], color='#8A7040', linewidth=2.5, alpha=0.72, zorder=9)

    # Piers
    for py in [60, 65, 70]:
        ax.plot([10, 22], [py, py], color='#604030', linewidth=1.0, alpha=0.55, zorder=9)
        ax.plot([78, 90], [py, py], color='#604030', linewidth=1.0, alpha=0.55, zorder=9)

    # City walls
    wall_pts = [
        (10, 8), (90, 8), (90, 55),
        (10, 55),
    ]
    draw_city_wall(ax, wall_pts, col='#8A7050', lw=2.8, alpha=0.88, zorder=15)

    # Districts
    fill_district(ax, [(10,42),(90,42),(90,55),(10,55)],
                  '#4A6880', alpha=0.40, zorder=3)   # Harbor Front N
    fill_district(ax, [(28,20),(68,20),(68,42),(28,42)],
                  PARCH_MID, alpha=0.42, zorder=3)   # Ledger Quarter center
    fill_district(ax, [(10,8),(36,8),(36,42),(10,42)],
                  '#8A6840', alpha=0.38, zorder=3)   # Carters Yard W
    fill_district(ax, [(68,8),(90,8),(90,42),(68,42)],
                  '#C0A070', alpha=0.38, zorder=3)   # Shrine Terrace E (elevated)

    # Roads
    draw_road(ax, [(50,8),(50,55)], lw=2.0, zorder=7)
    draw_road(ax, [(10,30),(90,30)], lw=2.0, zorder=7)
    draw_road(ax, [(10,20),(90,20)], lw=1.3, zorder=6)
    draw_road(ax, [(10,42),(90,42)], lw=1.3, zorder=6)
    draw_road(ax, [(28,8),(28,55)], lw=1.3, zorder=6)
    draw_road(ax, [(68,8),(68,55)], lw=1.3, zorder=6)

    # South Gate
    ax.plot(50, 8, 's', color='#706040', markersize=10, zorder=16, alpha=0.85)
    ax.plot(50, 8, 's', color=PARCH_PALE, markersize=5, zorder=17, alpha=0.75)
    gate_label(ax, 50, 8, 'South Gate', direction='S', size=4.5)

    # Shrine of First Claw — most prominent
    claw_shrine_sym(ax, 78, 24, size=2.0, zorder=14)
    landmark_label(ax, 78, 19.5, 'Shrine of\nFirst Claw', size=4.5)

    # Tide Ledger Hall
    archive_sym(ax, 50, 32, size=1.4, col='#5A4028', zorder=12)
    landmark_label(ax, 50, 29.5, 'Tide Ledger Hall', size=4.5)

    # Shoreward House
    keep_sym(ax, 50, 48, size=1.2, col='#4A5878', zorder=12)
    landmark_label(ax, 50, 45.5, 'Shoreward House', size=4.2)

    # Sand-Sea Carters Guild HQ
    keep_sym(ax, 20, 22, size=1.3, col='#7A5830', zorder=12)
    landmark_label(ax, 20, 19.5, "Carters' Guild HQ", size=4.2)

    # Storm Ledger
    archive_sym(ax, 46, 48, size=1.0, col='#4A5060', zorder=12)
    landmark_label(ax, 46, 46.2, 'Storm Ledger', size=4.2)

    # Quarantine Ledger NE near harbor
    archive_sym(ax, 78, 48, size=0.9, col='#4A5060', zorder=12)
    landmark_label(ax, 78, 46.5, 'Quarantine Ledger', size=4.2)

    # District labels
    district_label(ax, 50, 51, 'Harbor Front')
    district_label(ax, 48, 31, 'Ledger Quarter')
    district_label(ax, 22, 30, "Carters' Yard")
    district_label(ax, 78, 34, 'Shrine Terrace')

    compass_rose(ax, cx=8, cy=8, size=4.5, zorder=32)
    decorative_border(ax)
    title_label(ax, 'MAFDET')
    map_label(ax, CW/2, 4.5, 'Harbor-Trade City — Coppersalt Bay, Strand Folk & Keeper Masonry',
              size=5.2, col=INK_FADED, style='italic')

    save_map(fig, 'mafdet')

# ═══════════════════════════════════════════════════════════
#  MAIN
# ═══════════════════════════════════════════════════════════
if __name__ == '__main__':
    print('Generating city maps...')
    draw_midgaard()
    draw_kiess()
    draw_kowloon()
    draw_rakuen()
    draw_mafdet()
    print('Done.')
