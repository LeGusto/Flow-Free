#include "FlowGrid.hpp"
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
        // Clean up empty paths (due to player resets)
        while (!undoStack.empty() && undoStack.back().path->cells.empty())
        {
            undoStack.pop_back();
        }

        if (undoStack.empty())
            return;

        Action action = std::move(undoStack.back());
        undoStack.pop_back();

        switch (action.type)
        {
        case Action::ADD:
            action.pathClone = *action.path;
            redoStack.emplace_back(action);
            action.path->clearPath();
            break;
        case Action::REMOVE:

            break;
        case Action::COMPLETE:
            // action.path->shrinkPath();
            break;
        }
    };
    void redo()
    {
        if (redoStack.empty())
            return;

        Action action = std::move(redoStack.back());
        redoStack.pop_back();

        switch (action.type)
        {
        case Action::ADD:
            action.path->swapPath(action.pathClone);
            undoStack.emplace_back(action);
            break;
        case Action::REMOVE:
            // action.path->clearPath();
            break;
        case Action::COMPLETE:
            // action.path->shrinkPath();
            break;
        }
    };

private:
    std::vector<Action> undoStack;
    std::vector<Action> redoStack;
};