# FULLHENOC & HENOCUNIVERSE CLASS DIAGRAMS

## FULLHENOC CORE
```
FullHenoc relationships

+---------------------------+
| CfrmHenoc : QMainWindow   |
| - scene: DiagramScene*    |
| - glWidget: GLViewport*   |
| - timer: QTimer*          |
| - myWorldProp: Whstc      |
+-----------+---------------+
            |
            | owns & configures
            v
+-----------+-------------------+        creates via insert mode
| DiagramScene : QGraphicsScene |----+---> HBox : QGraphicsRectItem
| - mode / itemType             |    |     HBall : QGraphicsEllipseItem
| - default line width          |    |     HLine : QGraphicsLineItem
| - emits itemInserted()        |    |
+-----------+-------------------+    |     (each holds an HObject)
            |                        |
            | attaches               |
            v                        |
+-----------+----------------+       |
| HObject (data carrier)     |<------+
| - mass, friction, masks    |
| - rotation, color, size    |
+----------------------------+

+-----------+----------------+
| GLViewport : QOpenGLWidget |
| - setStepDelta()           |
| - paintGL() -> ODEBridge   |
+-----------+----------------+
            |
            | uses
            v
+-----------+---------------+
| ODEBridge namespace       |
| - Init/Resize/Clear       |
| - AddBox/Ball/Line        |
| - StepAndDraw()           |
+---------------------------+
```

Supporting dialogs:
- `CfrmMundo : QDialog` – edits fields in `Whstc` and presets for line thickness.
- `CfrmPeCaLi : QDialog` – edits `HObject` attributes and synchronizes mask presets with spin boxes.

## HENOCUNIVERSE PHYSICS LAYER
```
HenocUniverse core types

          +------------------------+
          | Geometry (abstract)    |
          | - SetCenter()/Axis()   |
          | - UpdateBounds()       |
          +-----------+------------+
                      ^
                      |
          +-----------+------------+
          | Shape implementations  |
          | (circle, quad, terrain)|
          +-----------+------------+

+--------------------+         owns          +-----------------------+
| Object (abstract)  |---------------------->| ObjectProperties      |
| - Move()/Rotate()  |                       | - density, friction   |
| - Property()       |                       | - masks, callbacks    |
+-----+--------------+                       +-----------------------+
      ^
      |
  +---+-------------------+        +-------------------+
  | Dynamic<G> : Object   |<>----->| G : Geometry      |
  +-----------------------+        +-------------------+
  | - Body (dBodyID)      |
  | - SetMass()           |
  +-----------------------+

  +-----------------------+
  | Static<G> : Object    |
  +-----------------------+
  | - no Body (static)    |
  +-----------------------+

+---------------------------+
| World                     |
| - dWorldID / contactGroup |
| - GenerateContacts()      |
| - SetGravity(), ERP, CFM  |
+-------------+-------------+
              |
              v
+-------------+---------------+
| ContactList                 |
| - AddContact()/Finalize()   |
| - CreateJoints()            |
+-----------------------------+
```

## DATA CARRIERS
```
HObject aggregation

+--------------+
|   HObject    |
| - mass       |
| - friction   |
| - bounce     |
| - masks      |
| - color      |
| - size       |
+------+-------+
       ^
       |
+------+-------+--------+
|  HBox   HBall   HLine |
+-----------------------+
```

