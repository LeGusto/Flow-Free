#include "../headers/ActionManager.hpp"

ActionManager::ActionManager() = default;

ActionManager::~ActionManager() = default;

// Handles undoing an "Add" action
// Moves the given action to the undo stack and clears the redo stack
void ActionManager::undoAddAction(const Action &action)
{
    undoStack.emplace_back(std::move(action)); // Add the action to the undo stack
    redoStack.clear();                         // Clear the redo stack to prevent invalid redo operations
}

// Handles redoing an "Add" action
// Moves the given action to the redo stack
void ActionManager::redoAddAction(const Action &action)
{
    redoStack.emplace_back(std::move(action)); // Add the action to the redo stack
}

// Undoes the last action
void ActionManager::undo()
{
    // Remove invalid actions (e.g., actions with empty paths) from the undo stack
    while (!undoStack.empty() && undoStack.back().path->cells.empty())
    {
        undoStack.pop_back();
    }

    // If the undo stack is empty, there is nothing to undo
    if (undoStack.empty())
        return;

    // Retrieve the last action from the undo stack
    Action action = std::move(undoStack.back());
    undoStack.pop_back();

    // Handle the action based on its type
    switch (action.type)
    {
    case Action::ADD:
        // Save the current state of the path and move the action to the redo stack
        action.pathClone = *action.path;
        redoAddAction(action);
        action.path->clearPath(); // Clear the path associated with the action
        break;
    }
}

// Redoes the last undone action
void ActionManager::redo()
{
    // If the redo stack is empty, there is nothing to redo
    if (redoStack.empty())
        return;

    // Retrieve the last action from the redo stack
    Action action = std::move(redoStack.back());
    redoStack.pop_back();

    // Handle the action based on its type
    switch (action.type)
    {
    case Action::ADD:
        // Restore the path state and move the action back to the undo stack
        action.path->swapPath(action.pathClone);
        undoStack.emplace_back(std::move(action));
        break;
    }
}