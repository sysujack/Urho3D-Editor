#pragma once

#include "KeyBinding.h"
#include "AbstractInput.h"
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/HashSet.h>
#include <Urho3D/Input/Input.h>

namespace Urho3D
{

class AbstractMainWindow;
class Scene;
class Node;
class Serializable;
class Selection;
class GenericDialog;

/// Generic UI widget clicked.
URHO3D_EVENT(E_GENERICWIDGETCLICKED, GenericWidgetClicked)
{
    URHO3D_PARAM(P_ELEMENT, Element);   // GenericWidget ptr
    URHO3D_PARAM(P_ITEM, Item);         // GenericWidget ptr (optional)
}

class GenericDocument : public Object
{
    URHO3D_OBJECT(GenericDocument, Object);

public:
    GenericDocument(Context* context) : Object(context) { }
};

enum class DialogLocationHint
{
    Undocked,
    DockLeft,
    DockRight,
    DockTop,
    DockBottom,
};

struct AbstractAction
{
    String id_;
    String text_;
    std::function<void()> actionCallback_;
    KeyBinding keyBinding_;
};

class GenericMenu
{
public:
    virtual GenericMenu* AddMenu(const String& name) = 0;
    virtual GenericMenu* AddAction(const String& name, const String& actionId) = 0;
};

// #TODO: Rename file
class GenericWidget : public Object
{
    URHO3D_OBJECT(GenericWidget, Object);

public:
    GenericWidget(AbstractMainWindow& mainWindow, GenericWidget* parent);
    GenericWidget* GetParent() const { return parent_; }

protected:
    AbstractMainWindow& mainWindow_;

private:
    GenericWidget* parent_ = nullptr;
};

class GenericDialog : public GenericWidget
{
    URHO3D_OBJECT(GenericDialog, GenericWidget);

public:
    GenericDialog(AbstractMainWindow& mainWindow, GenericWidget* parent) : GenericWidget(mainWindow, parent) { }
    GenericWidget* CreateBodyWidget(StringHash type);
    template <class T> T* CreateBodyWidget() { return dynamic_cast<T*>(CreateBodyWidget(T::GetTypeStatic())); }
    virtual void SetBodyWidget(GenericWidget* widget) = 0;
    virtual void SetName(const String& name) = 0;
};

class GenericHierarchyListItem : public Object
{
    URHO3D_OBJECT(GenericHierarchyListItem, Object);

public:
    GenericHierarchyListItem(Context* context) : Object(context) { }
    void SetParent(GenericHierarchyListItem* parent) { parent_ = parent; }
    void SetInternalPointer(Object* internalPointer) { internalPointer_ = internalPointer; }
    Object* GetInternalPointer() const { return internalPointer_; }

    void InsertChild(GenericHierarchyListItem* item, unsigned index);
    void RemoveChild(unsigned index);
    GenericHierarchyListItem* GetParent() const { return parent_; }
    unsigned GetNumChildren() const { return children_.Size(); }
    GenericHierarchyListItem* GetChild(unsigned index) const { return index < children_.Size() ? children_[index] : nullptr; }
    int GetIndex();

    virtual String GetText() { return String::EMPTY; }

private:
    GenericHierarchyListItem* parent_ = nullptr;
    Object* internalPointer_ = nullptr;
    Vector<SharedPtr<GenericHierarchyListItem>> children_;
};

class GenericHierarchyList : public GenericWidget
{
    URHO3D_OBJECT(GenericHierarchyList, GenericWidget);

public:
    using ItemVector = PODVector<GenericHierarchyListItem*>;
    GenericHierarchyList(AbstractMainWindow& mainWindow, GenericWidget* parent) : GenericWidget(mainWindow, parent) { }
    virtual void AddItem(GenericHierarchyListItem* item, unsigned index, GenericHierarchyListItem* parent) = 0;
    virtual void SelectItem(GenericHierarchyListItem* item) = 0;
    virtual void DeselectItem(GenericHierarchyListItem* item) = 0;
    virtual void GetSelection(ItemVector& result) = 0;
    ItemVector GetSelection() { ItemVector result; GetSelection(result); return result; }
};

class AbstractMainWindow
{
public:
    virtual GenericWidget* CreateWidget(StringHash type, GenericWidget* parent) = 0;
    virtual GenericDialog* AddDialog(DialogLocationHint hint = DialogLocationHint::Undocked) = 0;
    virtual void AddAction(const AbstractAction& actionDesc) = 0;
    virtual GenericMenu* AddMenu(const String& name) = 0;

    virtual Context* GetContext() = 0;
    virtual AbstractInput* GetInput() = 0;

    template <class T> void AddAction(const String& id, KeyBinding keyBinding, T function)
    {
        AddAction({ id, "", function, keyBinding });
    }
};

}
