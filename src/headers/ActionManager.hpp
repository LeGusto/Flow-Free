#pragma once

#include "PathManager.hpp"

#pragma once

struct Action
{
    enum Type
    {
        ADD,
        REMOVE,
        COMPLETE
    };

    Type type;
    Path *path;
    // For Redo
    mutable Path pathClone;

    Action(Type type, Path *path)
        : type(type), path(path) {}
};

class ActionManager
{
public:
    ActionManager();
    ~ActionManager();

    void undoAddAction(const Action &action);
    void redoAddAction(const Action &action);

    void undo();
    void redo();

private:
    std::vector<Action> undoStack;
    std::vector<Action> redoStack;
};