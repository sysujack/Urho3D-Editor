#pragma once

#include <Urho3D/Core/Context.h>
#include <Urho3D/Engine/Engine.h>
#include <QTimer>
#include <QWidget>

namespace Urho3D
{

class Urho3DWidget : public QWidget, public Object
{
    Q_OBJECT
    URHO3D_OBJECT(Urho3DWidget, Object);

public:
    /// Construct.
    Urho3DWidget(Context* context);
    /// Return whether the widget is successfully initialized.
    bool IsInitialized() const { return engine_->IsInitialized(); }

private slots:
    /// Handle main timer.
    void OnTimer();

protected:
    virtual QPaintEngine * paintEngine() const override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    void RunFrame();

private:
    /// Engine.
    SharedPtr<Engine> engine_;
    /// Main timer.
    QTimer timer_;

};

}

