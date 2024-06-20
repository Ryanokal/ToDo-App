#include "MyApp.h"
#include "MyFrame.h"

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame("To-Do App");
    frame->Show(true);
    return true;
}
