# Publishing the native GitHub Wiki

GitHub Wikis are stored in a **separate Git repository**. Files in `docs/` do not automatically appear at the repository's `/wiki` URL.

The repository includes `scripts/publish-wiki.ps1` to copy the existing documentation into the real Wiki, generate a sidebar, and push the changes.

## First-time setup

1. Visit [the Wiki](https://github.com/test3457890/File-System-Manager-Using-C/wiki).
2. Click **Create the first page**, name it **Home**, and save it. This initializes the Wiki's separate Git repository.
3. Make sure Git for Windows is installed and authenticated to GitHub with Wiki write access.
4. Clone or download this repository on your Windows computer.
5. In PowerShell from the repository folder, run:

```powershell
.\scripts\publish-wiki.ps1
```

This previews the changes **without pushing**. To publish the Wiki pages:

```powershell
.\scripts\publish-wiki.ps1 -Push
```

The script publishes Home, Getting Started, Architecture, Commands, Roadmap, and a navigation sidebar. It overwrites those named Wiki pages with their matching source documents, so review the source before running `-Push`. Other Wiki pages are not removed.

## Updating the Wiki later

Update the Markdown in `docs/`, commit the changes, then rerun the script with `-Push`. The native Wiki remains separate from the normal repository and is not updated automatically.

## Alternative: Git commands

Once the Wiki has been initialized, you can clone `https://github.com/test3457890/File-System-Manager-Using-C.wiki.git` and edit its Markdown files directly.

**Important:** The normal GitHub repository file API cannot publish native Wiki pages; the Wiki Git repository must be pushed separately.
