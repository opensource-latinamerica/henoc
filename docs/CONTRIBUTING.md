# CONTRIBUTING GUIDELINES

We welcome community improvements. Please follow these steps when proposing changes:

1. **Fork and branch** – create a feature branch (e.g., `feature/new-tool`) from `main`.
2. **Keep builds green** – run `bash ./autogen.sh && ./configure && make` (or `make -C src`) to ensure Henoc compiles before opening a PR.
3. **Coding style** – prefer Qt 5 idioms, keep files in ASCII, and add concise comments for non-obvious logic. Use existing patterns in `src/FullHenoc/` and `src/HenocUniverse/` as references.
4. **Tests & demos** – when possible, update or add `.xki` examples and describe how to exercise the change. If the feature affects physics, include before/after notes in the PR.
5. **Documentation** – update `docs/ARCHITECTURE.md`, `docs/CLASS_DIAGRAMS.md`, or the README when the architecture or user workflow changes. Include migration notes if you alter configuration flags.
6. **Submit a pull request** – reference related issues, describe the motivation, list major changes, and mention manual testing performed.

For larger contributions, open a discussion first to align on scope and design. Bug reports and feature requests are tracked in GitHub issues.
