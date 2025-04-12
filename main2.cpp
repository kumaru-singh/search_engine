#include <wx/wx.h>
#include <wx/textctrl.h>
#include <curl/curl.h>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <string>

namespace fs = std::filesystem;

class MyFrame : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "Search Engine GUI with Crawler", wxDefaultPosition, wxSize(800, 600)) {
        wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

        urlInput = new wxTextCtrl(this, wxID_ANY, "https://example.com", wxDefaultPosition, wxSize(780, 25));
        wxButton* crawlBtn = new wxButton(this, wxID_ANY, "Download Page");

        searchInput = new wxTextCtrl(this, wxID_ANY, "Search term", wxDefaultPosition, wxSize(780, 25));
        wxButton* searchBtn = new wxButton(this, wxID_ANY, "Search");

        resultBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(780, 400), wxTE_MULTILINE | wxTE_READONLY);

        mainSizer->Add(urlInput, 0, wxALL, 5);
        mainSizer->Add(crawlBtn, 0, wxALL, 5);
        mainSizer->Add(searchInput, 0, wxALL, 5);
        mainSizer->Add(searchBtn, 0, wxALL, 5);
        mainSizer->Add(resultBox, 1, wxALL | wxEXPAND, 5);

        SetSizerAndFit(mainSizer);

        crawlBtn->Bind(wxEVT_BUTTON, &MyFrame::OnCrawl, this);
        searchBtn->Bind(wxEVT_BUTTON, &MyFrame::OnSearch, this);
    }

private:
    wxTextCtrl* urlInput;
    wxTextCtrl* searchInput;
    wxTextCtrl* resultBox;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t totalSize = size * nmemb;
        output->append((char*)contents, totalSize);
        return totalSize;
    }

    void OnCrawl(wxCommandEvent&) {
        std::string url = urlInput->GetValue().ToStdString();
        std::string output;

        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);
            CURLcode res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (res == CURLE_OK) {
                std::string filename = "page_" + std::to_string(std::hash<std::string>{}(url)) + ".txt";
                std::ofstream outFile(filename);
                outFile << output;
                outFile.close();
                resultBox->SetValue("Downloaded and saved to " + filename);
            } else {
                resultBox->SetValue("Failed to download URL.");
            }
        }
    }

    void OnSearch(wxCommandEvent&) {
        std::string query = searchInput->GetValue().ToStdString();
        std::ostringstream results;

        for (const auto& file : fs::directory_iterator(".")) {
            if (file.path().extension() == ".txt") {
                std::ifstream inFile(file.path());
                std::string line;
                int lineNum = 0;

                while (std::getline(inFile, line)) {
                    lineNum++;
                    if (line.find(query) != std::string::npos) {
                        results << file.path().filename().string() << ": Line " << lineNum << ": " << line << "\n";
                    }
                }
            }
        }
        resultBox->SetValue(results.str());
    }
};

class MyApp : public wxApp {
public:
    bool OnInit() override {
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
