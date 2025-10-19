<p align="center">
    <h1>🕸️ wxWidgets Search Engine with Web Crawler</h1>
    <p>
        <a href="https://github.com/your-username/your-repo"><img alt="build" src="https://img.shields.io/badge/build-passing-brightgreen" /></a>
        <a href="https://github.com/your-username/your-repo/actions"><img alt="ci" src="https://img.shields.io/github/actions/workflow/status/your-username/your-repo/ci.yml?branch=main&label=CI" /></a>
        <a href="https://github.com/your-username/your-repo/releases"><img alt="release" src="https://img.shields.io/github/v/release/your-username/your-repo" /></a>
        <a href="LICENSE"><img alt="license" src="https://img.shields.io/badge/license-MIT-blue" /></a>
    </p>
</p>

A lightweight C++ desktop application using wxWidgets and libcurl for downloading web pages and searching them locally. Ideal as a mini offline search engine and GUI demo for recruiters.

---

## Table of Contents

- Features
- Preview
- Tech Stack
- Quick Start
- Build & Run
- How It Works
- Code Overview
- Contributing
- Author & License
- Keywords

---

## Features

- Download web pages via libcurl and save as plain-text `.txt` files (hashed filenames).
- Offline full-text search across downloaded files with filename, line number, and context snippet.
- Simple, responsive GUI built with wxWidgets.
- Self-contained: no external DB — data saved under app data/downloads.
- Search options: case sensitivity and whole-word matching.
- Keyboard shortcuts and context actions (Open, Copy snippet, Reveal in Explorer).

---

## Preview

<p align="center">
    <img src="assets/ui_fetcher.png" alt="Fetcher panel" width="520" />
    <br />
    <img src="assets/ui_search.png" alt="Search & Results panel" width="520" />
</p>

---

## Tech Stack

| Component      | Technology |
| -------------- | ---------- |
| GUI            | wxWidgets |
| HTTP Fetching  | libcurl |
| File Handling  | C++17 <filesystem> |
| Language       | C++ (STL) |
| Build          | g++, clang, or MSVC |

---

## Quick Start (Developer-friendly)

1. Clone the repo
     git clone https://github.com/your-username/your-repo.git
2. Install dependencies: wxWidgets, libcurl, C++17 toolchain
3. Build and run (examples below)

---

## Build & Run

Linux / macOS (example)
```bash
sudo apt install libwxgtk3.0-gtk3-dev libcurl4-openssl-dev
g++ main.cpp `wx-config --cxxflags --libs` -lcurl -std=c++17 -o search_engine
./search_engine
```

Windows (MinGW example)
```bash
g++ main.cpp -std=c++17 -IC:\wxWidgets\include -LC:\wxWidgets\lib -lwxmsw31u_core -lwxbase31u -lcurl -o search_engine.exe
search_engine.exe
```

Output files: <app_data>/downloads/<sha256>.txt — each file stores original URL, fetch timestamp, and plain-text content.

---

## How It Works

1. Enter URL → app downloads HTML via libcurl.  
2. Save Page → written as `page_<sha256>.txt` in downloads folder.  
3. Search → type a keyword/phrase; app scans `.txt` files and lists matches with filename, line number, and snippet.  
4. Preview → click result to open file at matching line in preview pane.

---

## Code Overview

- OnCrawl()
    - Performs HTTP GET with libcurl, normalizes HTML to text, computes SHA-256 filename, and saves the file.
- OnSearch()
    - Walks downloads directory using std::filesystem, streams files line-by-line, applies search options, and populates results list.
- UI
    - Three-pane pattern: left (downloads), top-right (fetcher), bottom-right (search & results + preview).

Suggestions for improvements:
- Multi-threaded crawling
- Inverted index for fast queries
- Highlighted matches in preview
- Recursive link crawling
- Optional SQLite integration for metadata

---

## Contributing

- Fork → create feature branch → open PR with concise description and tests (if applicable).
- Keep commits focused and include build instructions for any new dependency.

---

## Author

Rahul Singh — C++ developer exploring GUI and search engine design.  
(Replace author info with your contact/portfolio link for recruiter-friendly profile.)

---

## License

MIT License — see LICENSE file.

---

## Keywords

C++, wxWidgets, libcurl, filesystem, search-engine, web-crawler, offline-search, GUI
