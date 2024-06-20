#include "TaskDatabase.h"
#include <stdexcept>

TaskDatabase::TaskDatabase() : db(nullptr) {}

TaskDatabase::~TaskDatabase() {
    if (db) {
        sqlite3_close(db);
    }
}

void TaskDatabase::Init() {
    if (sqlite3_open("tasks.db", &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open database");
    }

    const char *createTableQuery = "CREATE TABLE IF NOT EXISTS tasks ("
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                   "task TEXT NOT NULL)";
    char *errMsg = nullptr;
    if (sqlite3_exec(db, createTableQuery, nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::string error = "Failed to create table: ";
        error += errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}

long TaskDatabase::AddTask(const std::string& task) {
    const char *insertQuery = "INSERT INTO tasks (task) VALUES (?)";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare insert statement");
    }

    sqlite3_bind_text(stmt, 1, task.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to insert task");
    }

    long id = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    return id;
}

void TaskDatabase::RemoveTask(long id) {
    const char *deleteQuery = "DELETE FROM tasks WHERE id = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, deleteQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare delete statement");
    }

    sqlite3_bind_int(stmt, 1, id);
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to delete task");
    }

    sqlite3_finalize(stmt);
}

std::vector<std::pair<long, std::string>> TaskDatabase::GetTasks() {
    const char *selectQuery = "SELECT id, task FROM tasks";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, selectQuery, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare select statement");
    }

    std::vector<std::pair<long, std::string>> tasks;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        long id = sqlite3_column_int(stmt, 0);
        std::string task = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        tasks.emplace_back(id, task);
    }

    sqlite3_finalize(stmt);
    return tasks;
}
