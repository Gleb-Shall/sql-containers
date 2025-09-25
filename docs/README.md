# Skip List Documentation

This directory contains the generated Doxygen documentation for the Skip List STL container.

## Viewing Documentation

- **Online**: The documentation is automatically deployed to GitHub Pages
- **Local**: Run `make docs` to generate documentation locally
- **Files**: All HTML files are in the `html/` subdirectory

## Structure

```
docs/
├── html/           # Generated HTML documentation
│   ├── index.html  # Main documentation page
│   ├── classes.html
│   ├── functions.html
│   └── ...
└── README.md       # This file
```

## Regenerating Documentation

To regenerate the documentation locally:

```bash
make docs
```

The documentation will be updated in the `docs/html/` directory.
