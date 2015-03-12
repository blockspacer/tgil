#ifndef GRAPHICS_COMPONENT_H
#define GRAPHICS_COMPONENT_H

#include "Component.h"
#include "Types.h"

class Model;
class RenderData;

class GraphicsComponent : public Component {
protected:
   SPtr<Model> model;

public:
   GraphicsComponent(GameObject &gameObject)
      : Component(gameObject) {}

   virtual ~GraphicsComponent() {}

   virtual void draw(const RenderData &renderData) = 0;

   SPtr<Model> getModel() const {
      return model;
   }

   void setModel(SPtr<Model> model) {
      this->model = model;
   }
};

class NullGraphicsComponent : public GraphicsComponent {
public:
   NullGraphicsComponent(GameObject &gameObject)
      : GraphicsComponent(gameObject) {}

   virtual ~NullGraphicsComponent() {}

   virtual void draw(const RenderData &renderData) {}
};

#endif
