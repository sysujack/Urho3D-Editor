#pragma once

#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/HashSet.h>
// #include "SceneOverlay.h"
// #include "../Core/Document.h"
// #include <QAction>
// #include <QSet>
// #include <QUndoStack>
// #include <Urho3D/Graphics/Camera.h>
// #include <Urho3D/Graphics/Viewport.h>
// #include <Urho3D/Scene/Node.h>
// #include <Urho3D/Scene/Scene.h>

namespace Urho3D
{

class Input;
class Node;
class Component;

/// Selection action.
enum class SelectionAction
{
    /// Select object.
    Select,
    /// Deselect object.
    Deselect,
    /// Flip selection.
    Toggle
};

/// Selection.
class Selection : public Object
{
    URHO3D_OBJECT(Selection, Object);

public:
    /// Set of objects.
    using ObjectSet = HashSet<Object*>;
    /// Set of nodes.
    using NodeSet = HashSet<Node*>;
    /// Set of components.
    using ComponentSet = HashSet<Component*>;
    /// Construct.
    Selection(Context* context) : Object(context) { }

    /// Clear selection.
    void ClearSelection();
    /// Select objects.
    void SetSelection(const ObjectSet& objects);
    /// Select object.
    void SelectObject(Object* object, SelectionAction action, bool clearSelection);
    /// Set hovered object.
    void SetHoveredObject(Object* object);

    /// Get selected objects.
    const ObjectSet& GetSelected() const { return selectedObjects_; }
    /// Get selected nodes.
    const NodeSet& GetSelectedNodes() const { return selectedNodes_; }
    /// Get selected components.
    const ComponentSet& GetSelectedComponents() const { return selectedComponents_; }
    /// Get selected nodes and components.
    const NodeSet& GetSelectedNodesAndComponents() const { return selectedNodesAndComponents_; }
    /// Get center point of selected nodes.
    Vector3 GetSelectedCenter();
    /// Get hovered object.
    Object* GetHoveredObject() const { return hoveredObject_; }
    /// Get hovered node.
    Node* GetHoveredNode() const;
    /// Get hovered component.
    Component* GetHoveredComponent() const;

public:
    /// Called when selection is changed.
    std::function<void()> onSelectionChanged_;

private:
    /// Gather secondary selection lists.
    void UpdateChangedSelection();

private:
    /// Selected objects.
    ObjectSet selectedObjects_;
    /// Selected nodes.
    NodeSet selectedNodes_;
    /// Selected components.
    ComponentSet selectedComponents_;
    /// Selected nodes and components.
    NodeSet selectedNodesAndComponents_;
    /// Hovered object.
    Object* hoveredObject_ = nullptr;
    /// Last center of selected nodes and components.
    /// \todo Is it needed?
    Vector3 lastSelectedCenter_;

};

}

