#include "MyFrame.h"
#include <wx/wx.h>
#include <wx/icon.h>

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_BUTTON(ID_AddTask, MyFrame::OnAddTask)
                EVT_BUTTON(ID_RemoveTask, MyFrame::OnRemoveTask)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400))
{
    // Load the icon based on the operating system
    wxIcon icon;
#ifdef _WIN32
    icon.LoadFile("appicon.ico", wxBITMAP_TYPE_ICO);
#else
    icon.LoadFile("appicon.png", wxBITMAP_TYPE_PNG);
#endif
    SetIcon(icon);

    wxPanel *panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(wxColour(*wxBLACK));

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    taskInput = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(taskInput, 1);

    addButton = new wxButton(panel, ID_AddTask, "Add");
    addButton->SetBackgroundColour(wxColour(*wxBLUE));
    hbox1->Add(addButton, 0, wxLEFT, 5);

    removeButton = new wxButton(panel, ID_RemoveTask, "Remove");
    removeButton->SetBackgroundColour(wxColour(*wxRED));
    hbox1->Add(removeButton, 0, wxLEFT, 5);

    vbox->Add(hbox1, 0, wxEXPAND | wxALL, 10);

    taskList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    taskList->InsertColumn(0, "ID", wxLIST_FORMAT_LEFT, 50);
    taskList->InsertColumn(1, "Task", wxLIST_FORMAT_LEFT, 300);
    taskList->InsertColumn(3, "Priority", wxLIST_FORMAT_LEFT, 70);
    taskList->SetBackgroundColour(wxColour(*wxBLACK));

    vbox->Add(taskList, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(vbox);

    db.Init();
    LoadTasks();
}

void MyFrame::OnAddTask(wxCommandEvent& event) {
    wxString task = taskInput->GetValue();
    if (!task.IsEmpty()) {
        std::string taskStr = task.ToStdString();
        long id = db.AddTask(taskStr);
        AddTaskToList(id, task);
        taskInput->Clear();
    }
}

void MyFrame::OnRemoveTask(wxCommandEvent& event) {
    long itemIndex = taskList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (itemIndex != wxNOT_FOUND) {
        long id = wxAtol(taskList->GetItemText(itemIndex));
        db.RemoveTask(id);
        taskList->DeleteItem(itemIndex);
    }
}

void MyFrame::LoadTasks() {
    auto tasks = db.GetTasks();
    for (const auto& task : tasks) {
        AddTaskToList(task.first, task.second);
    }
}

void MyFrame::AddTaskToList(long id, const wxString& task) {
    long index = taskList->InsertItem(taskList->GetItemCount(), wxString::Format(wxT("%ld"), id));
    taskList->SetItem(index, 1, task);
}
