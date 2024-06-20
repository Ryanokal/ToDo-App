#include "MyFrame.h"

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
                EVT_BUTTON(ID_AddTask, MyFrame::OnAddTask)
                EVT_BUTTON(ID_RemoveTask, MyFrame::OnRemoveTask)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300))
{
    wxPanel *panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    taskInput = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(taskInput, 1);

    addButton = new wxButton(panel, ID_AddTask, "Add");
    hbox1->Add(addButton, 0, wxLEFT, 5);

    removeButton = new wxButton(panel, ID_RemoveTask, "Remove");
    hbox1->Add(removeButton, 0, wxLEFT, 5);

    vbox->Add(hbox1, 0, wxEXPAND | wxALL, 10);

    taskList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    taskList->InsertColumn(0, "ID", wxLIST_FORMAT_LEFT, 50);
    taskList->InsertColumn(1, "Task", wxLIST_FORMAT_LEFT, 300);

    vbox->Add(taskList, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(vbox);

    db.Init();
    LoadTasks();
}

void MyFrame::OnAddTask(wxCommandEvent& event) {
    wxString task = taskInput->GetValue();
    if (!task.IsEmpty()) {
        // Convert wxString to std::string
        std::string taskStr = task.ToStdString();

        // Call AddTask using the converted string
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
