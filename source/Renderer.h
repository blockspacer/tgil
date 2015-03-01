#ifndef RENDERER_H
#define RENDERER_H

#include "HUDRenderer.h"
#include "DebugRenderer.h"

#include <glm/glm.hpp>

class GameObject;

class Renderer {
protected:
   /**
    * Renderer used to draw the HUD for each player
    */
   HUDRenderer hudRenderer;

   /**
    * Renderer used to draw debug physics information
    */
   DebugRenderer debugRenderer;

   /**
    * Width of the framebuffer (in pixels)
    */
   int width;

   /**
    * Height of the framebuffer (in pixels)
    */
   int height;

   /**
    * Field of view of the perspective projection
    */
   float fov;

   /**
    * Number of frames that need clear calls
    */
   int clearFramesNeeded;

   /**
    * The perspective projection
    */
   glm::mat4 projectionMatrix;

   /**
    * If debug rendering is enabled
    */
   bool renderDebug;

   /**
    * Renders the scene from the given camera's perspective
    */
   void renderFromCamera(Scene &scene, const GameObject &camera);

   /**
    * Renders the debug physics information for the scene
    */
   void renderDebugInfo(Scene &scene, const glm::mat4 &viewMatrix);

public:
   Renderer();

   virtual ~Renderer();

   /**
    * Initializes the renderer, setting OpenGL properties and preparing rendering subsystems
    */
   void init(float fov, int width, int height);

   /**
    * Updates the renderer to match the new framebuffer resolution
    */
   void onFramebufferSizeChange(int width, int height);

   /**
    * Renders the given scene
    */
   void render(Scene &scene);

   /**
    * Enables / disables rendering of debug physics information
    */
   void enableDebugRendering(bool enabled) {
      renderDebug = enabled;
   }

   /**
    * Returns whether debug physics rendering is enabled
    */
   bool debugRenderingEnabled() const {
      return renderDebug;
   }
};

#endif
