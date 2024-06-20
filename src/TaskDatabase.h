#ifndef TASKDATABASE_H
#define TASKDATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>

class TaskDatabase {
public:
    TaskDatabase();
    ~TaskDatabase();

    void Init();
    long AddTask(const std::string& task);
    void RemoveTask(long id);
    std::vector<std::pair<long, std::string>> GetTasks();

private:
    sqlite3 *db;
};

#endif // TASKDATABASE_H
