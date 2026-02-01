#include "MyFrame.h"
#include "../core/HtmlParser.h"
#include <fstream>
#include <sstream>

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "MiniSearch Engine", wxDefaultPosition, wxSize(800,600)),
      searcher(indexer) {

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    urlInput = new wxTextCtrl(this, wxID_ANY, "https://example.com");
    wxButton* crawlBtn = new wxButton(this, wxID_ANY, "Crawl");

    searchInput = new wxTextCtrl(this, wxID_ANY, "search term");
    wxButton* searchBtn = new wxButton(this, wxID_ANY, "Search");

    resultBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                               wxTE_MULTILINE | wxTE_READONLY);

    sizer->Add(urlInput, 0, wxEXPAND | wxALL, 5);
    sizer->Add(crawlBtn, 0, wxEXPAND | wxALL, 5);
    sizer->Add(searchInput, 0, wxEXPAND | wxALL, 5);
    sizer->Add(searchBtn, 0, wxEXPAND | wxALL, 5);
    sizer->Add(resultBox, 1, wxEXPAND | wxALL, 5);

    SetSizer(sizer);

    crawlBtn->Bind(wxEVT_BUTTON, &MyFrame::OnCrawl, this);
    searchBtn->Bind(wxEVT_BUTTON, &MyFrame::OnSearch, this);
}

void MyFrame::OnCrawl(wxCommandEvent&) {
    std::string url = urlInput->GetValue().ToStdString();
    pool.submit([this, url] {
        Crawler crawler;
        std::string html;
        bool ok = crawler.fetch(url, html);
        if (!ok) return;

        std::string text = HtmlParser::stripTags(html);
        static int docId = 0;
        indexer.addDocument(docId++, text);
    });
}

void MyFrame::OnSearch(wxCommandEvent&) {
    auto results = searcher.search(searchInput->GetValue().ToStdString());
    std::stringstream ss;
    for (auto& r : results)
        ss << "Doc " << r.docId << " Score: " << r.score << "\n";
    resultBox->SetValue(ss.str());
}
