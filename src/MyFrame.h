#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "TaskDatabase.h"

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);

private:
    void OnAddTask(wxCommandEvent& event);
    void OnRemoveTask(wxCommandEvent& event);

    wxTextCtrl *taskInput;
    wxButton *addButton;
    wxButton *removeButton;
    wxListCtrl *taskList;

    TaskDatabase db;

    void LoadTasks();
    void AddTaskToList(long id, const wxString& task);

wxDECLARE_EVENT_TABLE();
};

enum {
    ID_AddTask = 1,
    ID_RemoveTask = 2
};

#endif // MYFRAME_H
