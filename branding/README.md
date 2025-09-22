# Henoc Branding Assets

This folder contains SVG sources for the Henoc logo and wordmark.

## Usage and Rights

- Copyright: The Henoc logos, wordmarks, and lockups in this folder are copyrighted creative assets owned by the Henoc project maintainers. They are not covered by the software license of the codebase.
- Allowed use: You may use these assets as part of building, packaging, and distributing the Henoc application and its forks, as long as the use accurately represents the project and is non‑misleading.
- Restricted use: Do not use the logos/wordmarks to imply sponsorship, endorsement, or affiliation; do not create derivative marks or modified logos; and do not use the assets for your own product or organization branding without permission.
- Permission requests: If you want to use the Henoc logos/wordmarks outside of the project (e.g., marketing materials, articles, talks, apps, websites), please open a GitHub issue in this repository titled “Branding use request” describing the intended use, audience, visual context, and timeline. A maintainer will review and respond.

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
