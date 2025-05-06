#include "../headers/ActionManager.hpp"

ActionManager::ActionManager() = default;
ActionManager::~ActionManager() = default;

void ActionManager::undoAddAction(const Action &action)
{
    undoStack.emplace_back(std::move(action));
    redoStack.clear();
}

void ActionManager::redoAddAction(const Action &action)
{
    redoStack.emplace_back(std::move(action));
}

void ActionManager::undo()
{
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
        redoAddAction(action);
        action.path->clearPath();
        break;
    }
}

void ActionManager::redo()
{
    if (redoStack.empty())
        return;

    Action action = std::move(redoStack.back());
    redoStack.pop_back();

    switch (action.type)
    {
    case Action::ADD:
        action.path->swapPath(action.pathClone);
        undoAddAction(action);
        break;
    }
}