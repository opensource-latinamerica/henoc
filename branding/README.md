# Henoc Branding Assets

This folder contains SVG sources for the Henoc logo and wordmark.

## Files

- icon-h-solid-256.svg: Solid H mark (rounded square), monochrome.
- icon-h-solid-32.svg: Pixel-hinted small H (32×32), monochrome.
- icon-h-32px-hinted.svg: Alternative pixel-hinted H (32×32).
- icon-h-gradient-256.svg: H with gradient beam + ball.
- catapult-stroke.svg: Minimal catapult stroke icon.
- frame-ball-solid.svg: ENMARCA frame + ball concept.
- wordmark-henoc.svg: Geometric wordmark (paths), full size.
- wordmark-henoc-light.svg / wordmark-henoc-dark.svg: Light/dark variants.
- wordmark-henoc-topbar.svg: Compact single-line wordmark for app bars.
- lockup-icon-wordmark-light.svg / lockup-icon-wordmark-dark.svg: Icon + wordmark lockups.

## Export to PNG

Use the helper script to export common sizes (16/32/64/128/256) for every SVG.

```bash
cd branding
./export.sh
```

Requirements: one of Inkscape, rsvg-convert (librsvg), or ImageMagick's `convert` must be installed and on PATH.

PNG files will be written to `branding/png/` with `-SIZE` suffixes.

## Notes

- All SVGs are pure vector and can be edited with Inkscape, Illustrator, or a code editor.
- Colors: neutrals (#111827, #0B1220) plus accents (#22D3EE, #A78BFA, #FB7185, #10B981 / #A7F3D0).
- Wordmarks are outlined paths (no font dependency).
- For favicons/app icons, prefer the solid H at small sizes and the gradient H for larger.
