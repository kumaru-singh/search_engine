#pragma once
#include <wx/wx.h>
#include "../core/Crawler.h"
#include "../core/Indexer.h"
#include "../core/Searcher.h"
#include "../utils/ThreadPool.h"

class MyFrame : public wxFrame {
public:
    MyFrame();

private:
    wxTextCtrl* urlInput;
    wxTextCtrl* searchInput;
    wxTextCtrl* resultBox;

    ThreadPool pool;
    Indexer indexer;
    Searcher searcher;

    void OnCrawl(wxCommandEvent&);
    void OnSearch(wxCommandEvent&);
};
