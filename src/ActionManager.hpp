#include "FlowGrid.hpp"
#include "Path.hpp"

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

    Action(Type type, Path *path)
        : type(type), path(path) {}
};

class ActionManager
{
public:
    ActionManager() = default;
    ~ActionManager() = default;

    void undoAddAction(const Action &action)
    {
        undoStack.emplace_back(std::move(action));
        redoStack.clear();
    };
    void redoAddAction(const Action &action)
    {
        redoStack.emplace_back(std::move(action));
    };

    void undo()
    {
        if (undoStack.empty())
            return;
        Action action = undoStack.back();
        undoStack.pop_back();

        switch (action.type)
        {
        case Action::ADD:
            action.path->clearPath();
            break;
        case Action::REMOVE:
            // for (auto &cell : action.cells)
            // {
            //     action.path->extendPath(cell);
            // }
            break;
        case Action::COMPLETE:
            // action.path->shrinkPath();
            break;
        }
    };
    void redo(const Action &action);

private:
    std::vector<Action> undoStack;
    std::vector<Action> redoStack;
};