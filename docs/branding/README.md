# Project visuals

- `overview.svg` is the editable banner displayed at the top of the repository README.
- `social-preview.svg` is the editable source for the repository's share image.
- `social-preview.png` is a 1280 by 640 PNG export for GitHub's Social preview setting.

The SVG files use local text and vector shapes, with no scripts, external fonts, or tracking services. They follow the portfolio's blue, paper, and ink colors. The filter, terrain, and compiler images remain actual program output in `docs/assets/`.

After editing the share image, export it with Inkscape:

```bash
inkscape docs/branding/social-preview.svg --export-type=png --export-filename=docs/branding/social-preview.png
```
